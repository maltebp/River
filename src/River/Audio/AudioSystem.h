#pragma once

#include "AudioAsset.h"

#include <unordered_map>
#include <vector>

#include "AudioInstance.h"


namespace River {
	// TODO: Remove this if left unused
	//struct AudioPlayArguments {

	//	double volume = 1.0;

	//	bool looping = false;

	//	unsigned int priority = 100;

	//	bool is3d = false;

	//	double positionX = 0;
	//	double positionY = 0;

	//	double velocityX = 0;
	//	double velocityY = 0;

	//	bool freeOnFinish;

	//	std::function<void(AudioInstance*)> onFinish;

	//	// TODO: Implement speed (pitch)
	//	//double speed = 1.0;

	//};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


	class AudioSystem {
		friend AudioInstance;
	public:

		static void initialize();

		static void update(double time);

		static void playAudio(AudioInstance* audio);

		static void stopAudio(AudioInstance* audio);

		static void setMasterVolume(double volume);

		static void adjustMasterVolume(double volume);

		static double getMasterVolume();

		static void setReferenceDistance(double distance);

		static double getReferenceDistance();

		static void setListenerPosition(double positionX, double positionY);

		static void setListenerVelocity(double velocityX, double velocityY);

		static inline const double DEFAULT_REFERENCE_DISTANCE = 1.0;


	private:

		static void activateInstance(AudioInstance* instance);
		
		static void deactivateInstance(AudioInstance* instance);

		static double calculateHeuristic(AudioInstance* audioInstance);


	private:
		
		static inline bool initialized = false;

		static inline double referenceDistance = DEFAULT_REFERENCE_DISTANCE;

		static inline double listenerPositionX, listenerPositionY = 0.0;
		static inline double listenerVelocityX, listenerVelocityY = 0.0;

		static inline double masterVolume = 1.0;

		// ALL audio instances that are currently registered
		static inline std::vector<AudioInstance*> playingInstances;

		// Mapping of how many audio instances uses the a given AudioAsset
		static inline std::unordered_map<AudioAsset*, unsigned int> assetInstanceCount;
	};

}