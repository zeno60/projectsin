#ifndef IEVENTHANDLER_HEADER
#define IEVENTHANDLER_HEADER

#include "EventDispatcher.h"
#include "Event.h"

class IEventHandler {

public:
  virtual void EventHandler(const Event &e) = 0;

  // Mutator and selector
  IEventHandler * GetNextHandler() {return _nextHandler;}
  void SetNextHandler(IEventHandler *next) { _nextHandler = next; }

  IEventHandler() : _nextHandler(0)
  {
    EventDispatcher::Get()->RegisterHandler(this);
  }

protected:
  void SendEvent(Event &e) 
  {
    EventDispatcher::Get()->SendEvent(e);
  }

private:
  IEventHandler *_nextHandler;
};

#endif