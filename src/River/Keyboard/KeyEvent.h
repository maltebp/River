#pragma once

#include "River/Primitives/KeyboardKey.h"
#include "River/Utility/Event.h"

namespace River {

	class KeyEvent : public Event {
	public:
		
		KeyEvent(const KeyboardKey key) 
			:	key(key)
		{}


		KeyboardKey getKey() {
			return key;
		}


		/**
		 * @brief	Consumes the event if its key matches the argument
		 * @return	True if the key matches the argument and the event was not already consumed
		*/
		bool consumeKey(KeyboardKey key) {
			if( !consumed && key == this->key ) {
				consume();
				return true;
			}
			return false;
		}


	private:
		
		const KeyboardKey key = KeyboardKey::A;
	
	};

}


