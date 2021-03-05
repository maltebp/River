// TODO: DELETE THIS
//#pragma once
//
//#include "Layer.h"
//
//#include "River/Error.h"
//
//
//namespace River {
//
//	/**
//	 * @brief	A simple implementation of a Layer, exposes its Action-methods to be implemented
//	 *			as std::functions.
//	*/
//	class SimpleLayer : public Layer {
//	public:
//	
//		void onCreate(std::function<void()> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onCreateAction = action;
//		}
//
//		void onUpdate(std::function<void()> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onUpdateAction = action;
//		}
//
//		void onDestroy(std::function<void()> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onDestroyAction = action;
//		}
//
//		void onKeyEvent(std::function<void(KeyEvent&)> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onKeyEventAction = action;
//		}
//
//		void onMouseMoveEvent(std::function<void(MouseMoveEvent&)> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onMouseMoveEventAction = action;
//		}
//
//		void onMouseScrollEvent(std::function<void(MouseScrollEvent&)> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onMouseScrollEventAction = action;
//		}
//
//		void onMouseButtonEvent(std::function<void(MouseButtonEvent&)> action) {
//			if( action == nullptr ) throw InvalidArgumentException("Action must not be nullptr");
//			onMouseButtonEventAction = action;
//		}
//	
//	protected:
//		virtual void onCreate(){ onCreateAction(); }
//		virtual void onUpdate(){ onUpdateAction(); }
//		virtual void onDestroy(){ onDestroyAction(); }
//
//		virtual void onKeyEvent(KeyEvent& e) {onKeyEventAction(e);}
//		virtual void onMouseMoveEvent(MouseMoveEvent& e) {onMouseMoveEventAction(e);}
//		virtual void onMouseScrollEvent(MouseScrollEvent& e) {onMouseScrollEventAction(e);}
//		virtual void onMouseButtonEvent(MouseButtonEvent& e) {onMouseButtonEventAction(e); }
//
//	private:
//		std::function<void()> onCreateAction = [](){};
//		std::function<void()> onUpdateAction = [](){};
//		std::function<void()> onDestroyAction = [](){};
//		
//		std::function<void(KeyEvent&)> onKeyEventAction = [](auto e){};
//		std::function<void(MouseMoveEvent&)> onMouseMoveEventAction = [](auto e){};
//		std::function<void(MouseScrollEvent&)> onMouseScrollEventAction = [](auto e){};
//		std::function<void(MouseButtonEvent&)> onMouseButtonEventAction = [](auto e){};
//
//	};
//}
//
//
