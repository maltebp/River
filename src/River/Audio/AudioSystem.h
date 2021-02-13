#pragma once

#include "AudioAsset.h"

#include <unordered_map>
#include <vector>

#include "AudioInstance.h"


namespace River {
	
	class AudioSystem {
		friend AudioInstance;
	public:

		static void initialize();

		static void update(double time);

		static void playAudio(AudioInstance* audio);

		static void stopAudio(AudioInstance* audio);

		static void setMasterVolume(double volume);

		static double getMasterVolume();

		static void setListenerPosition(double positionX, double positionY);

		static void setListenerVelocity(double velocityX, double velocityY);	

		static void setListenerDepth(double depth);

		static double getListenerDepth();

		// Will clamp the angle
		static void setListenerRotation(double rotation);

		static double getListenerRotation();


	private:

		static void activateInstance(AudioInstance* instance);
		
		static void deactivateInstance(AudioInstance* instance);

		static double calculateHeuristic(AudioInstance* audioInstance);


	private:
		
		static inline bool initialized = false;

		static inline double masterVolume = 1.0;

		static inline double listenerPositionX = 0;
		static inline double listenerPositionY = 0;
		
		static inline double listenerVelocityX = 0;
		static inline double listenerVelocityY = 0;

		static inline double listenerDepth = 0;

		static inline double listenerRotation = 0;

		// ALL audio instances that are currently registered
		static inline std::vector<AudioInstance*> playingInstances;

		// Mapping of how many audio instances uses the a given AudioAsset
		static inline std::unordered_map<AudioAsset*, unsigned int> assetInstanceCount;
	};

}