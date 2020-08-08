#pragma once

#include <string>

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


		// TODO: Create setUniform--() functions when needed

		
	};
}


