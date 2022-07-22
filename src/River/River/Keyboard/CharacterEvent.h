#pragma once

#include "River/Utility/Event.h"


namespace River {

	class CharacterEvent : public Event {
	public:

		CharacterEvent(char32_t character)
			:	character(character)
		{}

		char32_t getCharacter() {
			return character;
		}


	private:
		
		char32_t character;


	};

}