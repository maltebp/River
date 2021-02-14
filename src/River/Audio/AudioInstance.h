#pragma once

#include "AudioAsset.h"

#include <functional>

namespace River {


	class AudioInstance {
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

		double getVolume();

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

		double getPositionX();

		double getPositionY();

		void setVelocity(double velocityX, double velocityY);

		void setDepth(double depth);

		double getDepth(double depth);

		// Relative to the non speed adjusted asset
		// If set to longer than asset length, it is stopped
		void setTime(double time);

		double getTime();

		// Does not pause if not playing (pause flag isnt set -> isPaused would return false)
		void pause();

		void unpause();

		bool isPaused();
		
		void stop();

		// Not fired when loop has finished
		// Not fired if stopped prematurely
		// The AUdio is entirely free from the playing system at this point
		void onFinish(std::function<void(AudioInstance*)> callback);

		AudioAsset* getAsset();

		// Called by game
		static void updateInstances(double time);


	private:

		void activate();

		void deactivate();

		bool isActive();

		double calculateHeuristic();


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

		std::function<void(AudioInstance*)> onFinishCallback = nullptr;

		double heuristic = 0;

		void* nativeObject = nullptr;

	};

}
