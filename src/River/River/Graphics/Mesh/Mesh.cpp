#include "River/pch.h"
#include "Mesh.h"

#include "River/Error.h"

using namespace River;
using namespace glm;


Mesh::Mesh(
    const std::vector<vec3>& positions,
    const std::vector<vec3>& normals,
    const std::vector<unsigned int>& indices    
) 
    :   vertexBuffer((positions.size() + normals.size()) * sizeof(vec3), BufferUsageHint::STATIC),
        numTriangles(indices.size() / 3)
{
    if( positions.size() != normals.size() ) throw InvalidArgumentException("N");
    if( indices.size() == 0 ) throw InvalidArgumentException("No indices given");
    if( indices.size() % 3 != 0 ) throw InvalidArgumentException("Invalid number of indices (not divisible by 3)");

    size_t normalsOffset = normals.size() * sizeof(vec3);

    vertexBuffer.setSubData(positions, 0);
    vertexBuffer.setSubData(normals, normalsOffset);

    indexBuffer.setData(indices);

    vertexArray.setAttribute(0, VertexAttributes::FLOAT3, &vertexBuffer, 0, 0);
    vertexArray.setAttribute(1, VertexAttributes::FLOAT3, &vertexBuffer, normalsOffset, 0);
    
    vertexArray.setIndexBuffer(&indexBuffer, VertexIndexTypes::UINT);
}


VertexArray& Mesh::getVertexArray() {
    return vertexArray;
}


const VertexArray& Mesh::getVertexArray() const {
    return vertexArray;
}


size_t Mesh::getNumTriangles() const {
    return numTriangles;
}