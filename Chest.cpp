#include "Chest.h"

#include "FontCache.h"
#include "SpriteCache.h"

Chest::Chest(int x, int y, LootOption option_a, LootOption option_b)
    : x(x),
      y(y),
      opened(false),
      option_a(option_a),
      option_b(option_b),
      button_a_rect({200.f, 250.f}, {150.f, 50.f}),
      button_b_rect({450.f, 250.f}, {150.f, 50.f}) {}

void Chest::draw(sf::RenderWindow& window, int tile_size) const {
  if (opened) return;

  const sf::Texture& tex = getSpriteSet().chest();
  sf::Sprite spr(tex);
  auto sz = tex.getSize();
  spr.setScale({(float)tile_size / sz.x, (float)tile_size / sz.y});
  spr.setPosition({(float)(x * tile_size), (float)(y * tile_size)});
  window.draw(spr);
}

void Chest::drawLootWindow(sf::RenderWindow& window) const {
  const sf::Font* font = getFont();

  sf::RectangleShape overlay(sf::Vector2f(1000, 600));
  overlay.setFillColor(sf::Color(0, 0, 0, 160));
  window.draw(overlay);

  sf::RectangleShape panel(sf::Vector2f(400, 200));
  panel.setPosition({200.f, 200.f});
  panel.setFillColor(sf::Color(60, 40, 20));
  panel.setOutlineColor(sf::Color(200, 170, 0));
  panel.setOutlineThickness(3);
  window.draw(panel);

  if (font) {
    sf::Text title(*font, "Chest opened! Choose your reward:", 18u);
    title.setFillColor(sf::Color(200, 170, 0));
    title.setPosition({215.f, 210.f});
    window.draw(title);
  }

  sf::RectangleShape btn_a(sf::Vector2f(150, 50));
  btn_a.setPosition(button_a_rect.position);
  btn_a.setFillColor(sf::Color(40, 100, 40));
  btn_a.setOutlineColor(sf::Color::White);
  btn_a.setOutlineThickness(2);
  window.draw(btn_a);

  if (font) {
    sf::Text text_a(*font, option_a.label, 14u);
    text_a.setFillColor(sf::Color::White);
    text_a.setPosition(button_a_rect.position + sf::Vector2f{10.f, 15.f});
    window.draw(text_a);
  }

  sf::RectangleShape btn_b(sf::Vector2f(150, 50));
  btn_b.setPosition(button_b_rect.position);
  btn_b.setFillColor(sf::Color(100, 40, 40));
  btn_b.setOutlineColor(sf::Color::White);
  btn_b.setOutlineThickness(2);
  window.draw(btn_b);

  if (font) {
    sf::Text text_b(*font, option_b.label, 14u);
    text_b.setFillColor(sf::Color::White);
    text_b.setPosition(button_b_rect.position + sf::Vector2f{10.f, 15.f});
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
