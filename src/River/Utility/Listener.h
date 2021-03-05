#pragma once

#include <functional>
#include <map>


namespace River {


	template <typename ... Args>
	using Listener = std::function<void(Args...)>;

	
	template <typename ... Args>
	struct ListenerInvoker;


	/**
	 * @brief	Encapsulated container for listeners
	 * @tparam ...Args Arguments to Listener
	*/
	template <typename ... Args>
	struct ListenerMap {
		friend ListenerInvoker<Args...>;
	public:

		void add(void* handle, Listener<Args...> listener) {
			if( listeners.find(handle) != listeners.end() ) {
				throw River::InvalidArgumentException("Handle already points to a Listener");
			}
			listeners[handle] = listener;
		}

		void remove(void* handle) {
			listeners.erase(handle);
		}

	private:

		std::map<void*, Listener<Args...>> listeners;

	};


	/**
	 * @brief	Allows access to an encapsulated ListenerMap
	 * @tparam ...Args Arguments to Listener
	*/
	template <typename ... Args>
	struct ListenerInvoker {
	public:

		ListenerInvoker(const ListenerMap<Args...>& listeners)
			: listeners(listeners.listeners) {
		}

	public:

		const std::map<void*, Listener<Args...>>& listeners;

	};

}
