#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "Entity.h"

// Базовый класс монстра
class Monster : public Entity {
 public:
  Monster(int x, int y, int max_hp, int attack)
      : Entity(x, y, max_hp, attack), is_alerted(false) {}

  virtual ~Monster() = default;

  // Переопределяем takeDamage чтобы монстр «злился» при получении урона
  void takeDamage(int damage) override;

  bool isAlerted() const { return is_alerted; }

 protected:
  bool is_alerted;  // монстр заметил героя
};

// ============================================================
// Тип 1: Goblin — слабый, быстрый, атакует каждый ход
// Особенность: если HP < 30% — убегает (пропускает атаку)
// ============================================================
class Goblin : public Monster {
 public:
  Goblin(int x, int y) : Monster(x, y, 30, 8) {}

  void draw(sf::RenderWindow& window, int tile_size) const override;
  std::string getName() const override { return "Goblin"; }

  // Гоблин убегает при низком HP
  bool isRunning() const { return hp < max_hp * 0.3f; }
};

// ============================================================
// Тип 2: Troll — сильный, медленный
// Особенность: каждый 2-й ход делает усиленный удар (x2 урон)
// ============================================================
class Troll : public Monster {
 public:
  Troll(int x, int y) : Monster(x, y, 50, 12), attack_counter(0) {}

  void draw(sf::RenderWindow& window, int tile_size) const override;
  std::string getName() const override { return "Troll"; }

  // Возвращает урон с учётом усиленного удара
  int getAttackDamage();

 private:
  int attack_counter;  // счётчик ходов для усиленного удара
};
