#include "Game.h"

#include <random>

Game::Game()
    : window(sf::VideoMode(1000, 600), "Dungeon Crawler"),
      hero(1, 1),
      state(GameState::PLAYING),
      active_chest_index(-1) {
  window.setFramerateLimit(60);
  setupEventListeners();

  spawnMonsters();
  spawnChests();
}

void Game::setupEventListeners() {
  // Observer: подписываемся на события игры
  EventManager::getInstance().subscribe("monster_died",
    [this](const std::string& name) {
      ui.addLog(name + " defeated!");
    });

  EventManager::getInstance().subscribe("hero_exhausted",
    [this](const std::string&) {
      ui.addLog("You are exhausted! Rest (Space).");
    });

  EventManager::getInstance().subscribe("hero_rested",
    [this](const std::string& data) {
      if (data == "healed")
        ui.addLog("Rested: -10 MP +8 HP");
      else
        ui.addLog("Rested. (no mana to heal)");
    });
}

void Game::run() {
  while (window.isOpen()) {
    processEvents();
    update();
    render();
  }
}

void Game::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }

    // Перезапуск
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::R) {
      restart();
      return;
    }

    // Управление только во время игры
    if (state == GameState::PLAYING) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) handleMove(0, -1);
        if (event.key.code == sf::Keyboard::S) handleMove(0, 1);
        if (event.key.code == sf::Keyboard::A) handleMove(-1, 0);
        if (event.key.code == sf::Keyboard::D) handleMove(1, 0);
        if (event.key.code == sf::Keyboard::Space) handleRest();
      }
    }

    // Клик мышью — открытие сундука или выбор награды
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      // mapPixelToCoords корректирует координаты при любом размере окна
      sf::Vector2f world = window.mapPixelToCoords(
          sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
      sf::Vector2i mouse_pos(static_cast<int>(world.x),
                             static_cast<int>(world.y));

      if (state == GameState::CHEST_OPEN) {
        handleChestClick(mouse_pos);
      } else if (state == GameState::PLAYING) {
        handleChestClick(mouse_pos);
      }
    }
  }
}

void Game::handleMove(int dx, int dy) {
  int new_x = hero.getX() + dx;
  int new_y = hero.getY() + dy;

  // Проверяем нет ли монстра на новой клетке — если есть, атакуем
  for (auto& monster : monsters) {
    if (!monster->isAlive()) continue;
    if (monster->getX() == new_x && monster->getY() == new_y) {
      std::string log = CombatSystem::doRound(hero, *monster);
      ui.addLog(log);
      hero.onMove();  // бой тоже считается как движение
      checkWinLose();
      return;
    }
  }

  // Проверяем, не стена ли
  if (!map.isWalkable(new_x, new_y)) return;

  // Перемещаем героя
  hero.setX(new_x);
  hero.setY(new_y);
  hero.onMove();

  // Если герой на зоне ярости — применяем эффект
  if (map.isRageZone(new_x, new_y)) {
    applyRageZoneEffect();
  }

  // Монстры рядом атакуют героя (если герой к ним подошёл)
  for (auto& monster : monsters) {
    if (!monster->isAlive()) continue;
    if (CombatSystem::isAdjacent(hero, *monster)) {
      // Монстр атакует в ответ на приближение
      int dmg = monster->getAttack();
      Troll* troll = dynamic_cast<Troll*>(monster.get());
      if (troll) dmg = troll->getAttackDamage();

      Goblin* goblin = dynamic_cast<Goblin*>(monster.get());
      if (goblin && goblin->isRunning()) dmg = 0;

      if (dmg > 0) {
        hero.takeDamage(dmg);
        ui.addLog(monster->getName() + " hits you for " +
                  std::to_string(dmg));
      }
    }
  }

  checkWinLose();
}

void Game::handleRest() {
  hero.onRest();
  // Монстры рядом атакуют пока герой отдыхает
  for (auto& monster : monsters) {
    if (!monster->isAlive()) continue;
    if (CombatSystem::isAdjacent(hero, *monster)) {
      int dmg = monster->getAttack();
      hero.takeDamage(dmg);
      ui.addLog(monster->getName() + " attacks while you rest! -" +
                std::to_string(dmg));
    }
  }
  checkWinLose();
}

