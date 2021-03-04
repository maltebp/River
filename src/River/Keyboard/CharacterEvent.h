#pragma once

#include "River/Event/Event.h"


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