#pragma once

#include "Key.h"
#include "River/Utility/Event.h"

namespace River {

	class KeyEvent : public Event {
	public:
		
		KeyEvent(const Key key) 
			:	key(key)
		{}


		Key getKey() {
			return key;
		}


		/**
		 * @brief	Consumes the event if its key matches the argument
		 * @return	True if the key matches the argument and the event was not already consumed
		*/
		bool consumeKey(Key key) {
			if( !consumed && key == this->key ) {
				consume();
				return true;
			}
			return false;
		}


	private:
		
		const Key key = Key::A;
	
	};

}


