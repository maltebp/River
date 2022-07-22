#pragma once

#include "River/Utility/Listener.h"
#include "KeyEvent.h"
#include "CharacterEvent.h"


namespace River {

	class KeyboardController;

	/**
	 * @brief	Interface for handling inputs from the keyboard,
	 *			including raw key events and character events.
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

		/**
		 * @brief	Listeners that are notified when a character input
		 *			occurs. This takes into account modifiers, keyboard
		 *			layout.
		 *			A character input will be fired along side a key down
		 *			event
		*/
		static inline ListenerMap<CharacterEvent&> characterInputListeners;

		/**
		 * @brief	Tests if the given key was pressed (being held down) in
		 *			this event cycle. A key will be not registered as pressed
		 *			in the same cycle as it was pushed down, nor released.
		 * 
		 * @param consume	Whether or not to consume the key press. If true,
		 *					consecutive calls to this function with the same
		 *					KeyboardKey will return false.
		*/
		static bool isKeyPressed(KeyboardKey key, bool consume = false);


	private:

		static inline ListenerInvoker keyDownInvoker = (keyDownListeners);

		static inline ListenerInvoker keyUpInvoker = (keyUpListeners);

		static inline ListenerInvoker characterInputInvoker = (characterInputListeners);

	};

}


