#pragma once

#include "AudioAsset.h"

#include <functional>

namespace River {

	class Game;

	/**
	 * @brief	Emitter of a single audio source, either globally or spatially
	 *			
	 *			Units of spatial values (position, size, range, velocity),
	 *			are arbitrary, as the AudioPlayer is only concerned with the
	 *			relativity of its own values and the values of the AudioListener
	 *			and other AudioPlayers
	 */
	class AudioPlayer {
		friend Game;
	public:

		/**
		 * @brief	The default range for all spatial players
		*/
		static inline const double DEFAULT_RANGE = 1000;

		/**
		 * @brief	Default priority value for all spatial players
		*/
		static inline const unsigned int DEFAULT_PRIORITY = AudioAsset::DEFAULT_PRIORITY;
		
		AudioPlayer();

		/**
		* @brief Destroying the AudioPlayer will automatically stop audio from playing
		*/
		~AudioPlayer();
		
		/**
		 * @brief	Make the AudioPlayer play the given AudioSource from the beginning
		 *			of the audio asset. Passing the current as the parameter, makes it
		 *			work as a restart. It has no effect whether the AudioPlayer is
		 *			paused or not
		 * 
		 * @param asset	Audio to play
		*/
		void play(AudioAsset* asset);

		/**
		 * @return  Whether the AudioPlayer is playing. It is not considered playing
		 *			when it has been paused.
		*/
		bool isPlaying();

		/**
		 * @brief	Sets the priority for this AudioPlayer. The value has no absolute
		 *			meaning, as it is only relative to the priorty of other AudioPlayers.
		 *			Its default value is AudioPlayer:DEFAULT_PRIORITY
		 * @param  
		*/
		void setPriority(unsigned int priorty);

		unsigned int getPriority();

		/**
		 * @param volume	The volume in decimal percentage. A value of 0 may be used to mute it.
		 *					May be larger 1, which may have an effect for spatial sounds
		 *
		 * @throws River::InvalidArgumentException if volume is less than 0
		*/
		void setVolume(double volume);

		double getVolume();
		
		/**
		 * @brief	Sets whether the AudioPlayer should loop.
		 *			This may be set after the AudioPlayer has started
		 * 
		*/
		void setLooping(bool toggle);

		bool isLooping();

		/**
		 * @brief	Sets the playback speed of the AudioPlayer
		 *			May be set while playing
		 * 
		 * @param speed	The speed in decimal percentage
		 * 
		 * @throws River::InvalidArgumentException if the speed is not greater than 0
		*/
		void setSpeed(double speed);

		double getSpeed(double speed);

		/**
		 * @param volume	Sets whether this AudioPlayer should be spatial.
		 *					This will only have an effect if the AudioPlayer's
		 *					position or depth differs from the AudioListener's.
		 *
		 * @throws River::InvalidArgumentException if volume is less than 0
		*/
		void setSpatial(bool toggle);

		bool isSpatial();


		/**
		 * @brief	The size of the AudioPlayer determines how far away
		 *			from its position that the audio will not be attenuated.
		 *			This only has an effect for spatial AudioPlayers, and will
		 *			also affect the depth
		 * 
		 * @param size	The size in the user defined unit. May not be negative.
		 *				Defaults to 0
		 * 
		 * @throws	River::InvalidArgumentException if size is less than 0, or
		 *			if the size is not less than the AudioPlayer's range
		*/
		void setSize(double size);

		double getSize();

		/**
		 * @brief	Determines how far away from the AudioPlayer's position that
		 *			its emitted sound may be heard. The audio will be linearly
		 *			attenuated in this range.
		 *			This only has an effect for spatial AudioPlayers
s		 *
		 * @param size	May not be negative. Defaults to AudioPlayer::DEFAULT_RANGE
		 *
		 * @throws	River::InvalidArgumentException if the range is less than 0,
		 *			or if the range is not greater than the AudioPlayer's size
		*/
		void setRange(double range);

		double getRange();

		/**
		 * @brief	Sets the 2D position of the AudioPlayer
		*/
		void setPosition(double positionX, double positionY);

		double getPositionX();

		double getPositionY();

		/**
		 * @brief	Sets 2D velocity of the AudioPlayer. The velocity
		 *			will have an effect on the audio pitch and volume
		 *			as described by the Doppler effect
		*/
		void setVelocity(double velocityX, double velocityY);

		/**
		 * @brief	The AudioPlayer's depth may be considered as its
		 *			z-value. Setting the depth, smoothens the spatial
		 *			effect of the audio.
		 * 
		 * @param depth Sets the depth
		*/
		void setDepth(double depth);

		double getDepth(double depth);

		
		/**
		 * @brief Sets how far into the non-sped up AudioAsset the AudioPlayer is
		 *	
		 * @param time	Time in seconds. If the time is larger than the length
		 *				of the non-sped up AudioAsset, the current time will wrap
		 *				around if the AudioPlayer is looping or stop if not.
		*/
		void setTime(double time);

		double getTime();

		/**
		 * @brief	Different from stopping the AudioPlayer, as it may be unpaused and
		 *			continue playing from the time it was paused. 
		 *			If the AudioPlayer is not playing this will have no effect, in particular,
		 *			isPaused will not return true.
		*/
		void pause();

		void unpause();

		/**
		 * @return  True if the AudioPlayer has been paused. Will false if it was
		 *			paused while not playing, or if was restarted after being paused
		*/
		bool isPaused();
		
		/**
		 * @brief	Different from stopping the AudioPlayer, as it may be unpaused and
		 *			continue playing from the time it was paused.
		 *			If the AudioPlayer is not playing this will have no effect, in particular,
		 *			isPaused will not return true.
		*/
		void stop();

		/**
		 * @brief	Sets a callback which will fire when the AudioPlayer has successfully
		 *			finished playing its AudioAsset. If the AudioPlayer is looping it will
		 *			not trigger this, not will it be triggered if the AudioPlayer is manually
		 *			stopped.
		 *			The AudioPlayer may safely replayed or deleted within this callback
		*/
		void onFinish(std::function<void(AudioPlayer*)> callback);

		/**
		 * @return	The asset, which the AudioPlayer is currently playing, or has 
		 *			most recently played.
		*/
		AudioAsset* getAsset();


	private:

		/**
		 * @brief	Updates all playing AudioPlayers, which effectively is to
		 *			update the internal audio system.
		 *
		 * @param time	Time in seconds which has passed since last update
		*/
		static void updatePlayers(double time);

		void activate();

		void deactivate();

		bool isActive();

		double calculateHeuristic();


	private:

		AudioAsset* asset = nullptr;
		
		bool playing = false;

		bool paused = false;
		
		double volume = 1.0;

		double speed = 1.0;

		bool looping = false;
		
		unsigned int priority = DEFAULT_PRIORITY;

		bool spatial = false; 

		double range = DEFAULT_RANGE;

		double size = 0;

		double depth = 0;
		
		double positionX = 0;
		double positionY = 0;
		
		double velocityX = 0;
		double velocityY = 0;

		double currentTime = 0;

		std::function<void(AudioPlayer*)> onFinishCallback = nullptr;

		double heuristic = 0;

		void* nativeObject = nullptr;

	};

}
