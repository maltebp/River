#pragma once

#include "River/Event/Event.h"
#include "River/Primitives/Resolution.h"


namespace River {

	class ResolutionEvent : public Event {
	public:

		ResolutionEvent(const Resolution& resolution)
			:	resolution(resolution)
		{}

		Resolution getResolution() {
			return resolution;
		}

	private:

		Resolution resolution;

	};

}