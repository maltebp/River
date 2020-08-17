#include "Camera.h"


namespace River{


	void Camera::setPosition(float x, float y, float z){
		dirty = true;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Camera::setRotation(float rotation){
		this->rotation = rotation;
		
		// TODO: Adjust rotation to 2 PI interval here
		//if( this->rotation > M_2_PI ) this->rotation = 
	}

	void Camera::setZoom(float zoom){
		this->zoom = zoom;
		if( zoom < 0.01 ) this->zoom = 0.01; 
	}

	glm::mat4& Camera::getViewMatrix(){
		if( dirty ){
			viewMatrix = glm::mat4(1.0f); // Identity matrix (diagonal is 1's)
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-x, -y, 0.0f));
			viewMatrix = glm::rotate(viewMatrix, rotation, glm::vec3(0, 0, 1));
			viewMatrix = glm::scale(viewMatrix, glm::vec3(zoom, zoom, 1));
			dirty = false;
		}
		return viewMatrix;
	}

}