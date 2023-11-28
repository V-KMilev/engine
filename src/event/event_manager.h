#pragma once

#include <functional>
#include <queue>
#include <stack>

namespace Engine {

	enum class EventType {
		NONE     = 0,
		KEYBORD  = 1,
		MOUSE    = 2,
		GEOMETRY = 3,
		CAMERA   = 4,
		LIGHT    = 5
	};

	using EventCallback = std::function<void()>;

	class EventManager {
	public:
		EventManager();
		~EventManager() = default;

		EventManager(const EventManager& other) = delete;
		EventManager& operator = (const EventManager& other) = delete;

		EventManager(EventManager && other) = delete;
		EventManager& operator = (EventManager && other) = delete;

		void addEvent(EventType eventType, const EventCallback& callback);

		void executeEvents();

		void undo();
		void redo();

	private:
		std::queue<EventCallback> _mEventQueue;
		std::stack<EventCallback> _mUndoQueue;
		std::stack<EventCallback> _mRedoQueue;

		unsigned int _mHistoryDepth = 20;
	};
}
