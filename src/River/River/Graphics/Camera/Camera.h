#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "River/External/glm/gtc/matrix_transform.hpp"
#include "River/External/glm/gtc/type_ptr.hpp"
#include "River/External/glm/glm.hpp"

namespace River{
	class Camera{
	public:

		Camera(unsigned int viewWidth, unsigned int viewHeight);

		void setViewWidth(unsigned int width);
		void setViewHeight(unsigned int height);

		void setPosition(float x, float y, float z);
		void setX(float x);
		void setY(float y);
		void setZ(float z);

		void adjustPosition(float x, float y, float z);
		void adjustX(float x);
		void adjustY(float y);
		void adjustZ(float z);

		float getX() {
			return x;
		}

		float getY() {
			return y;
		}

		float getZ() {
			return z;
		}

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

		float getRotation();

		/**
		 * @brief	Sets the Camera's zoom level
		 * @param zoom	The zoom level in percentage. 1 is no zoom (default), <1 is negative zoom (objects are smaller) and >1 is positive zoom (objects are larger.
		*/
		void setZoom(float zoom);

		/**
		 * @brief	Adds the given value to the Camera's current zoom level 
		 * @param zoom	The amount of zoom to add in percentage points. 1 is no zoom (default), <1 is negative zoom (objects are smaller) and >1 is positive zoom (objects are larger.
		*/
		void adjustZoom(float zoomAdjustment);

		/** 
		 * @return	The matrix to transform objects into the Camera's view (conventionally the 'projectionMatrix * viewMatrix'
		*/
		glm::mat4& getCameraMatrix();

	private:

		unsigned int viewWidth, viewHeight;

		float x = 0, y = 0, z = 0;

		float rotation = 0;

		float zoom = 1;

		bool dirty = true;

		glm::mat4 matrix;

	};

}