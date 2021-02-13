#pragma once

#include "AudioAsset.h"

#include <functional>

namespace River {

	class AudioSystem;

	class AudioInstance {
		friend AudioSystem;
	public:

		static inline const double DEFAULT_RANGE = 1000;

		static inline const unsigned int DEFAULT_PRIORITY = AudioAsset::DEFAULT_PRIORITY;
		
		AudioInstance(AudioAsset* asset);

		~AudioInstance();
		
		void play();

		bool isPlaying();

		void setPriority(unsigned int);

		unsigned int getPriority();
		
		void setLooping(bool toggle);

		bool isLooping();

		void setSpeed(double speed);

		double getSpeed(double speed);

		void setVolume(double volume);

		void set3D(bool toggle);

		bool is3D();

		// Notes:
		//  - May also adjust range
		//  - Affects the depth
		void setSize(double size);

		double getSize();

		// Also affects depth
		void setRange(double range);

		double getRange();

		void setPosition(double positionX, double positionY);

		void setVelocity(double velocityX, double velocityY);

		void setDepth(double depth);

		double getDepth(double depth);

		// Relative to the non speed adjusted asset
		void setTime(double time);

		double getTime();

		void pause();

		void unpause();

		bool isPaused();
		
		void resume();

		void stop();

		// Also works when looping
		void restart();

		// Not fired when loop has finished
		void onFinish(std::function<void(AudioInstance*)> callback);

	
	private:

		void activate(void* nativeObject);

		void* deactivate();

		bool isActive();


	private:

		AudioAsset* asset;
		
		bool playing = false;

		bool paused = false;
		
		double volume = 1.0;

		double speed = 1.0;

		bool looping = false;
		
		unsigned int priority = DEFAULT_PRIORITY;

		bool threeD = false; 

		double range = DEFAULT_RANGE;

		double size = 0;

		double depth = 0;
		
		double positionX = 0;
		double positionY = 0;
		
		double velocityX = 0;
		double velocityY = 0;

		double currentTime = 0;
		
		double heuristic = 0;
	
		void* nativeObject = nullptr;

		std::function<void(AudioInstance*)> onFinishCallback = nullptr;
	};

}
