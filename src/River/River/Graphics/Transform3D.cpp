#include "River/pch.h"

#include "Transform3D.h"

#include "River/External/glm/gtc/matrix_transform.hpp"

#include "River/Error.h"


using namespace glm;
using namespace River;


void Transform3D::setPosition(vec3 position) {
    this->position = position;
    updateMatrix();   
}


vec3 Transform3D::getLocalPosition() const {
    return position;
}


vec3 Transform3D::getWorldPosition() const {
    return vec4(matrix * vec4(position, 1.0f));
}


void Transform3D::setScale(vec3 scale) {
    this->scale = scale;
    updateMatrix();
}


vec3 Transform3D::getLocalScale() const {
    return scale;
}


void Transform3D::setRotation(quat rotation) {
    this->rotation = rotation;
    updateMatrix();
}


quat Transform3D::getLocalRotation() const {
    return rotation;
}


void Transform3D::setParent(ECS::Entity* newParent) {
    // TODO: This needs rework (ASAP), once entity system supports finding entity from component

    if( parent != nullptr ) {
        Transform3D* parentTransform = parent->getComponent<Transform3D>();
        auto isThisEntity = [this](ECS::Entity* entity) { 
            return entity->getComponent<Transform3D>()->getId() == getId();
        };


        for (auto iterator = parentTransform->children.begin(); iterator != parentTransform->children.end(); iterator++) {
            if( (*iterator)->getComponent<Transform3D>()->getId() == getId() ) {
                parentTransform->children.erase(iterator);
            }
        }
        
        parent = nullptr;
    }

    if( newParent != nullptr ) {
        Transform3D* newParentTransform = newParent->getComponent<Transform3D>();
        ECS::Domain& domain = newParent->getDomain();
        ECS::Entity* thisEntity = nullptr;
        domain.forMatchingEntities<Transform3D>([this, &thisEntity](ECS::Entity* entity, Transform3D* transform){
            if( transform->getId() == getId() ) {
                thisEntity = entity;
            }
        });

        if( thisEntity == nullptr ) {
            throw Exception("Couldn't find entity for Transform3D");
        }

        newParentTransform->children.push_back(thisEntity);
    }
}


glm::mat4x4 Transform3D::getMatrix() const {
    return matrix;
}


void Transform3D::updateMatrix() {

    matrix = glm::scale(mat4(1.0f), scale);
    matrix = matrix * mat4_cast(rotation);
    matrix = translate(matrix, position);       

    if( parent != nullptr ) {
        Transform3D* parentTransform = parent->getComponent<Transform3D>();
        matrix = parentTransform->matrix * matrix;
    }

    for( ECS::Entity* child : children) {
        Transform3D* childTransform = child->getComponent<Transform3D>();
        childTransform->updateMatrix();
    }
}