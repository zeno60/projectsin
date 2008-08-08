#include "EventDispatcher.h"

#include "IEventHandler.h"

// pointer to singleton instance
EventDispatcher* EventDispatcher::_instanceOf = 0;

EventDispatcher::EventDispatcher() : _deviceList(0)
{
	for(int i = 255; i >= 0; --i)
	{
		keys[i] = false;
	}
}

EventDispatcher::~EventDispatcher()
{
	if(_instanceOf) delete _instanceOf;
}

EventDispatcher* EventDispatcher::Get() 
{
	if (_instanceOf) return _instanceOf;
	return _instanceOf = new EventDispatcher();
}


void EventDispatcher::RegisterHandler(IEventHandler *device)
{
  device->SetNextHandler(_deviceList);
  _deviceList = device;
}


void EventDispatcher::SendEvent(Event &e) 
{
	if(e.Type == E_EVENT_TYPE::E_KEYPRESS) keys[e.KeyInput.KeyCode] = e.KeyInput.PressedDown;
	IEventHandler * curDevice = _deviceList;
  for (;curDevice;curDevice = curDevice->GetNextHandler()) curDevice->EventHandler(e);
}