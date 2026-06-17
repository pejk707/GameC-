#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Chest.h"
#include "CombatSystem.h"
#include "EventManager.h"
#include "Hero.h"
#include "Map.h"
#include "Monster.h"
#include "UI.h"

enum class GameState { PLAYING, CHEST_OPEN, GAME_OVER, VICTORY };

// Game — главный класс, управляет игровым циклом
// Связывает все системы вместе
class Game {
 public:
  Game();

  void run();

 private:
  void processEvents();
  void update();
  void render();

  // Обработка хода игрока
  void handleMove(int dx, int dy);
  void handleRest();

  // Применение зоны ярости
  void applyRageZoneEffect();

  // Попытка открыть сундук по клику мыши
  void handleChestClick(sf::Vector2i mouse_pos);

  // Применение награды из сундука
  void applyLoot(const LootOption& loot);

  // Проверка условий победы/поражения
  void checkWinLose();

  // Перезапуск игры
  void restart();

  // Генерация монстров рядом с зонами ярости
  void spawnMonsters();

  // Генерация сундуков (4..6 штук) на случайных проходимых клетках
  void spawnChests();

  // Подписка на события (Observer)
  void setupEventListeners();

  sf::RenderWindow window;
  Map map;
  Hero hero;
  std::vector<std::unique_ptr<Monster>> monsters;
  std::vector<Chest> chests;
  UI ui;

  GameState state;
  int active_chest_index;  // индекс открытого сундука (-1 = нет)

  static const int TILE_SIZE = 40;
};
