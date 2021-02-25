#pragma once

#include <functional>
#include <map>


namespace River {

	// TODO: Comment this

	template <typename ... Args>
	using Listener = std::function<void(Args...)>;

	template <typename ... Args>
	struct ListenerInvoker;

	template <typename ... Args>
	struct ListenerMap {
		friend ListenerInvoker<Args...>;
	public:

		void add(void* handle, Listener<Args...> listener) {
			// TODO: Throw exception if void* is already registered
			listeners[handle] = listener;
		}

		void remove(void* handle) {
			listeners.erase(handle);
		}

	private:

		std::map<void*, Listener<Args...>> listeners;

	};

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
