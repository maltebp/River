#include "River/pch.h"

#include "Screen.h"

#include "River/Error.h"
#include "River/Graphics/GL.h"

namespace River {

	
	Resolution Screen::getResolution() {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return Resolution(mode->width, mode->height);
	}


	std::vector<Resolution> Screen::getSupportedResolutions() {
		Resolution currentResolution = getResolution();

		int numVideoModes;
		const GLFWvidmode* videoModes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &numVideoModes);

		std::vector<Resolution> resolutions;

		Resolution previousResolution;
		for( int i = 0; i < numVideoModes; i++ ) {
			const GLFWvidmode* videoMode = videoModes+i;
			Resolution resolution(videoMode->width, videoMode->height);

			if( resolution == previousResolution ) continue;

			resolutions.push_back(resolution);
			previousResolution = resolution;
		}

		return resolutions;
	}


	double Screen::getScaling() {
		float scaleX, scaleY;

		glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &scaleX, &scaleY);

		if( scaleX != scaleY ) {
			throw new Exception("River do not support monitor DPI scaling that is not equal in the x and y axis");
		}

		return scaleX;
	}
	
}