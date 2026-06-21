#pragma once
#include <SFML/Graphics.hpp>

// Хранит текстуры для каждого типа тайла.
// При запуске пытается загрузить PNG из папки tiles/;
// если файл не найден — генерирует пиксель-арт программно и сохраняет PNG,
// чтобы ты мог его отредактировать.
//   tiles/floor.png, tiles/wall.png, tiles/rage_zone.png
class TileSet {
 public:
  TileSet();

  const sf::Texture& floor()    const { return floorTex; }
  const sf::Texture& wall()     const { return wallTex; }
  const sf::Texture& rageZone() const { return rageZoneTex; }

 private:
  sf::Texture floorTex;
  sf::Texture wallTex;
  sf::Texture rageZoneTex;
};
