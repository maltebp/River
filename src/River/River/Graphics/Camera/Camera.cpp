#include "River/pch.h"

#include "Camera.h"

#include <iostream>

#include "River/Error.h"

using namespace glm;
using namespace River;


const vec3 DEFAULT_DIRECTION = { 0.0, 0.0, -1.0f };


Camera::Camera(unsigned int viewWidth, unsigned int viewHeight) { 
	setViewSize(viewWidth, viewHeight);
}


void Camera::setViewSize(unsigned int viewWidth, unsigned int viewHeight) {
	if( viewWidth == 0 ) throw InvalidArgumentException("View width must be larger than 0");
	if( viewHeight == 0 ) throw InvalidArgumentException("View width must be larger than 0");
	_viewWidth = viewWidth;
	_viewHeight = viewHeight;	
}


void Camera::setPosition(float x, float y, float z) {
	_position = { x, y, z };
}


void Camera::setPosition(vec3 position) {
	_position = position;
	std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl; // TODO:
}


void Camera::adjustPosition(vec3 position) {
	_position += position;
}


vec3 Camera::getPosition() const {
	return _position;
}


void Camera::setRotation(quat rotation) {
	_rotation = rotation;
}


void Camera::setRotationToDirection(vec3 direction) {
	direction = normalize(direction);

	vec3 axis = cross(DEFAULT_DIRECTION, direction);
	float w = 1 + dot(DEFAULT_DIRECTION, direction);

	_rotation = normalize(quat(w, axis));
}


void Camera::setRotationToTarget(vec3 target) {
	vec3 targetDirection = target - _position;
	setRotationToDirection(targetDirection);
}


void Camera::adjustRotation(vec3 axis, float angle) {
	_rotation = rotate(_rotation, radians(angle), axis);
}


quat Camera::getRotation() const {
	return _rotation;
}


mat4 Camera::getMatrix() const {

	vec3 target = _rotation * DEFAULT_DIRECTION;

	mat4 viewMatrix = lookAt(_position, _position + target, vec3(0, 1, 0));

	mat4 projectionMatrix = perspectiveFov(
		_fov, (float)_viewWidth, (float)_viewHeight, 0.01f, 10000.0f
	);

	return projectionMatrix * viewMatrix;
}