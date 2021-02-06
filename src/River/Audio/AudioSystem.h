#pragma once

#include "AudioAsset.h"

#include <unordered_map>
#include <vector>


namespace River {

	class AudioSystem;


	// TODO: Clean up this class (and add comments)
	class AudioInstance {
		friend AudioSystem;
	
	private:
		void deactivate();
		void activate();

	private:
		AudioAsset* asset;
		bool looping = false;
		unsigned int priority;
		double heuristic = 0;

		// TODO: Make sure this does not go over 1
		double volume = 1.0;

		bool active = false;

		bool finished = false;

		double length;
		double currentTime = 0;

		double positionX, positionY = 0;
		double velocityX, velocityY = 0;
	};


	class AudioSystem {
	public:

		static void initialize();

		static void update(double time);

		// TODO: JUST FOR TESTING (WILL BE DELETED!)
		static void playAudio(AudioAsset* asset);

		static inline const double DEFAULT_REFERENCE_DISTANCE = 1.0;

		static void setMasterVolume(double volume);

		static double getMasterVolume();

		static void setReferenceDistance(double distance);

		static double getReferenceDistance();

		static void setListenerPosition(double positionX, double positionY);

		static void setListenerVelocity(double velocityX, double velocityY);


	private:
		static double calculateHeuristic(AudioInstance* audioInstance);


	private:
		static inline bool initialized = false;

		static inline double referenceDistance = DEFAULT_REFERENCE_DISTANCE;

		static inline double listenerPositionX, listenerPositionY = 0.0;
		static inline double listenerVelocityX, listenerVelocityY = 0.0;

		static inline double masterVolume = 1.0;

		// ALL audio instances that are currently registered
		static inline std::vector<AudioInstance*> audioInstances;

		// Mapping of how many audio instances uses the a given AudioAsset
		static inline std::unordered_map<AudioAsset*, unsigned int> assetInstanceCount;
	};

}