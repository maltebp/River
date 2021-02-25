#pragma once

#include "River/Event/Event.h"
#include "River/Primitives/Resolution.h"


namespace River {

	struct ResolutionEvent : Event {
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