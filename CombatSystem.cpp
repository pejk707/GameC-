#include "CombatSystem.h"

#include <cmath>
#include <sstream>

std::string CombatSystem::doRound(Hero& hero, Monster& monster) {
  std::ostringstream log;

  int hero_dmg = hero.getEffectiveAttack();

  // Урон монстра — тролль может бить усиленно
  int monster_dmg = monster.getAttack();
  Troll* troll = dynamic_cast<Troll*>(&monster);
  if (troll != nullptr) {
    monster_dmg = troll->getAttackDamage();
  }

  // Гоблин при низком HP пропускает атаку (убегает)
  Goblin* goblin = dynamic_cast<Goblin*>(&monster);
  bool goblin_runs = (goblin != nullptr && goblin->isRunning());

  // Применяем урон
  monster.takeDamage(hero_dmg);
  log << "Hero hits " << monster.getName() << " for " << hero_dmg << " dmg. ";

  if (monster.isAlive() && !goblin_runs) {
    hero.takeDamage(monster_dmg);
    log << monster.getName() << " hits back for " << monster_dmg << " dmg.";
  } else if (goblin_runs) {
    log << "Goblin tries to flee! Skips attack.";
  }

  return log.str();
}

bool CombatSystem::isAdjacent(const Hero& hero, const Monster& monster) {
  int dx = std::abs(hero.getX() - monster.getX());
  int dy = std::abs(hero.getY() - monster.getY());
  // Соседние клетки: разница 1 по X или Y (не по диагонали)
  return (dx + dy) == 1;
}
