#include "event_manager.h"

namespace Engine {
	EventManager::EventManager() {}

	void EventManager::addEvent(EventType eventType, const EventCallback& callback) {
		_mEventQueue.push(callback);
		_mRedoQueue = {};

		if (_mUndoQueue.size() >= _mHistoryDepth) {
			// Remove the oldest event from the undo queue
			_mUndoQueue.pop();
		}

		_mUndoQueue.push(callback);
	}

	void EventManager::executeEvents() {
		while (!_mEventQueue.empty()) {
			EventCallback& event = _mEventQueue.front();
			event();

			if (_mUndoQueue.size() >= _mHistoryDepth) {
				// Remove the oldest event from the undo queue
				_mUndoQueue.pop();
			}

			_mUndoQueue.push(event);
			_mEventQueue.pop();
		}
	}

	void EventManager::undo() {
		if (!_mUndoQueue.empty()) {
			EventCallback& event = _mUndoQueue.top();
			event();

			_mRedoQueue.push(event);
			_mUndoQueue.pop();
		}
	}

	void EventManager::redo() {
		if (!_mRedoQueue.empty()) {
			EventCallback& event = _mRedoQueue.top();
			event();

			_mUndoQueue.push(event);
			_mRedoQueue.pop();
		}
	}
}
