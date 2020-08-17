#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <River/Vendor/glm/gtc/matrix_transform.hpp>
#include <River/Vendor/glm/glm.hpp>
#include <River/Vendor/glm/gtc/type_ptr.hpp>

namespace River{
	class Camera{
	private:
		float x, y, z;
		float rotation;
		float zoom;

		// Signals if the position/rotation/zoom has been updated
		bool dirty = true;
		glm::mat4 viewMatrix;

	public:

		void setPosition(float x, float y, float z);
		void setX(float x);
		void setY(float y);
		void setZ(float z);

		void adjustPosition(float x, float y, float z);
		void adjustX(float x);
		void adjustY(float y);
		void adjustZ(float z);

		void setRotation(float rotation);

		void setZoom(float zoom);

		glm::mat4& getViewMatrix();
	};
}


