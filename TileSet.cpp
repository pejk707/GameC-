#include "TileSet.h"

#include <cstdint>
#include <filesystem>

namespace {

// Build a 16x16 sf::Image from a pixel-index array + colour palette.
template<std::size_t N>
sf::Image fromData(const uint8_t pixels[16][16], const sf::Color (&pal)[N]) {
    sf::Image img(sf::Vector2u{16u, 16u});
    for (unsigned y = 0; y < 16; ++y)
        for (unsigned x = 0; x < 16; ++x)
            img.setPixel({x, y}, pal[pixels[y][x]]);
    return img;
}

// ─── Stone Floor ─────────────────────────────────────────────────────────────
// 0=mortar  1=base  2=mid  3=highlight  4=shadow
const sf::Color FC[] = {
    {18, 14, 11},  // 0 mortar
    {52, 44, 37},  // 1 base stone
    {65, 56, 48},  // 2 mid stone
    {82, 72, 60},  // 3 highlight
    {33, 27, 22},  // 4 shadow
};
const uint8_t FP[16][16] = {
    {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    {0,3,2,1,1,2,1,1, 0,3,2,1,1,2,1,1},
    {0,3,1,1,1,1,1,1, 0,3,1,1,1,1,1,1},
    {0,1,1,1,2,1,1,1, 0,1,1,1,2,1,1,1},
    {0,1,1,1,1,1,1,1, 0,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,4,4, 0,1,1,1,1,1,4,4},
    {0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4},
    {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    {4,4,4,0,3,2,1,1, 1,1,2,1,0,3,2,4},
    {1,1,1,0,3,1,1,1, 1,1,1,1,0,3,1,1},
    {1,1,1,0,1,1,1,2, 1,1,1,1,0,1,1,1},
    {1,1,1,0,1,1,1,1, 1,1,1,1,0,1,1,1},
    {1,1,1,0,1,1,1,1, 1,4,4,1,0,1,4,1},
    {4,4,4,0,4,4,4,4, 4,4,4,4,0,4,4,4},
    {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
};

// ─── Stone Wall ──────────────────────────────────────────────────────────────
// 0=mortar  1=face  2=shadow-face  3=highlight  4=deep-shadow  5=light-face
const sf::Color WC[] = {
    { 22, 22, 22},  // 0 mortar
    { 82, 79, 75},  // 1 face
    { 70, 68, 65},  // 2 shadow face
    {120,116,110},  // 3 highlight
    { 50, 48, 47},  // 4 deep shadow
    {100, 97, 93},  // 5 light face
};
const uint8_t WP[16][16] = {
    {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    {0,3,5,1,1,5,1,1, 0,3,5,1,1,5,1,1},
    {0,3,1,1,1,1,1,1, 0,3,1,1,1,1,1,1},
    {0,1,1,1,5,1,1,1, 0,1,1,1,5,1,1,1},
    {0,1,1,1,1,1,1,1, 0,1,1,1,1,1,1,1},
    {0,2,2,2,2,2,4,4, 0,2,2,2,2,2,4,4},
    {0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4},
    {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    {4,4,4,0,3,5,1,1, 1,1,5,1,0,3,5,4},
    {1,1,1,0,3,1,1,1, 1,1,1,1,0,3,1,1},
    {2,2,2,0,1,1,1,5, 1,1,1,1,0,1,1,2},
    {4,4,4,0,2,2,2,2, 2,2,2,2,0,2,4,4},
    {0,0,0,0,4,4,4,4, 4,4,4,4,0,4,4,0},
    {0,3,5,0,0,3,5,1, 1,5,1,0,0,3,5,1},
    {0,3,1,0,0,3,1,1, 1,1,1,0,0,3,1,1},
    {0,4,4,0,0,4,4,4, 4,4,4,0,0,4,4,4},
};

// ─── Rage Zone ───────────────────────────────────────────────────────────────
// 0=void  1=dark-red  2=mid-red  3=crack-glow  4=glow  5=hot  6=center
const sf::Color RC[] = {
    {  8,  2,  2},  // 0 near-black void
    { 35,  6,  6},  // 1 dark red tile
    { 75, 12,  8},  // 2 mid red (unused here, available for edits)
    {165, 35, 10},  // 3 crack outer glow
    {220, 75, 18},  // 4 glow ring
    {255,130, 30},  // 5 hot orange
    {255,200,120},  // 6 bright center
};
const uint8_t RP[16][16] = {
    {0,0,0,0,0,1,1,0, 0,1,1,0,0,0,0,0},
    {0,0,0,1,1,1,0,0, 0,0,1,1,1,0,0,0},
    {0,0,1,1,0,0,0,0, 0,0,0,0,1,1,0,0},
    {0,1,1,0,0,0,3,3, 3,3,0,0,0,1,1,0},
    {0,1,0,0,0,3,4,4, 4,4,3,0,0,0,1,0},
    {1,1,0,0,3,4,5,5, 5,5,4,3,0,0,1,1},
    {1,0,0,3,4,5,6,5, 5,6,5,4,3,0,0,1},
    {0,0,3,4,5,5,5,6, 6,5,5,5,4,3,0,0},
    {0,0,3,4,5,5,6,5, 5,6,5,5,4,3,0,0},
    {1,1,0,3,4,5,5,5, 5,5,5,4,3,0,1,1},
    {1,0,0,0,3,4,4,4, 4,4,4,3,0,0,0,1},
    {0,1,0,0,0,3,3,3, 3,3,3,0,0,0,1,0},
    {0,1,1,0,0,0,0,0, 0,0,0,0,0,1,1,0},
    {0,0,1,1,0,0,0,0, 0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0, 0,0,1,1,0,0,0,0},
    {0,0,0,0,1,1,0,0, 0,1,1,0,0,0,0,0},
};

} // namespace

TileSet::TileSet() {
    // If a PNG is found in tiles/ — use it (lets the user replace sprites).
    // Otherwise use the pixel-art fallback and save it to tiles/ so the user
    // has a starting point to edit.
    auto load = [](sf::Texture& tex, const char* path, sf::Image fallback) {
        if (!tex.loadFromFile(path)) {
            [[maybe_unused]] bool ok = tex.loadFromImage(fallback);
            try {
                std::filesystem::create_directories("tiles");
                [[maybe_unused]] bool saved = fallback.saveToFile(path);
            } catch (...) {}
        }
        tex.setSmooth(false);
    };

    load(floorTex,    "tiles/floor.png",     fromData(FP, FC));
    load(wallTex,     "tiles/wall.png",      fromData(WP, WC));
    load(rageZoneTex, "tiles/rage_zone.png", fromData(RP, RC));
}
