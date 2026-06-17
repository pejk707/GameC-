#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

#include "Entity.h"

// Герой — управляемый игроком персонаж
// Содержит механику усталости (Fatigue):
//   чем больше шагов подряд без отдыха — тем меньше урон
class Hero : public Entity {
 public:
  Hero(int x, int y);

  void draw(sf::RenderWindow& window, int tile_size) const override;
  std::string getName() const override { return "Hero"; }

  // Вызывается при каждом шаге — накапливает усталость
  void onMove();

  // Вызывается при отдыхе (стоять на месте 1 ход)
  void onRest();

  // Урон с учётом усталости
  int getEffectiveAttack() const;

  // Мана для отображения в UI
  int getMana() const { return mana; }
  int getMaxMana() const { return max_mana; }
  void useMana(int amount) { mana = std::max(0, mana - amount); }
  void restoreMana(int amount) { mana = std::min(mana + amount, max_mana); }

  int getFatigue() const { return fatigue; }
  int getMaxFatigue() const { return max_fatigue; }

 private:
  int fatigue;      // текущая усталость (0..max_fatigue)
  int max_fatigue;  // при достижении — урон падает до минимума

  int mana;
  int max_mana;
};
