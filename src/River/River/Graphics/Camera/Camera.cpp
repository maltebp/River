#include "River/pch.h"

#include "Camera.h"

#include <iostream>

#include "River/Error.h"

using namespace glm;
using namespace River;


Camera::Camera(unsigned int viewWidth, unsigned int viewHeight) { 
	setViewSize(viewWidth, viewHeight);
}


void Camera::setViewSize(unsigned int viewWidth, unsigned int viewHeight) {
	if( viewWidth == 0 ) throw InvalidArgumentException("View width must be larger than 0");
	if( viewHeight == 0 ) throw InvalidArgumentException("View width must be larger than 0");
	this->viewWidth = viewWidth;
	this->viewHeight = viewHeight;	
}


void Camera::setFov(float fov) {
	if( fov <= 0 ) throw InvalidArgumentException("FOV must be larger than 0");
	if( fov >= 180 ) throw InvalidArgumentException("FOV must be less than 180");
	this->fov = fov;
}


float Camera::getFov() const {
	return fov;
}


void Camera::setRotationToDirection(vec3 direction) {
	direction = normalize(direction);

	vec3 axis = cross(DEFAULT_VIEW_DIRECTION, direction);
	float w = 1 + dot(DEFAULT_VIEW_DIRECTION, direction);

	rotation = normalize(quat(w, axis));
}


vec3 Camera::getViewDirection() const {
	return rotation * DEFAULT_VIEW_DIRECTION;
}


vec3 Camera::getUpDirection() const {
	return rotation * DEFAULT_UP_DIRECTION;
}


vec3 Camera::getRightDirection() const {
	return rotation * DEFAULT_RIGHT_DIRECTION;
}


mat4 Camera::getMatrix() const {
	vec3 rotatedUpDirection = rotation * DEFAULT_UP_DIRECTION;

	vec3 relativePosition = rotation * (distance * -DEFAULT_VIEW_DIRECTION);

	vec3 position = target + relativePosition;

	mat4 viewMatrix = lookAt(position, target, rotatedUpDirection);

	mat4 projectionMatrix = perspectiveFov(
		fov, (float)viewWidth, (float)viewHeight, 0.01f, 10000.0f
	);

	return projectionMatrix * viewMatrix;
}