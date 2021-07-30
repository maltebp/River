#pragma once

#include <map>

#include "River/Utility/Listener.h"

namespace River{

	class Event {
	public:
		
		void consume() {
			consumed = true;
		}

		bool isConsumed() const {
			return consumed;
		}


	protected:

		Event() {}


	protected:

		bool consumed = false;


	};

}


