#pragma once

namespace River {
	class Layer {

	protected:
		Layer(){}

		virtual void onInitialization(){}
		virtual void onUpdate(){}
		virtual void onTermination(){}

	public:
		void initialize();
		void update();
		void terminate();
	};
}


