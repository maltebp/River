#pragma once

#include <sstream>
#include <iomanip>


/**
 * @brief		Converts the value of the given type to a hex string in the format '0x0A1234'
 *
 * @param t		The value to convert
 * @param width Number of symbols to proceed the '0x'. This will have no effect if it less
 *				than the width required to represent the given value.
 *				If 0, the width is set to the width of the value of the given type with all
 *				bits set to 1.
 * @return		The resulting string
*/
template <typename T>
inline std::string toHexString(T t, unsigned int width = 0) {
	unsigned int actualWidth = width == 0 ? sizeof(t) * 2 : width;
	std::stringstream sstream;
	sstream << "0x" << std::setfill('0') << std::uppercase << std::setw(actualWidth) << std::hex << t;
	return sstream.str();
}


/**
 * @brief		Converts the value of the given type to a hex string in the format '0x0A1234'
 *
 * @param t		The value to convert
 * @param width Number of symbols to proceed the '0x'. This will have no effect if it less
 *				than the width required to represent the given value.
 *				If 0, the width is set to the width of the value of the given type with all
 *				bits set to 1.
 * @return		The resulting string
*/
template <>
inline std::string toHexString(unsigned char c, unsigned int width) {
	unsigned int actualWidth = width == 0 ? sizeof(c) * 2 : width;
	std::stringstream sstream;
	sstream << "0x"
		<< std::setfill('0') << std::uppercase << std::setw(actualWidth) << std::hex
		<< (unsigned int)c;
	return sstream.str();
}


/**
 * @brief		Converts the value of the given type to a hex string in the format '0x0A1234'
 *
 * @param t		The value to convert
 * @param width Number of symbols to proceed the '0x'. This will have no effect if it less
 *				than the width required to represent the given value.
 *				If 0, the width is set to the width of the value of the given type with all
 *				bits set to 1.
 * @return		The resulting string
*/
template <>
inline std::string toHexString(char c, unsigned int width) {
	unsigned int actualWidth = width == 0 ? sizeof(c) * 2 : width;
	std::stringstream sstream;
	sstream << "0x"
		<< std::setfill('0') << std::uppercase << std::setw(actualWidth) << std::hex
		<< (int)c;
	return sstream.str();
}