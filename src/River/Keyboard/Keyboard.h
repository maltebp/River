#pragma once

#include "River/Utility/Listener.h"
#include "KeyEvent.h"
#include "CharacterEvent.h"


namespace River {

	class KeyboardController;

	class Keyboard {
		friend KeyboardController;
	public:
		
		static inline ListenerMap<KeyEvent&> keyDownListeners;

		static inline ListenerMap<KeyEvent&> keyUpListeners;

		static inline ListenerMap<CharacterEvent&> characterInputListeners;

		static bool isKeyPressed(Key key, bool consume = false);


	private:

		static inline ListenerInvoker keyDownInvoker = (keyDownListeners);

		static inline ListenerInvoker keyUpInvoker = (keyUpListeners);

		static inline ListenerInvoker characterInputInvoker = (characterInputListeners);

	};

}


