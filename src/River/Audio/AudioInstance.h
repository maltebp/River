#pragma once

#include "AudioAsset.h"

#include <functional>

namespace River {

	class AudioSystem;

	class AudioInstance {
		friend AudioSystem;
	public:
		
		AudioInstance(AudioAsset* asset);

		~AudioInstance();
		
		void play();

		bool isPlaying();

		void loop(bool toggle);

		bool isLooping();

		void setVolume(double volume);

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
		unsigned int priority = 0;
		bool threeD = false;

		// TODO: Make sure this does not go over 1
		double volume = 1.0;
		double positionX = 0;
		double positionY = 0;
		
		double velocityX = 0;
		double velocityY = 0;

		bool dirty = false;
		bool timeDirty = false;
		double currentTime = 0;
		bool active = false;
		bool finished = false; // TODO: Is this ever used?
		double heuristic = 0;
	};

}
