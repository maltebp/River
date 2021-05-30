#pragma once

#include "River/Graphics/GL.h"
#include "River/Error.h"
#include "VertexShader.h"
#include "FragmentShader.h"


namespace River {


	class ShaderProgram {
	public:

		ShaderProgram();

		~ShaderProgram();

		void build(const VertexShader& vertexShader, const FragmentShader& fragmentShader);

		void use();

		// Uniforms -----------------------------------------------------------------
		//void setUniformFloat(const std::string& name, float value);
		void setIntArray(const std::string& name, const GLint* values, int numValues);

		/**
		 * @brief Matrix is column-major order (see GLM documentation)
		 * @param	The number of columns and rows in the matrix
		*/
		void setFloatMatrix(const std::string &name, unsigned int size, const GLfloat* values);

		bool hasUniform(const std::string& name) const;

		bool isReady() const;


	private:

		// Prevent copying
		ShaderProgram(const ShaderProgram&) = delete;

		ShaderProgram& operator=(const ShaderProgram&) = delete;

		/**
		 * @throws	River::ShaderException	 Thrown if the given uniform doesn't exist
		*/
		GLint getUniformLocation(const std::string &name);

		std::string getInfoLog();


	private:

		bool ready = false;

		unsigned int id = 0;

	};

}


