#pragma once

#include "River/Utility/Listener.h"
#include "KeyEvent.h"
#include "CharacterEvent.h"


namespace River {

	class KeyboardController;

	/**
	 * @brief	Interface for handling inputs from the keyboard,
	 *			including raw key and character events.
	*/
	class Keyboard {
		friend KeyboardController;
	public:
		
		/**
		 * @brief	Listeners that are notified when a key is pushed
		 *			(not held/pressed) down
		*/
		static inline ListenerMap<KeyEvent&> keyDownListeners;

		/**
		 * @brief	Listeners that are notified when a key is released
		*/
		static inline ListenerMap<KeyEvent&> keyUpListeners;

		static inline ListenerMap<CharacterEvent&> characterInputListeners;

		static bool isKeyPressed(Key key, bool consume = false);


	private:

		static inline ListenerInvoker keyDownInvoker = (keyDownListeners);

		static inline ListenerInvoker keyUpInvoker = (keyUpListeners);

		static inline ListenerInvoker characterInputInvoker = (characterInputListeners);

	};

}


