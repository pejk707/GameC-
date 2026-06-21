#include "UI.h"

#include <sstream>
#include "FontCache.h"

static const float PANEL_X = 810.0f;
static const float PANEL_Y = 10.0f;

UI::UI() { font_loaded = font.openFromFile(getExeDir() + "/arial.ttf"); }

void UI::addLog(const std::string& message, sf::Color color) {
  combat_log.push_back(std::make_pair(message, color));
  if (combat_log.size() > 5)
    combat_log.erase(combat_log.begin());
}

void UI::draw(sf::RenderWindow& window, const Hero& hero,
              const std::vector<std::unique_ptr<Monster>>& monsters) const {
  if (!font_loaded) return;

  sf::RectangleShape panel(sf::Vector2f(185, 580));
  panel.setPosition({PANEL_X - 5, 5.f});
  panel.setFillColor(sf::Color(25, 20, 15));
  panel.setOutlineColor(sf::Color(100, 80, 50));
  panel.setOutlineThickness(2);
  window.draw(panel);

  sf::Text title(font, "DUNGEON", 18u);
  title.setFillColor(sf::Color(200, 170, 0));
  title.setPosition({PANEL_X + 30, PANEL_Y});
  window.draw(title);

  float bar_w = 160.0f;
  float hp_ratio = static_cast<float>(hero.getHp()) / hero.getHpCap();
  float mp_ratio = static_cast<float>(hero.getMana()) / hero.getMaxMana();
  float fat_ratio = static_cast<float>(hero.getFatigue()) / hero.getMaxFatigue();

  drawBar(window, PANEL_X, 40, bar_w, 14, hp_ratio, sf::Color(180, 30, 30),
          "HP " + std::to_string(hero.getHp()) + "/" +
              std::to_string(hero.getHpCap()),
          font);
  drawBar(window, PANEL_X, 65, bar_w, 14, mp_ratio, sf::Color(30, 60, 180),
          "MP " + std::to_string(hero.getMana()) + "/" +
              std::to_string(hero.getMaxMana()),
          font);
  drawBar(window, PANEL_X, 90, bar_w, 14, fat_ratio, sf::Color(160, 100, 0),
          "Fatigue " + std::to_string(hero.getFatigue()) + "/" +
              std::to_string(hero.getMaxFatigue()),
          font);

  sf::Text atk_text(font, "ATK: " + std::to_string(hero.getEffectiveAttack()), 13u);
  atk_text.setFillColor(sf::Color(220, 220, 100));
  atk_text.setPosition({PANEL_X, 115.f});
  window.draw(atk_text);

  sf::Text rest_hint(font, "Rest x2: -10MP +8HP", 11u);
  rest_hint.setFillColor(sf::Color(100, 180, 100));
  rest_hint.setPosition({PANEL_X, 132.f});
  window.draw(rest_hint);

  int alive_count = 0;
  for (const auto& m : monsters) if (m->isAlive()) ++alive_count;

  sf::Text monsters_title(font, "Monsters: " + std::to_string(alive_count), 13u);
  monsters_title.setFillColor(sf::Color(200, 100, 100));
  monsters_title.setPosition({PANEL_X, 175.f});
  window.draw(monsters_title);

  float my = 193.0f;
  for (const auto& m : monsters) {
    bool alive = m->isAlive();
    sf::Color name_color = alive ? sf::Color(220, 160, 160) : sf::Color(90, 90, 90);
    sf::Color stat_color = alive ? sf::Color(180, 180, 180) : sf::Color(70, 70, 70);

    std::string prefix = alive ? m->getName() : "[x] " + m->getName();
    sf::Text name_text(font, prefix, 11u);
    name_text.setFillColor(name_color);
    name_text.setPosition({PANEL_X, my});
    window.draw(name_text);

    std::string stats = "  HP:" + std::to_string(m->getHp()) +
                        "/" + std::to_string(m->getMaxHp()) +
                        " ATK:" + std::to_string(m->getAttack());
    sf::Text stat_text(font, stats, 11u);
    stat_text.setFillColor(stat_color);
    stat_text.setPosition({PANEL_X, my + 13.f});
    window.draw(stat_text);

    my += 30.0f;
  }

  sf::Text controls(font, "WASD-move  Space-rest\nClick chest to open", 11u);
  controls.setFillColor(sf::Color(140, 140, 140));
  controls.setPosition({PANEL_X, my + 4.f});
  window.draw(controls);

  float log_y = my + 40.0f;
  sf::Text log_title(font, "-- Combat Log --", 12u);
  log_title.setFillColor(sf::Color(150, 150, 100));
  log_title.setPosition({PANEL_X, log_y});
  window.draw(log_title);

  log_y += 16.0f;
  for (const auto& entry : combat_log) {
    std::string line = entry.first;
    sf::Color entry_color = entry.second;
    while (!line.empty()) {
      std::string part = line.substr(0, 22);
      line = line.size() > 22 ? line.substr(22) : "";
      sf::Text log_text(font, part, 11u);
      log_text.setFillColor(entry_color);
      log_text.setPosition({PANEL_X, log_y});
      window.draw(log_text);
      log_y += 13;
    }
  }
}

void UI::drawBar(sf::RenderWindow& window, float x, float y, float width,
                 float height, float fill_ratio, sf::Color color,
                 const std::string& label, const sf::Font& fnt) const {
  sf::RectangleShape bg(sf::Vector2f(width, height));
  bg.setPosition({x, y});
  bg.setFillColor(sf::Color(50, 50, 50));
  window.draw(bg);

  sf::RectangleShape fill(sf::Vector2f(width * fill_ratio, height));
  fill.setPosition({x, y});
  fill.setFillColor(color);
  window.draw(fill);

  sf::Text text(fnt, label, 11u);
  text.setFillColor(sf::Color::White);
  text.setPosition({x + 2, y});
  window.draw(text);
}

void UI::drawGameOver(sf::RenderWindow& window, bool victory) const {
  if (!font_loaded) return;

  sf::RectangleShape overlay(sf::Vector2f(1000, 600));
  overlay.setFillColor(sf::Color(0, 0, 0, 200));
  window.draw(overlay);

  std::string msg = victory ? "YOU WIN!" : "YOU DIED...";
  sf::Color color = victory ? sf::Color(0, 220, 0) : sf::Color(220, 0, 0);

  sf::Text text(font, msg, 48u);
  text.setFillColor(color);
  text.setPosition({250.f, 250.f});
  window.draw(text);

  sf::Text sub(font, "Press R to restart", 20u);
  sub.setFillColor(sf::Color::White);
  sub.setPosition({280.f, 320.f});
  window.draw(sub);
}

void UI::drawRageZoneHint(sf::RenderWindow& window) const {
  if (!font_loaded) return;
  sf::Text hint(font, "Red tiles = Danger Zones (-3 HP/turn)", 12u);
  hint.setFillColor(sf::Color(200, 80, 80));
  hint.setPosition({5.f, 582.f});
  window.draw(hint);
}
