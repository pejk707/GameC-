#include "Hero.h"

#include "EventManager.h"
#include "FontCache.h"

Hero::Hero(int x, int y)
    : Entity(x, y, 100, 15, 120),
      fatigue(0),
      max_fatigue(5),
      mana(50),
      max_mana(50) {}

void Hero::draw(sf::RenderWindow& window, int tile_size) const {
  sf::RectangleShape rect(sf::Vector2f(tile_size - 2, tile_size - 2));
  rect.setPosition(x * tile_size + 1, y * tile_size + 1);
  rect.setFillColor(sf::Color(30, 100, 200));
  window.draw(rect);

  if (const sf::Font* font = getFont()) {
    sf::Text text("@", *font, tile_size - 8);
    text.setFillColor(sf::Color::White);
    text.setPosition(x * tile_size + 4, y * tile_size + 2);
    window.draw(text);
  }
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
