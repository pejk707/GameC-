#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <windows.h>

inline std::string getExeDir() {
  wchar_t buf[MAX_PATH];
  GetModuleFileNameW(nullptr, buf, MAX_PATH);
  return std::filesystem::path(buf).parent_path().string();
}

inline const sf::Font* getFont() {
  static sf::Font font;
  static bool loaded = font.openFromFile(getExeDir() + "/arial.ttf");
  return loaded ? &font : nullptr;
}
