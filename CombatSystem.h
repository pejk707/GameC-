#pragma once
#include <string>

#include "Hero.h"
#include "Monster.h"

// CombatSystem отвечает только за расчёт боя (принцип Single Responsibility)
// Бой автоматический: герой и монстр обмениваются ударами до смерти одного
class CombatSystem {
 public:
  // Проводит один раунд боя (один удар туда-обратно)
  // Возвращает описание произошедшего для лога
  static std::string doRound(Hero& hero, Monster& monster);

  // Проверяет, находится ли монстр рядом с героем (по соседним клеткам)
  static bool isAdjacent(const Hero& hero, const Monster& monster);
};
