#include "TileSet.h"

#include <cmath>

TileSet::TileSet() {
  if (!floorTex.loadFromFile("tiles/floor.png")) {
    floorTex.loadFromImage(makeFloorImage());
    floorTex.setSmooth(false);
  }
  if (!wallTex.loadFromFile("tiles/wall.png")) {
    wallTex.loadFromImage(makeWallImage());
    wallTex.setSmooth(false);
  }
  if (!rageZoneTex.loadFromFile("tiles/rage_zone.png")) {
    rageZoneTex.loadFromImage(makeRageZoneImage());
    rageZoneTex.setSmooth(false);
  }
}

// Тёмно-коричневый каменный пол: 4 плитки в сетке 2x2 со швами
sf::Image TileSet::makeFloorImage() {
  const unsigned SZ = 16;
  sf::Image img;
  img.create(SZ, SZ, sf::Color(38, 33, 28));

  const sf::Color mortar(25, 21, 17);
  const sf::Color hi(53, 47, 39);

  // Горизонтальный шов посередине
  for (unsigned x = 0; x < SZ; ++x) img.setPixel(x, 8, mortar);
  // Вертикальные швы со смещением (имитация кирпичной кладки)
  for (unsigned y = 0; y < 8; ++y)  img.setPixel(8, y, mortar);
  for (unsigned y = 8; y < SZ; ++y) img.setPixel(4, y, mortar);
  for (unsigned y = 8; y < SZ; ++y) img.setPixel(12, y, mortar);
  // Внешняя рамка (будет совмещаться с соседними тайлами)
  for (unsigned i = 0; i < SZ; ++i) {
    img.setPixel(i, 0, mortar);
    img.setPixel(0, i, mortar);
  }
  // Блик на верхнем-левом крае каждой плитки
  for (unsigned x = 1; x < 8; ++x)   img.setPixel(x, 1, hi);
  for (unsigned x = 9; x < SZ; ++x)  img.setPixel(x, 1, hi);
  for (unsigned x = 1; x < 4; ++x)   img.setPixel(x, 9, hi);
  for (unsigned x = 5; x < 12; ++x)  img.setPixel(x, 9, hi);
  for (unsigned x = 13; x < SZ; ++x) img.setPixel(x, 9, hi);
  for (unsigned y = 2; y < 8; ++y)   img.setPixel(1, y, hi);
  for (unsigned y = 2; y < 8; ++y)   img.setPixel(9, y, hi);
  for (unsigned y = 10; y < SZ; ++y) img.setPixel(1, y, hi);
  for (unsigned y = 10; y < SZ; ++y) img.setPixel(5, y, hi);
  for (unsigned y = 10; y < SZ; ++y) img.setPixel(13, y, hi);

  return img;
}

// Серая каменная стена: кирпичная кладка с блик/тень на гранях
sf::Image TileSet::makeWallImage() {
  const unsigned SZ = 16;
  sf::Image img;
  img.create(SZ, SZ, sf::Color(76, 74, 72));

  const sf::Color mortar(42, 42, 42);
  const sf::Color hi(108, 105, 102);
  const sf::Color shadow(52, 50, 48);

  // Горизонтальные швы кирпичей
  for (unsigned x = 0; x < SZ; ++x) {
    img.setPixel(x, 0, mortar);
    img.setPixel(x, 8, mortar);
    img.setPixel(x, 15, mortar);
  }
  // Вертикальные швы (кирпичная кладка в перевязку)
  for (unsigned y = 0; y < 8; ++y)  img.setPixel(8, y, mortar);
  for (unsigned y = 8; y < SZ; ++y) img.setPixel(4, y, mortar);
  for (unsigned y = 8; y < SZ; ++y) img.setPixel(12, y, mortar);
  for (unsigned y = 0; y < SZ; ++y) img.setPixel(0, y, mortar);
  for (unsigned y = 0; y < SZ; ++y) img.setPixel(15, y, mortar);

  // Блик (верхний и левый край каждого кирпича)
  for (unsigned x = 1; x < 8; ++x)   img.setPixel(x, 1, hi);
  for (unsigned x = 9; x < 15; ++x)  img.setPixel(x, 1, hi);
  for (unsigned x = 1; x < 4; ++x)   img.setPixel(x, 9, hi);
  for (unsigned x = 5; x < 12; ++x)  img.setPixel(x, 9, hi);
  for (unsigned x = 13; x < 15; ++x) img.setPixel(x, 9, hi);
  for (unsigned y = 2; y < 8; ++y)   img.setPixel(1, y, hi);
  for (unsigned y = 2; y < 8; ++y)   img.setPixel(9, y, hi);
  for (unsigned y = 10; y < 15; ++y) img.setPixel(1, y, hi);
  for (unsigned y = 10; y < 15; ++y) img.setPixel(5, y, hi);
  for (unsigned y = 10; y < 15; ++y) img.setPixel(13, y, hi);

  // Тень (нижний и правый край каждого кирпича)
  for (unsigned x = 1; x < 8; ++x)   img.setPixel(x, 7, shadow);
  for (unsigned x = 9; x < 15; ++x)  img.setPixel(x, 7, shadow);
  for (unsigned y = 1; y < 8; ++y)   img.setPixel(7, y, shadow);
  for (unsigned y = 1; y < 8; ++y)   img.setPixel(14, y, shadow);
  for (unsigned x = 1; x < 4; ++x)   img.setPixel(x, 14, shadow);
  for (unsigned x = 5; x < 12; ++x)  img.setPixel(x, 14, shadow);
  for (unsigned x = 13; x < 15; ++x) img.setPixel(x, 14, shadow);
  for (unsigned y = 9; y < 15; ++y)   img.setPixel(3, y, shadow);
  for (unsigned y = 9; y < 15; ++y)   img.setPixel(11, y, shadow);

  return img;
}

// Зона ярости: тёмно-красный с радиальным свечением в центре
sf::Image TileSet::makeRageZoneImage() {
  const unsigned SZ = 16;
  sf::Image img;
  img.create(SZ, SZ, sf::Color(90, 15, 15));

  for (unsigned y = 0; y < SZ; ++y) {
    for (unsigned x = 0; x < SZ; ++x) {
      float dx   = static_cast<float>(x) - 7.5f;
      float dy   = static_cast<float>(y) - 7.5f;
      float dist = std::sqrt(dx * dx + dy * dy);
      if (dist < 5.5f) {
        float t  = 1.f - dist / 5.5f;
        auto  r  = static_cast<sf::Uint8>(90  + static_cast<int>(90  * t));
        auto  g  = static_cast<sf::Uint8>(15  + static_cast<int>(25  * t));
        img.setPixel(x, y, sf::Color(r, g, 15));
      }
    }
  }

  // Тёмная рамка по краю
  for (unsigned i = 0; i < SZ; ++i) {
    img.setPixel(i,      0,      sf::Color(55, 8, 8));
    img.setPixel(i,      SZ - 1, sf::Color(55, 8, 8));
    img.setPixel(0,      i,      sf::Color(55, 8, 8));
    img.setPixel(SZ - 1, i,      sf::Color(55, 8, 8));
  }

  return img;
}
