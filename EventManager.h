#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// Паттерн Observer: EventManager хранит подписчиков на события
// Использование: subscribe("monster_died", callback) / notify("monster_died")
class EventManager {
 public:
  using Callback = std::function<void(const std::string&)>;

  // Singleton — единственный экземпляр на всю игру
  static EventManager& getInstance() {
    static EventManager instance;
    return instance;
  }

  // Подписаться на событие
  void subscribe(const std::string& event, Callback callback) {
    listeners[event].push_back(callback);
  }

  // Уведомить всех подписчиков о событии с данными
  void notify(const std::string& event, const std::string& data = "") {
    if (listeners.count(event)) {
      for (auto& cb : listeners[event]) {
        cb(data);
      }
    }
  }

 private:
  EventManager() = default;
  std::unordered_map<std::string, std::vector<Callback>> listeners;
};
