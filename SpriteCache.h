#pragma once
#include "SpriteSet.h"

inline const SpriteSet& getSpriteSet() {
    static SpriteSet sprites;
    return sprites;
}
