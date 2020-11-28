#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <River/Error.h>


namespace River {

	struct MouseButton {
	public:

		/**
		 * @brief Creates a new MouseButton, without defining a name for its code
		 */
		MouseButton(uint32_t code) :code(code) {}	


		/**
		 * @brief	This registers a new mouse button
		 */
		MouseButton(const std::string& name, uint32_t code) : code(code) {
			if( names.find(code) != names.end() )
				throw new InvalidArgumentException("Duplicate mouse button code '" + std::to_string(code) + "'");
			names.emplace(code, name);
		}

		const std::string& getName() const {
			return names.find(code)->second;
		}

		const uint32_t code;

		friend std::ostream& operator<<(std::ostream&, const MouseButton&);


	private:
		static inline std::unordered_map<uint32_t, std::string> names;		
	};

	
	// Note: This remove the possibility of hashing this class. It could be implemented but it's to extensive
	inline bool operator==(const MouseButton& btn1, const MouseButton& btn2) {
		return btn1.code == btn2.code;
	}

	inline std::ostream& operator<<(std::ostream& strm, const MouseButton& btn) {
		return strm << btn.getName();
	}


	namespace MouseButtons {

		/**
		 * @brief	An unknown mouse button (may indicate an error has occured in the framework)
		*/
		inline const MouseButton	UNKNOWN		= { "Unknown",	0	};

		inline const MouseButton	LEFT		= { "Left",		1	};
		inline const MouseButton	RIGHT		= { "Right",	2	};

		// Mousewheel press
		inline const MouseButton	MIDDLE		= { "Middle",	3	};
		
		// This may depend on the mouse settings
		inline const MouseButton	EXTRA1		= { "Extra1",	4	};
		inline const MouseButton	EXTRA2		= { "Extra2",	5	};
		inline const MouseButton	EXTRA3		= { "Extra3",	6	};
		inline const MouseButton	EXTRA4		= { "Extra4",	7	};
		inline const MouseButton	EXTRA5		= { "Extra5",	8	};
	}
}