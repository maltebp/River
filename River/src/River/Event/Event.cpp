#include "Event.h"

namespace River{
	void Event::consume(){
		consumed = true;
	}

	bool Event::isConsumed(){
		return consumed;
	}
}