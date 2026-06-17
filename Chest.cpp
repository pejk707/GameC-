#include "Chest.h"

#include "FontCache.h"

Chest::Chest(int x, int y, LootOption option_a, LootOption option_b)
    : x(x),
      y(y),
      opened(false),
      option_a(option_a),
      option_b(option_b),
      button_a_rect(200, 250, 150, 50),
      button_b_rect(450, 250, 150, 50) {}

void Chest::draw(sf::RenderWindow& window, int tile_size) const {
  if (opened) return;

  sf::RectangleShape rect(sf::Vector2f(tile_size - 2, tile_size - 2));
  rect.setPosition(x * tile_size + 1, y * tile_size + 1);
  rect.setFillColor(sf::Color(200, 170, 0));
  window.draw(rect);

  if (const sf::Font* font = getFont()) {
    sf::Text text("C", *font, tile_size - 8);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x * tile_size + 5, y * tile_size + 2);
    window.draw(text);
  }
}

void Chest::drawLootWindow(sf::RenderWindow& window) const {
  const sf::Font* font = getFont();

  // Оверлей и кнопки рисуются всегда, текст — только если шрифт доступен
  sf::RectangleShape overlay(sf::Vector2f(1000, 600));
  overlay.setFillColor(sf::Color(0, 0, 0, 160));
  window.draw(overlay);

  sf::RectangleShape panel(sf::Vector2f(400, 200));
  panel.setPosition(200, 200);
  panel.setFillColor(sf::Color(60, 40, 20));
  panel.setOutlineColor(sf::Color(200, 170, 0));
  panel.setOutlineThickness(3);
  window.draw(panel);

  if (font) {
    sf::Text title("Chest opened! Choose your reward:", *font, 18);
    title.setFillColor(sf::Color(200, 170, 0));
    title.setPosition(215, 210);
    window.draw(title);
  }

  sf::RectangleShape btn_a(sf::Vector2f(150, 50));
  btn_a.setPosition(button_a_rect.left, button_a_rect.top);
  btn_a.setFillColor(sf::Color(40, 100, 40));
  btn_a.setOutlineColor(sf::Color::White);
  btn_a.setOutlineThickness(2);
  window.draw(btn_a);

  if (font) {
    sf::Text text_a(option_a.label, *font, 14);
    text_a.setFillColor(sf::Color::White);
    text_a.setPosition(button_a_rect.left + 10, button_a_rect.top + 15);
    window.draw(text_a);
  }

  sf::RectangleShape btn_b(sf::Vector2f(150, 50));
  btn_b.setPosition(button_b_rect.left, button_b_rect.top);
  btn_b.setFillColor(sf::Color(100, 40, 40));
  btn_b.setOutlineColor(sf::Color::White);
  btn_b.setOutlineThickness(2);
  window.draw(btn_b);

  if (font) {
    sf::Text text_b(option_b.label, *font, 14);
    text_b.setFillColor(sf::Color::White);
    text_b.setPosition(button_b_rect.left + 10, button_b_rect.top + 15);
    window.draw(text_b);
  }
}

int Chest::checkClick(sf::Vector2i mouse_pos) const {
  sf::Vector2f pos(static_cast<float>(mouse_pos.x),
                   static_cast<float>(mouse_pos.y));
  if (button_a_rect.contains(pos)) return 1;
  if (button_b_rect.contains(pos)) return 2;
  return 0;
}
