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


	class NullException : public Exception {
	public:
		NullException(const std::string& message) : Exception("NullException", message) {}
	};


	class InvalidStateException : public Exception {
	public:
		InvalidStateException(const std::string& message) : Exception("InvalidStateException", message) {}
	};


	class InvalidArgumentException : public Exception {
	public:
		InvalidArgumentException(const std::string& message) : Exception("InvalidArgumentException", message) {}
	};


	class NotImplementedException : public Exception {
	public:
		NotImplementedException(const std::string &message) : Exception("NotImplementedException", message) { }
	};


	class AssetException : public Exception{
	protected:
		AssetException(const std::string& exceptionName, const std::string& message) : Exception(exceptionName, message) {}
	public:
		AssetException(const std::string& message) : Exception("AssetException", message) {}
	};

	class InvalidAssetStateException: public AssetException {
	protected:
		InvalidAssetStateException(const std::string& exceptionName, const std::string& message) : AssetException(exceptionName, message) {}
	public:
		InvalidAssetStateException(const std::string& message) : AssetException("InvalidAssetStateException", message) {}
	};

	class AssetNotLoaded : public AssetException {
	public:
		AssetNotLoaded(const std::string& assetName) : AssetException("AssetNotLoadedException", "Asset '" + assetName + "' has not been loaded") {}
		AssetNotLoaded() : AssetException("AssetNotLoadedException", "Asset has not been loaded") {}

	};

	class AssetNotFoundException : public AssetException {
	public:
		AssetNotFoundException(const std::string& assetName) : AssetException("AssetNotFoundException", "The asset '" + assetName + "' could not be found") {}
	};

	class AssetDeletionFailedException : public AssetException {
	public:
		AssetDeletionFailedException(const std::string& msg) : AssetException("AssetNotFoundException", msg) {}
	};


	class ShaderException : public Exception {
	public:
		ShaderException(const std::string& message) : Exception("ShaderException", message) {}
	};

	class GLException : public Exception {
	public:
		GLException(const std::string& message) : Exception("GLException", message) {}
	};

	class NoTextureSlotException : public Exception{
	public:
		NoTextureSlotException() : Exception("NoTextureSlotException", "No texture slot is available") {}
	};


	class NotInitializedException : public Exception {
	public:
		NotInitializedException(const std::string& objectName) : Exception("'" + objectName + "' has not been initialized") {}
	};

	class AlreadyInitializedException : public Exception {
	public:
		AlreadyInitializedException(const std::string& objectName) : Exception("'" + objectName + "' has already been initialize"){}
	};
}

