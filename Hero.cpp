#include "Hero.h"

#include "EventManager.h"
#include "SpriteCache.h"

Hero::Hero(int x, int y)
    : Entity(x, y, 100, 15, 120),
      fatigue(0),
      max_fatigue(5),
      mana(50),
      max_mana(50) {}

void Hero::draw(sf::RenderWindow& window, int tile_size) const {
  const sf::Texture& tex = getSpriteSet().hero();
  sf::Sprite spr(tex);
  auto sz = tex.getSize();
  spr.setScale({(float)tile_size / sz.x, (float)tile_size / sz.y});
  spr.setPosition({(float)(x * tile_size), (float)(y * tile_size)});
  window.draw(spr);
}

void Hero::onMove() {
  // Каждый шаг увеличивает усталость
  if (fatigue < max_fatigue) {
    fatigue++;
  }

  if (fatigue >= max_fatigue) {
    EventManager::getInstance().notify("hero_exhausted");
  }
}

void Hero::onRest() {
  bool exhausted = (fatigue == 0);
  fatigue = 0;
  if (exhausted && mana >= 10) {
    useMana(10);
    heal(8);
    EventManager::getInstance().notify("hero_rested", "healed");
  } else {
    EventManager::getInstance().notify("hero_rested", "");
  }
}

int Hero::getEffectiveAttack() const {
  // Чем выше усталость — тем меньше урон (минимум 70% от базового)
  float fatigue_ratio = static_cast<float>(fatigue) / max_fatigue;
  float penalty = 1.0f - (fatigue_ratio * 0.3f);
  return static_cast<int>(attack * penalty);
}
