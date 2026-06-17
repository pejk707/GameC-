#include "Monster.h"

#include "EventManager.h"
#include "FontCache.h"

// ---- Monster ----

void Monster::takeDamage(int damage) {
  hp -= damage;
  is_alerted = true;

  if (!isAlive()) {
    EventManager::getInstance().notify("monster_died", getName());
  }
}

// ---- Goblin ----

void Goblin::draw(sf::RenderWindow& window, int tile_size) const {
  sf::RectangleShape rect(sf::Vector2f(tile_size - 2, tile_size - 2));
  rect.setPosition(x * tile_size + 1, y * tile_size + 1);
  rect.setFillColor(isRunning() ? sf::Color(200, 200, 0) : sf::Color(0, 160, 0));
  window.draw(rect);

  if (const sf::Font* font = getFont()) {
    sf::Text text("g", *font, tile_size - 8);
    text.setFillColor(sf::Color::White);
    text.setPosition(x * tile_size + 6, y * tile_size + 2);
    window.draw(text);
  }
}

// ---- Troll ----

void Troll::draw(sf::RenderWindow& window, int tile_size) const {
  sf::RectangleShape rect(sf::Vector2f(tile_size - 2, tile_size - 2));
  rect.setPosition(x * tile_size + 1, y * tile_size + 1);
  rect.setFillColor((attack_counter == 1) ? sf::Color(220, 0, 0) : sf::Color(120, 30, 30));
  window.draw(rect);

  if (const sf::Font* font = getFont()) {
    sf::Text text("T", *font, tile_size - 8);
    text.setFillColor(sf::Color::White);
    text.setPosition(x * tile_size + 5, y * tile_size + 2);
    window.draw(text);
  }
}

int Troll::getAttackDamage() {
  attack_counter++;
  if (attack_counter >= 2) {
    attack_counter = 0;
    return attack * 2;  // усиленный удар раз в 2 хода
  }
  return attack;
}
