#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Hero.h"
#include "Monster.h"

class UI {
 public:
  UI();

  void draw(sf::RenderWindow& window, const Hero& hero,
            const std::vector<std::unique_ptr<Monster>>& monsters) const;

  // Добавляет сообщение в боевой лог (хранит последние 5)
  void addLog(const std::string& message,
              sf::Color color = sf::Color(180, 180, 180));
  void clearLog() { combat_log.clear(); }

  // Показывает экран победы или поражения
  void drawGameOver(sf::RenderWindow& window, bool victory) const;

  // Подсказка про зону ярости
  void drawRageZoneHint(sf::RenderWindow& window) const;

 private:
  void drawBar(sf::RenderWindow& window, float x, float y, float width,
               float height, float fill_ratio, sf::Color color,
               const std::string& label, const sf::Font& font) const;

  sf::Font font;
  bool font_loaded;
  std::vector<std::pair<std::string, sf::Color>> combat_log;
};
