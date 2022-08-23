#pragma once

#include <vector>

#include "River/External/glm/glm.hpp"
#include "River/External/glm/gtc/quaternion.hpp"
#include "River/External/RiverECS/ECS.h"


namespace River {

	struct Transform3D : ECS::Component {
    public:

        void setPosition(glm::vec3);

        glm::vec3 getLocalPosition() const;

        glm::vec3 getWorldPosition() const;

        void setScale(glm::vec3 scale);

        glm::vec3 getLocalScale() const;

        void setRotation(glm::quat rotation);

        glm::quat getLocalRotation() const;        

        void setParent(ECS::Entity* newParent);

        ECS::Entity* getParent() const;

        glm::mat4x4 getMatrix() const;

    private:
        
        void updateMatrix();

    private:

        glm::vec3 position = { 0.0f, 0.0f, 0.0f };

        glm::quat rotation = { 0.0f, 0.0f, 0.0f, 0.0f };

        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };   

        ECS::Entity* parent = nullptr;

        std::vector<ECS::Entity*> children;

        glm::mat4 matrix = glm::mat4(1.0f);

	};

}