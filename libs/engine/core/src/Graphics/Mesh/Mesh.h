#pragma once

#include <vector>

#include "River/External/glm/glm.hpp"
#include "River/Graphics/SimpleVertexArray.h"

// TODO: Convert Mesh to Asset
// TODO: Support UV coordinates

namespace River {


    class Mesh {
    public:

        Mesh(
            const std::vector<glm::vec3>& vertices,
            const std::vector<glm::vec3>& normals,

            // TODO: Consider if this should be a list of uivec3 instead (if it is required that
            // they come in sets of 3 anyway - can we ever have a mesh not made up of triangles?)
            const std::vector<unsigned int>& indices
        );       

        // TODO: Consider if this is necessary (do we use it anywhere?)
        VertexArray& getVertexArray();

        const VertexArray& getVertexArray() const;

        size_t getNumTriangles() const;

    private:

        Mesh(const Mesh&) = delete;

        Mesh& operator=(const Mesh&) = delete;

    private:

        VertexArray vertexArray;

        VertexBuffer vertexBuffer;

        IndexBuffer indexBuffer;

        const size_t numTriangles;

    };

}