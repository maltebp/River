#pragma once

namespace River{

	class Event {

	protected:
		bool consumed = false;

	protected:
		Event(){}

	public:
		void consume();
		bool isConsumed();

	};
}


