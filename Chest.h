#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Тип награды в сундуке
enum class LootType { HEAL, ATTACK_BOOST, MANA };

// Один вариант награды
struct LootOption {
  LootType type;
  std::string label;  // текст для отображения игроку
  int value;
};

// Сундук — стоит на клетке, при открытии предлагает выбор из двух наград
// Механика: игрок кликает мышью на нужную кнопку
class Chest {
 public:
  Chest(int x, int y, LootOption option_a, LootOption option_b);

  void draw(sf::RenderWindow& window, int tile_size) const;

  // Рисует окно выбора награды поверх игры
  void drawLootWindow(sf::RenderWindow& window) const;

  bool isOpen() const { return opened; }
  void open() { opened = true; }

  int getX() const { return x; }
  int getY() const { return y; }

  const LootOption& getOptionA() const { return option_a; }
  const LootOption& getOptionB() const { return option_b; }

  // Проверяет, попал ли клик мыши на кнопку A или B
  // Возвращает 0 = нет выбора, 1 = A, 2 = B
  int checkClick(sf::Vector2i mouse_pos) const;

 private:
  int x;
  int y;
  bool opened;
  LootOption option_a;
  LootOption option_b;

  // Позиции кнопок выбора (в пикселях)
  sf::FloatRect button_a_rect;
  sf::FloatRect button_b_rect;
};
