#pragma once

#include <string>
#include <vector>

#include "River/Graphics/GL.h"
#include "Shader.h"
#include "River/Error.h"

namespace River {
	class ShaderProgram {

	public:
		
	private:
		unsigned int id = 0;

		bool ready = false;

		Shader* fragmentShader = nullptr;
		Shader* vertexShader = nullptr;

		std::string getInfoLog();

		// Prevent copying
		ShaderProgram(const ShaderProgram&);
		ShaderProgram& operator=(const ShaderProgram&);

	public:
		ShaderProgram();
		~ShaderProgram();

		void setFragmentShader(Shader* shader);
		void setVertexShader(Shader* shader);

		
		/*
			@throws River::ShaderException
		*/
		void build();

		void use();


		// Uniforms -----------------------------------------------------------------
		//void setUniformFloat(const std::string& name, float value);
		void setIntArray(const std::string& name, int* values, int numValues);

		/**
		 * @brief Matrix is column-major order (see GLM documentation)
		 * @param	The number of columns and rows in the matrix
		*/
		void setFloatMatrix(const std::string &name, int size, float *values);

		bool hasUniform(const std::string& name);
	private:
		/**
		 * @throws	River::ShaderException	 Thrown if the given uniform doesn't exist
		*/
		GLint getUniformLocation(const std::string &name);
	};
}


