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

		// will also adjust range 
		void setSize(double size);

		double getSize();

		void setRange(double range);

		double getRange();

		void setPosition(double positionX, double positionY);

		void setVelocity(double velocityX, double velocityY);

		void set3D(bool toggle);

		bool is3D();

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

		AudioAsset* asset;
		
		bool playing = false;

		std::function<void(AudioInstance*)> onFinishCallback = nullptr;

		bool paused = false;

		double speed = 1.0;
		
		bool looping = false;
		
		unsigned int priority = DEFAULT_PRIORITY;

		bool threeD = false;

		double range = DEFAULT_RANGE;

		// TODO: Make sure this does not go over 1
		double volume = 1.0;

		double size = 0;
		
		double positionX = 0;
		double positionY = 0;
		
		double velocityX = 0;
		double velocityY = 0;

		bool dirty = false; // TODO: Remove this
		
		bool timeDirty = false; // TODO: Remove this
		
		double currentTime = 0;
		
		bool active = false;
		
		bool finished = false; // TODO: Is this ever used?
		
		double heuristic = 0;
	
	};

}
