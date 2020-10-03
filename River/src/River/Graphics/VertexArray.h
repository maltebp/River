#pragma 

#include <vector>

#include "GL.h"

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

		void addAttributeUInt(unsigned int count){
			addAttribute({ GL_UNSIGNED_INT, count, sizeof(GLuint) });
		}

		void addAttributeInt(unsigned int count){
			addAttribute({ GL_INT, count, sizeof(GLint) });
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
			GL(glGenVertexArrays(1, &vertexArrayId));
			GL(glGenBuffers(1, &vertexBufferId));
			GL(glGenBuffers(1, &indexBufferId));

			GLint currentVertexArray = 0;
			GLint currentVertexBuffer = 0;
			GL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVertexArray));
			GL(glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &currentVertexBuffer));

			GL(glBindVertexArray(vertexArrayId));
			GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
			setupAttributes();

			if (stride != sizeof(T)) {
				GL(glDeleteVertexArrays(1, &vertexArrayId));
				GL(glDeleteBuffers(1, &vertexBufferId));
				GL(glDeleteBuffers(1, &indexBufferId));
				GL(glBindVertexArray(currentVertexArray));
				GL(glBindBuffer(GL_ARRAY_BUFFER, currentVertexBuffer));
				throw River::Exception("Size of attributes does not match size of struct");
			}

			// Setup and enable the attribute points
			unsigned int offset = 0;
			for (int i = 0; i < attributes.size(); i++) {
				Attribute& attribute = attributes[i];
				#pragma warning(suppress: 4312) // Suppress warning about casting to void*
				GL(glVertexAttribPointer(i, attribute.count, attribute.type, GL_FALSE, stride, (void*) offset));
				GL(glEnableVertexAttribArray(i));
				offset += attribute.size * attribute.count;
			}

			GL(glBindVertexArray(currentVertexArray));
			GL(glBindBuffer(GL_ARRAY_BUFFER, currentVertexBuffer));
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
			GL(glBindVertexArray(vertexArrayId));
			GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
			GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
			GL(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * numVertices, (void*) vertices.data(), GL_STATIC_DRAW));
			GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), (void*) indices.data(), GL_STATIC_DRAW));
		}

		void unbind() {
			GL(glBindVertexArray(0));
			GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		unsigned int getNumIndices() {
			return numIndices;
		}

		unsigned int getNumVertices() {
			return numVertices;
		}




	};
}


