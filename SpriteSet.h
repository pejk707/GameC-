#pragma once
#include <SFML/Graphics.hpp>

// Хранит спрайты для всех игровых сущностей.
// Автоматически генерирует пиксель-арт и сохраняет PNG в папку sprites/,
// где их можно отредактировать вручную.
class SpriteSet {
 public:
  SpriteSet();

  const sf::Texture& hero()   const { return heroTex; }
  const sf::Texture& goblin() const { return goblinTex; }
  const sf::Texture& troll()  const { return trollTex; }
  const sf::Texture& chest()  const { return chestTex; }

 private:
  sf::Texture heroTex;
  sf::Texture goblinTex;
  sf::Texture trollTex;
  sf::Texture chestTex;
};
