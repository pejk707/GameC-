#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>

// Абстрактный базовый класс для всех игровых объектов (герой, монстры)
class Entity {
 public:
  Entity(int x, int y, int max_hp, int attack, int hp_cap = 9999)
      : x(x), y(y), max_hp(max_hp), hp(max_hp), attack(attack), hp_cap(hp_cap) {}

  virtual ~Entity() = default;

  // Чисто виртуальные функции — обязательны к переопределению
  virtual void draw(sf::RenderWindow& window, int tile_size) const = 0;
  virtual std::string getName() const = 0;

  // Виртуальные функции с базовой реализацией
  virtual void takeDamage(int damage) { hp -= damage; }
  virtual bool isAlive() const { return hp > 0; }

  int getX() const { return x; }
  int getY() const { return y; }
  void setX(int new_x) { x = new_x; }
  void setY(int new_y) { y = new_y; }

  int getHp() const { return hp; }
  int getMaxHp() const { return max_hp; }
  int getHpCap() const { return hp_cap; }
  int getAttack() const { return attack; }

  void heal(int amount) { hp = std::min(hp + amount, max_hp); }
  void addMaxHp(int amount) {
    max_hp = std::min(max_hp + amount, hp_cap);
    hp = std::min(hp + amount, max_hp);
  }
  void addAttack(int amount) { attack += amount; }

 protected:
  int x;
  int y;
  int max_hp;
  int hp;
  int attack;
  int hp_cap;
};
