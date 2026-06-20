#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Хранит текстуры для каждого типа тайла.
// При запуске пытается загрузить PNG из папки tiles/;
// если файл не найден — генерирует текстуру программно.
// Чтобы подключить свои спрайты, положи рядом с .exe файлы:
//   tiles/floor.png, tiles/wall.png, tiles/rage_zone.png
class TileSet {
 public:
  TileSet();

  const sf::Texture& floor()    const { return floorTex; }
  const sf::Texture& wall()     const { return wallTex; }
  const sf::Texture& rageZone() const { return rageZoneTex; }

 private:
  static sf::Image makeFloorImage();
  static sf::Image makeWallImage();
  static sf::Image makeRageZoneImage();

  sf::Texture floorTex;
  sf::Texture wallTex;
  sf::Texture rageZoneTex;
};
