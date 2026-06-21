#include "Monster.h"

#include "EventManager.h"
#include "SpriteCache.h"

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
  const sf::Texture& tex = getSpriteSet().goblin();
  sf::Sprite spr(tex);
  auto sz = tex.getSize();
  spr.setScale({(float)tile_size / sz.x, (float)tile_size / sz.y});
  spr.setPosition({(float)(x * tile_size), (float)(y * tile_size)});

  // Убегающий гоблин — желтоватый оттенок
  if (isRunning()) spr.setColor(sf::Color(255, 255, 160));
  window.draw(spr);
}

// ---- Troll ----

void Troll::draw(sf::RenderWindow& window, int tile_size) const {
  const sf::Texture& tex = getSpriteSet().troll();
  sf::Sprite spr(tex);
  auto sz = tex.getSize();
  spr.setScale({(float)tile_size / sz.x, (float)tile_size / sz.y});
  spr.setPosition({(float)(x * tile_size), (float)(y * tile_size)});

  // Ярость перед ударом — красный оттенок
  if (attack_counter == 1) spr.setColor(sf::Color(255, 160, 160));
  window.draw(spr);
}

int Troll::getAttackDamage() {
  attack_counter++;
  if (attack_counter >= 2) {
    attack_counter = 0;
    return attack * 2;
  }
  return attack;
}