void Game::applyRageZoneEffect() {
  // Зона ярости: каждый ход на ней герой теряет 3 HP
  hero.takeDamage(3);
  ui.addLog("! DANGER ZONE ! -3 HP", sf::Color(220, 50, 50));
}

void Game::handleChestClick(sf::Vector2i mouse_pos) {
  // Если уже в режиме выбора награды
  if (state == GameState::CHEST_OPEN && active_chest_index >= 0) {
    Chest& chest = chests[active_chest_index];
    int choice = chest.checkClick(mouse_pos);
    if (choice == 1) {
      applyLoot(chest.getOptionA());
      chest.open();
      state = GameState::PLAYING;
      active_chest_index = -1;
    } else if (choice == 2) {
      applyLoot(chest.getOptionB());
      chest.open();
      state = GameState::PLAYING;
      active_chest_index = -1;
    }
    return;
  }

  // Кликаем на сундук на карте
  for (int i = 0; i < static_cast<int>(chests.size()); i++) {
    if (chests[i].isOpen()) continue;
    int cx = chests[i].getX() * TILE_SIZE;
    int cy = chests[i].getY() * TILE_SIZE;
    sf::FloatRect chest_rect(cx, cy, TILE_SIZE, TILE_SIZE);

    if (chest_rect.contains(sf::Vector2f(mouse_pos))) {
      // Герой должен быть рядом с сундуком
      int dx = std::abs(hero.getX() - chests[i].getX());
      int dy = std::abs(hero.getY() - chests[i].getY());
      if (dx <= 1 && dy <= 1) {
        active_chest_index = i;
        state = GameState::CHEST_OPEN;
      } else {
        ui.addLog("Get closer to the chest!");
      }
    }
  }
}

void Game::applyLoot(const LootOption& loot) {
  switch (loot.type) {
    case LootType::HEAL:
      hero.addMaxHp(loot.value);
      ui.addLog("Max HP +" + std::to_string(loot.value));
      break;
    case LootType::ATTACK_BOOST:
      hero.addAttack(loot.value);
      ui.addLog("ATK +" + std::to_string(loot.value));
      break;
    case LootType::MANA:
      hero.restoreMana(loot.value);
      ui.addLog("Mana +" + std::to_string(loot.value));
      break;
  }
}

void Game::spawnMonsters() {
  monsters.clear();

  // Собираем позиции всех клеток ярости с карты
  std::vector<std::pair<int,int>> rage_cells;
  for (int y = 0; y < Map::HEIGHT; ++y)
    for (int x = 0; x < Map::WIDTH; ++x)
      if (map.isRageZone(x, y))
        rage_cells.push_back({x, y});

  std::mt19937 rng(std::random_device{}());
  auto randInt = [&](int lo, int hi) {
    return std::uniform_int_distribution<int>(lo, hi)(rng);
  };

  auto isTaken = [&](int x, int y) {
    if (x == hero.getX() && y == hero.getY()) return true;
    for (const auto& m : monsters)
      if (m->getX() == x && m->getY() == y) return true;
    return false;
  };

  // Минимальное расстояние от стартовой позиции героя
  auto tooClose = [&](int x, int y) {
    return std::abs(x - hero.getX()) + std::abs(y - hero.getY()) < 6;
  };

  // Пытается разместить монстра рядом с случайной клеткой ярости (радиус 3)
  // При неудаче — в любом проходимом месте
  auto place = [&](auto makeMonster) {
    if (!rage_cells.empty()) {
      auto [rx, ry] = rage_cells[randInt(0, static_cast<int>(rage_cells.size()) - 1)];
      for (int attempt = 0; attempt < 150; ++attempt) {
        int x = rx + randInt(-3, 3);
        int y = ry + randInt(-3, 3);
        if (map.isWalkable(x, y) && !isTaken(x, y) && !tooClose(x, y)) {
          monsters.push_back(makeMonster(x, y));
          return;
        }
      }
    }
    for (int attempt = 0; attempt < 500; ++attempt) {
      int x = randInt(1, Map::WIDTH - 2);
      int y = randInt(1, Map::HEIGHT - 2);
      if (map.isWalkable(x, y) && !isTaken(x, y) && !tooClose(x, y)) {
        monsters.push_back(makeMonster(x, y));
        return;
      }
    }
  };

  place([](int x, int y) { return std::make_unique<Goblin>(x, y); });
  place([](int x, int y) { return std::make_unique<Goblin>(x, y); });
  place([](int x, int y) { return std::make_unique<Troll>(x, y); });
  place([](int x, int y) { return std::make_unique<Troll>(x, y); });
}

