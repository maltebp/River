#include "GL.h"

#include <vector>
#include <sstream>
#include <iomanip>
#include "River/Error.h"

namespace River {
	namespace GLError {
		// Just a small helper function to convert int to hex
		static std::string error_to_hex(GLenum i) {
			std::stringstream stream;
			stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << i;
			return stream.str();
		}

		void glCheckError() {
			GLenum err = glGetError();
			if (err == GL_NO_ERROR) return;

			// Get all errors
			std::vector<GLenum> errors;
			errors.push_back(err);
			while ((err = glGetError()) != GL_NO_ERROR)
				errors.push_back(err);

			std::stringstream msg;
			if (errors.size() > 1)
				msg << errors.size() << " GL errors occured: ";
			else
				msg << "1 GL error occured: ";

			for (int i = 0; i < errors.size(); i++) {
				msg << error_to_hex(errors[i]);
				if (i < (errors.size() - 1))
					msg << ", ";
			}

			throw new GLException(msg.str());
		}
	}
}

