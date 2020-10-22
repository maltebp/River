#include "Camera.h"


namespace River{

	Camera::Camera(unsigned int viewWidth, unsigned int viewHeight) :
		x(0), y(0), z(0), rotation(0), zoom(1), viewWidth(viewWidth), viewHeight(viewHeight),
		projectionMatrix(glm::ortho(((float)viewWidth) / -2.0f, ((float)viewWidth) / 2.0f, ((float)viewHeight) / -2.0f, ((float)viewHeight) / 2.0f, 1.0f, -101.0f))
	{}

	void Camera::setPosition(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
		dirty = true;
	}
		
	void Camera::setX(float x){
		this->x = x;
		dirty = true;
	}

	void Camera::setY(float y){
		this->y = y;
		dirty = true;
	}

	void Camera::setZ(float z){
		this->z = z;
		dirty = true;
	}

	void Camera::adjustPosition(float x, float y, float z){
		this->x += x;
		this->y += y;
		this->z += z;
		dirty = true;
	}

	void Camera::adjustX(float x){
		this->x += x;
		dirty = true;
	}

	void Camera::adjustY(float y){
		this->y += y;
		dirty = true;
	}

	void Camera::adjustZ(float z){
		this->z += z;
		dirty = true;
	}


	void Camera::setRotation(float rotation){
		this->rotation = fmod(rotation, 360.0f);
		dirty = true;
	}

	void Camera::adjustRotation(float rotation){
		this->rotation = fmod(this->rotation + rotation, 360.0f);
		dirty = true;
	}

	void Camera::setZoom(float zoom){
		this->zoom = zoom;
		if( zoom < 0.01f ) this->zoom = 0.01f; 
	}

	void Camera::adjustZoom(float zoomAdjustment){
		this->zoom += zoomAdjustment;
		if( this->zoom < 0.0f ) this->zoom = 0.01f;
	}

	glm::mat4& Camera::getCameraMatrix(){
		if( dirty ){
			cameraMatrix = glm::mat4(1.0f); // Identit	y matrix (diagonal is 1's)
			cameraMatrix = glm::rotate(cameraMatrix, glm::radians(rotation), glm::vec3(0, 0, 1));
			cameraMatrix = glm::translate(cameraMatrix, glm::vec3(-x, -y,  0.0f));
			cameraMatrix = glm::scale(cameraMatrix, glm::vec3(zoom, zoom, 1));
			cameraMatrix = projectionMatrix * cameraMatrix;
			dirty = false;
		}
		return cameraMatrix;
	}

}