#pragma 

#include <vector>

#include "River/Graphics/Graphics.h"

namespace River {

	template<typename T>
	class VertexArray {	

	private:
		struct Attribute {
			unsigned int type;
			unsigned int count;
			unsigned int size;
		};

		unsigned int vertexArrayId;
		unsigned int vertexBufferId;
		unsigned int indexBufferId;

		std::vector<T> vertices;
		unsigned int numVertices = 0;
		
		std::vector<Attribute> attributes;
		unsigned int stride;

		std::vector<unsigned int> indices;
		unsigned int numIndices = 0;


		// Prevent copying for now
		VertexArray(const VertexArray&);
		VertexArray& operator=(const VertexArray&);


	protected:
		virtual void setupAttributes() = 0;

		void addAttributeFloat(unsigned int count) {
			addAttribute({ GL_FLOAT, count, sizeof(GLfloat) });
		}

	private:
		void addAttribute(Attribute attribute) {
			attributes.push_back(attribute);
			stride += attribute.count * attribute.size;
		}

	public:
		VertexArray() {
			
		}

		~VertexArray() {
			// TODO: Implement cleanup
		}

		//T* next() {
		//	T* element = &vertices.at(nextVertexIndex);
		//	nextVertexIndex++;
		//	return element;
		//}

		void initialize() {
			glGenVertexArrays(1, &vertexArrayId);
			glGenBuffers(1, &vertexBufferId);
			glGenBuffers(1, &indexBufferId);

			GLint currentVertexArray = 0;
			GLint currentVertexBuffer = 0;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray);
			glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &currentVertexBuffer);

			glBindVertexArray(vertexArrayId);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
			setupAttributes();

			if (stride != sizeof(T)) {
				glDeleteVertexArrays(1, &vertexArrayId);
				glDeleteBuffers(1, &vertexBufferId);
				glDeleteBuffers(1, &indexBufferId);
				glBindVertexArray(currentVertexArray);
				glBindBuffer(GL_ARRAY_BUFFER, currentVertexBuffer);
				throw River::Exception("Size of attributes does not match size of struct");
			}

			// Setup and enable the attribute points
			unsigned int offset = 0;
			for (int i = 0; i < attributes.size(); i++) {
				Attribute& attribute = attributes[i];
				glVertexAttribPointer(i, attribute.count, attribute.type, GL_FALSE, stride, (const void*)offset);
				glEnableVertexAttribArray(i);
				offset += attribute.size * attribute.count;
			}

			glBindVertexArray(currentVertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, currentVertexBuffer);
		}

		T* nextVertices(unsigned int count) {
			if( vertices.size() < (numVertices+count) )
				vertices.resize(numVertices + count);
			T* nextElement = &vertices.at(numVertices);
			numVertices += count;
			return nextElement;
		}

		void clear() {
			numVertices = 0;
			numIndices = 0;
		}

		unsigned int* nextIndices(unsigned int count) {
			if( indices.size() < (numIndices+count) )
				indices.resize(numIndices + count);
			unsigned int* nextIndex = &indices.at(numIndices);
			numIndices += count;
			return nextIndex;
		}

		void bind() {
			glBindVertexArray(vertexArrayId);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * numVertices, (void*) vertices.data(), GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), (void*) indices.data(), GL_STATIC_DRAW);
		}

		void unbind() {
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		unsigned int getNumIndices() {
			return numIndices;
		}

		unsigned int getNumVertices() {
			return numVertices;
		}


	};
}


