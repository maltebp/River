#include "River/pch.h"

#include "SimpleVertexArray.h"

#include "River/Error.h"


namespace River {


	size_t SimpleVertexArray::setAttributes(VertexBuffer* buffer, const std::vector<VertexAttribute>& attributes) {

		// Calculate stride
		size_t stride = 0;
		for( auto& attribute : attributes ) {
			if( attribute == VertexAttributes::NONE ) {
				throw InvalidArgumentException("Attribute type must not VertexAttribute::NONE");
			}

			stride += attribute.getTypeSize() * (size_t)attribute.getCount();
		}

		// Setup all the attributes
		int index = 0;
		size_t offset = 0;
		for( auto& attribute : attributes ) {
			size_t size = attribute.getTypeSize() * (size_t)attribute.getCount();
			size_t spacing = stride - size;
			setAttribute(index, attribute, buffer, offset, spacing);
			index++;
			offset += size;
		}

		return stride;		
	}


}


