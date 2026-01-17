#include "EventHandler.h"


namespace Chess::Rendering::Events {


void EventHandler::On(EventType eventType, std::function<void(Event &event)> listener) {
  // #region On
  eventListeners.push_back({
      eventType,
      listener,
  });
  // #endregion
}

void EventHandler::HandleEvent(Event &event) {
  // #region HandleEvent
  if (event.stopPropagation) return;

  for (short i = 0; i < eventListeners.size(); i++) {
    EventListener &listener = eventListeners[i];
    if (event.eventType != listener.eventType) continue;
    listener.listener(event);
  }
  // #endregion
}
} // namespace Chess::Rendering::Events


