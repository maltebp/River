#include "Resolution.h"

namespace River {


	Resolution::Resolution() { }


	Resolution::Resolution(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}


	double Resolution::getAspectRatio() {
		return (double)width / (double)height;
	}


	bool Resolution::operator==(const Resolution& other) {
		return height == other.height && width == other.width;
	}


	bool Resolution::operator!=(const Resolution& other) {
		return height != other.height || width != other.width;
	}

}