void Game::spawnChests() {
  static const LootOption loot_pool[] = {
      {LootType::HEAL,         "+30 HP",   30},
      {LootType::HEAL,         "+20 HP",   20},
      {LootType::ATTACK_BOOST, "+5 ATK",    5},
      {LootType::ATTACK_BOOST, "+3 ATK",    3},
      {LootType::MANA,         "+25 Mana", 25},
      {LootType::MANA,         "+15 Mana", 15},
  };
  static constexpr int POOL_SIZE = static_cast<int>(std::size(loot_pool));

  std::mt19937 rng(std::random_device{}());
  auto randInt = [&](int lo, int hi) {
    return std::uniform_int_distribution<int>(lo, hi)(rng);
  };

  // Занятые клетки: герой, монстры, уже размещённые сундуки
  auto isTaken = [&](int x, int y) {
    if (x == hero.getX() && y == hero.getY()) return true;
    for (const auto& m : monsters)
      if (m->getX() == x && m->getY() == y) return true;
    for (const auto& c : chests)
      if (c.getX() == x && c.getY() == y) return true;
    return false;
  };

  int count = randInt(2, 4);
  int attempts = 0;
  while (static_cast<int>(chests.size()) < count && attempts < 500) {
    ++attempts;
    int x = randInt(1, Map::WIDTH - 2);
    int y = randInt(1, Map::HEIGHT - 2);
    if (!map.isWalkable(x, y) || isTaken(x, y)) continue;

    int ia = randInt(0, POOL_SIZE - 1);
    std::vector<int> other;
    for (int i = 0; i < POOL_SIZE; ++i)
      if (loot_pool[i].type != loot_pool[ia].type)
        other.push_back(i);
    int ib = other[randInt(0, static_cast<int>(other.size()) - 1)];

    chests.emplace_back(x, y, loot_pool[ia], loot_pool[ib]);
  }
}

void Game::checkWinLose() {
  if (!hero.isAlive()) {
    state = GameState::GAME_OVER;
    return;
  }

  // Победа — все монстры мертвы
  bool all_dead = true;
  for (const auto& m : monsters) {
    if (m->isAlive()) {
      all_dead = false;
      break;
    }
  }
  if (all_dead) {
    state = GameState::VICTORY;
  }
}

void Game::restart() {
  hero = Hero(1, 1);
  map = Map();
  chests.clear();
  spawnMonsters();
  spawnChests();

  ui.clearLog();
  state = GameState::PLAYING;
  active_chest_index = -1;
}

void Game::update() {
  // Можно добавить анимации или таймеры здесь
}

void Game::render() {
  window.clear(sf::Color(20, 15, 10));

  // Рисуем карту
  map.draw(window, TILE_SIZE);

  // Рисуем сундуки
  for (const auto& chest : chests) {
    chest.draw(window, TILE_SIZE);
  }

  // Рисуем монстров
  for (const auto& monster : monsters) {
    if (monster->isAlive()) {
      monster->draw(window, TILE_SIZE);
    }
  }

  // Рисуем героя
  hero.draw(window, TILE_SIZE);

  ui.draw(window, hero, monsters);

  // Окно выбора сундука
  if (state == GameState::CHEST_OPEN && active_chest_index >= 0) {
    chests[active_chest_index].drawLootWindow(window);
  }

  // Экран конца игры
  if (state == GameState::GAME_OVER) {
    ui.drawGameOver(window, false);
  } else if (state == GameState::VICTORY) {
    ui.drawGameOver(window, true);
  }

  window.display();
}
