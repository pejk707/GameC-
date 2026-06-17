#include "Map.h"

#include <algorithm>
#include <random>

Map::Map() { buildMap(); }

void Map::buildMap() {
  // Инициализируем карту полом
  tiles.assign(HEIGHT, std::vector<TileType>(WIDTH, TileType::FLOOR));

  // Внешние стены
  for (int x = 0; x < WIDTH; x++) {
    tiles[0][x] = TileType::WALL;
    tiles[HEIGHT - 1][x] = TileType::WALL;
  }
  for (int y = 0; y < HEIGHT; y++) {
    tiles[y][0] = TileType::WALL;
    tiles[y][WIDTH - 1] = TileType::WALL;
  }

  // Внутренние стены — создаём комнаты
  // Горизонтальная стена посередине с проходом
  for (int x = 1; x < WIDTH - 1; x++) {
    if (x != 5 && x != 14) {
      tiles[7][x] = TileType::WALL;
    }
  }

  // Вертикальная стена слева с проходом
  for (int y = 1; y < 7; y++) {
    if (y != 3) {
      tiles[y][8] = TileType::WALL;
    }
  }

  // Вертикальная стена справа с проходом
  for (int y = 8; y < HEIGHT - 1; y++) {
    if (y != 11) {
      tiles[y][12] = TileType::WALL;
    }
  }

  // Зоны ярости — ровно 1/4 всей карты (75 тайлов)
  std::mt19937 rng(std::random_device{}());
  auto randInt = [&](int lo, int hi) {
    return std::uniform_int_distribution<int>(lo, hi)(rng);
  };

  const int target = (WIDTH * HEIGHT) / 4;
  int placed = 0;
  while (placed < target) {
    int x = randInt(1, WIDTH - 2);
    int y = randInt(1, HEIGHT - 2);
    // Держим безопасную зону у стартовой позиции героя (1,1)
    if (std::abs(x - 1) + std::abs(y - 1) <= 3) continue;
    if (tiles[y][x] == TileType::FLOOR) {
      tiles[y][x] = TileType::RAGE_ZONE;
      ++placed;
    }
  }
}

void Map::draw(sf::RenderWindow& window, int tile_size) const {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      sf::RectangleShape rect(sf::Vector2f(tile_size, tile_size));
      rect.setPosition(x * tile_size, y * tile_size);

      switch (tiles[y][x]) {
        case TileType::WALL:
          rect.setFillColor(sf::Color(80, 80, 80));
          break;
        case TileType::RAGE_ZONE:
          // Зона ярости — тёмно-красный пол
          rect.setFillColor(sf::Color(100, 20, 20));
          break;
        case TileType::FLOOR:
        default:
          rect.setFillColor(sf::Color(40, 35, 30));
          break;
      }

      rect.setOutlineColor(sf::Color(20, 20, 20));
      rect.setOutlineThickness(1);
      window.draw(rect);
    }
  }
}

TileType Map::getTile(int x, int y) const {
  if (!isInBounds(x, y)) return TileType::WALL;
  return tiles[y][x];
}

bool Map::isWalkable(int x, int y) const {
  if (!isInBounds(x, y)) return false;
  return tiles[y][x] != TileType::WALL;
}

bool Map::isRageZone(int x, int y) const {
  if (!isInBounds(x, y)) return false;
  return tiles[y][x] == TileType::RAGE_ZONE;
}

bool Map::isInBounds(int x, int y) const {
  return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}
