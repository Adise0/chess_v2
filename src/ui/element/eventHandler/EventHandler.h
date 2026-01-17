#include "../event/Event.h"
#include <functional>
#include <vector>

namespace Chess::Rendering::Events {

struct EventListener {
  EventType eventType;
  std::function<void(Event &event)> listener;
};


class EventHandler {

private:
  std::vector<EventListener> eventListeners;


public:
  void On(EventType eventType, std::function<void(Event &event)> listener);
  void HandleEvent(Event &event);
};
} // namespace Chess::Rendering::Events
