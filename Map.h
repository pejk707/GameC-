#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "TileSet.h"

// Типы тайлов на карте
enum class TileType { FLOOR, WALL, RAGE_ZONE };

// Карта подземелья
// Содержит большую систему "Зоны ярости" (Rage Zones):
//   Проклятые клетки — герой получает +50% атаки, но теряет 3 HP за ход
class Map {
 public:
  static const int WIDTH = 20;
  static const int HEIGHT = 15;

  Map();

  void draw(sf::RenderWindow& window, int tile_size) const;

  TileType getTile(int x, int y) const;
  bool isWalkable(int x, int y) const;
  bool isRageZone(int x, int y) const;

  // Возвращает true если позиция внутри карты
  bool isInBounds(int x, int y) const;

 private:
  void buildMap();

  std::vector<std::vector<TileType>> tiles;
  TileSet tileSet;
};
