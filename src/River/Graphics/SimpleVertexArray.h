#pragma once

#include <type_traits>

#include "VertexArray.h"


namespace River {

	/**
	 * @brief	Simplification of the Vertex Array, which assumes a single buffer and attributes
	 *			that are interleaved with no gaps.
	*/
	class SimpleVertexArray : public VertexArray {
	public:

		/**
		 * @brief	Set the attributes for the array. The order of the attributes will translate
		 *			to the indices of the attributes (first attribute is index 0).
		 * 
		 * @param buffer	The buffer the array should use. 
		 * @param firstType	First attribute type (required). Must not be VertexAttribute::NONE.
		 * @param types		Additional attribute types. Must not be VertexAttribute::NONE.
		 * @return	Number of bytes of each vertex (sum of attribute sizes)
		*/
		template <typename ... Attributes>
		size_t setAttributes(VertexBuffer* buffer, VertexAttribute firstType, Attributes... types) {
			static_assert(std::conjunction<std::is_same<VertexAttribute, Attributes>...>::value);

			// Move attributes to vector
			std::vector<VertexAttribute> attributesList;
			attributesList.push_back(firstType);
			(attributesList.push_back((VertexAttribute)types), ...);

			// Setup attributes in internal method
			return setAttributes(buffer, attributesList);
		}

		/**
		 * @brief	Set the attributes for the array. The order of the attributes will translate
		 *			to the indices of the attributes (first attribute is index 0).
		 *
		 * @param buffer	The buffer the array should use
		 * @param firstType	 List of attribute types to add. Must not contain VertexAttribute::NONE.
		 * @return	Number of bytes of each vertex (sum of attribute sizes)
		*/
		size_t setAttributes(VertexBuffer* buffer, const std::vector<VertexAttribute>& attributes);


	private:

		using VertexArray::setAttribute;


	};

}
