#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "River/External/glm/gtc/matrix_transform.hpp"
#include "River/External/glm/gtc/type_ptr.hpp"
#include "River/External/glm/glm.hpp"


namespace River {


	class Camera {
	public:

		Camera(unsigned int viewWidth, unsigned int viewHeight);

		void setViewSize(unsigned int width, unsigned int height);

		void setRotationToDirection(glm::vec3 direction);

		void setFov(float fov);

		float getFov() const; 

		glm::vec3 getViewDirection() const;

		glm::vec3 getUpDirection() const;

		glm::vec3 getRightDirection() const;

		glm::mat4 getMatrix() const;

		glm::vec3 getPosition() const;

	public:

		glm::vec3 target = { 0, 0, 0 };

		glm::quat rotation = { 1, 0, 0, 0 };

		float distance = 1.0f;

		static inline const glm::vec3 DEFAULT_VIEW_DIRECTION = { 0, 0, -1.0f };
		
		static inline const glm::vec3 DEFAULT_UP_DIRECTION = { 0, 1.0f, 0 };

		static inline const glm::vec3 DEFAULT_RIGHT_DIRECTION = { 1.0f, 0, 0 };

	private:

		unsigned int viewWidth;

		unsigned int viewHeight;

		float fov = 45;

	};

}