#pragma once

namespace Engine {
	class UIElement {
		public:
			UIElement() = default;
			~UIElement() = default;

			UIElement(const UIElement& other) = delete;
			UIElement& operator = (const UIElement& other) = delete;

			UIElement(UIElement && other) = delete;
			UIElement& operator = (UIElement && other) = delete;

			virtual bool drawUI(unsigned int id) = 0;

		public:
			float _mDragSpeed = 1.0f;
	};
}
