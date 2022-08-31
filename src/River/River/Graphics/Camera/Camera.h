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

		void setPosition(glm::vec3 position);

		void setPosition(float x, float y, float z);

		void adjustPosition(glm::vec3 adjustment);

		glm::vec3 getPosition() const;

		void setRotation(glm::quat rotation);

		void setRotationToDirection(glm::vec3 direction);

		void setRotationToTarget(glm::vec3 target);

		void adjustRotation(glm::vec3 axis, float angle);

		glm::quat getRotation() const;

		// TODO: Get euler rotation

		glm::vec3 getDirection() const;

		void setX(float x);

		void setY(float y);

		void setZ(float z);

		void adjustPosition(float x, float y, float z);

		/**
		 * @brief	Set the cameras rotation to the given value
					The rotation is automatically adjusted to a 0-360 interval

		 * @param rotation	Rotation in degrees
		*/
		void setRotation(float rotation);

		/**
		 * @brief	Adds the given value to the Camera's current rotation.
					The rotation is automatically adjusted to a 0-360 interval

		 * @param rotation	Amount to rotate in degrees (clockwise)
		*/
		void adjustRotation(float rotationAdjustment);

		glm::mat4 getMatrix() const;

	private:

		unsigned int _viewWidth;

		unsigned int _viewHeight;

		glm::vec3 _position = { 0, 0, 0 };

		glm::quat _rotation = { 1, 0, 0, 0 };

		float _fov = 90;

	};

}