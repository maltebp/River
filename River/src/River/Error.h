#pragma once

#include <stdexcept>




namespace River {

	/// <summary>
	/// Generic River exception, which all other River exceptions inherits from
	///	<seealso cref="std::runtime_error"/>
	/// </summary>
	/// <see cref="std::runtime_error"/>
	class Exception : public std::runtime_error {
	protected:
		Exception(const std::string &name, const std::string &message) : std::runtime_error("River::" + name + ": " + message){}
	
	public:
		Exception(const std::string& message) : Exception("Exception", message) {}

	};

	class AssetNotFoundException : public Exception {
	public:
		AssetNotFoundException(const std::string& assetName) : Exception("AssetNotFoundException", "The asset '" + assetName + "' could not be found") {}
	};

	class ShaderException : public Exception {
	public:
		ShaderException(const std::string& message) : Exception("ShaderException", message) {}
	};
}

