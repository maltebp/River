#include "River/pch.h"

#include "GL.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "River/Error.h"

namespace River {
	namespace GLError {
		// Just a small helper function to convert int to hex
		static std::string errorToHex(GLenum i) {
			std::stringstream stream;
			stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << i;
			return stream.str();
		}


		static std::string errorToString(GLenum errorCode) {
			switch(errorCode){
			case 0x0500: return "Invalid enum";
			case 0x0501: return "Invalid value";
			case 0x0502: return "Invalid operation";
			case 0x0503: return "Stack overflow";
			case 0x0504: return "Stack underflow";
			case 0x0505: return "Out of memory";
			case 0x0506: return "Invalid framebuffer operation";
			case 0x0507: return "Context lost";
			case 0x0508: return "Table too large"; // Should be a deprecated error
			}
			return "Unknown error";
		}


		void glCheckError() {

			GLenum error = glGetError();
			if (error == GL_NO_ERROR) return;

			// Check if GL has even been initialized (it will also throw an 0x0502 error (INVALID_OPERATION)
			// In that case, calls to glGetError will also return an error, causing an infinite loop
			if( glfwGetCurrentContext() == nullptr )
				throw new GLException("No GL context!");

			// Collect all occured errors
			std::vector<GLenum> errors;
			errors.push_back(error);
			while( true ) {
				error = glGetError();
				if( error == GL_NO_ERROR ) break;
				errors.push_back(error);
			}

			std::stringstream msg;	
			msg << errors.size();

			if( errors.size() > 1 )
				msg << " GL errors occured: ";
			else
				msg << " GL error occured: ";

			for (int i = 0; i < errors.size(); i++) {
				msg << "'" << errorToString(errors[i]) << " (" <<  errorToHex(errors[i]) << ")'";
				if (i < (errors.size() - 1))
					msg << ", ";
			}

			throw new GLException(msg.str());
		}
	}
}

