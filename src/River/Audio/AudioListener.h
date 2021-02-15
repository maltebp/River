#pragma once

namespace River {

	/**
	 * @brief	Represents listener of audio emitted by AudioPlayers, by master volume
	 *			and spatial data (position, velocity, depth)
	 * 
	 *			Units of spatial values (position, size, range, velocity),
	 *			are arbitrary, as the AudioPlayer is only concerned with the
	 *			relativity of its own values and the values of the AudioListener
	 *			and other AudioPlayers
	 */
	class AudioListener {
	public:

		/**
		 * @brief	The perceived volume modifier for all audio. Effectively the master
		 *			volume of all AudioPlayers
		 * 
		 * @param volume	The volume in decimal percentage. A value of 0 may be used to mute it.
		 *					May be larger 1, which may have an effect for spatial sounds
		 *
		 * @throws River::InvalidArgumentException if volume is less than 0
		*/
		static void setVolume(double volume);

		static double getVolume();

		/**
		 * @brief	Sets the 2D position of the AudioPlayer
		*/
		static void setPosition(double positionX, double positionY);

		static double getPositionX();

		static double getPositionY();

		/**
		 * @brief	Sets 2D velocity of the AudioPlayer. The velocity
		 *			will have an effect on the audio pitch and volume
		 *			as described by the Doppler effect
		*/
		static void setVelocity(double velocityX, double velocityY);

		static double getVelocityX();

		static double getVelocityY();

		/**
		 * @brief	The AudioListener's depth may be considered as its
		 *			z-value. Setting the depth, smoothens the spatial
		 *			effect of the audio.
		 *
		 * @param depth	Defaults to 0
		 * 
		 * @throws River::InvalidArgumentException if the depth is less than 0
		*/
		static void setDepth(double depth);

		static double getDepth();

		/**
		 * @brief	The 2D rotation of the AudioListener, determines the "direction"
		 *			of the left and right ear. The default rotation, 0, sets the left
		 *			ear in direction of the negative x-axis, and the right in the direction
		 *			of the position x-axis
		 *
		 * @param	rotation 2D counter-clockwise rotation of the listener in degrees. Values below 0
		 *			and above 360 will automatically be wrapped around
		 *
		 * @throws River::InvalidArgumentException if the depth is less than 0
		*/
		static void setRotation(double rotation);

		static double getRotation();


	private:

		AudioListener() = delete;

		static inline double volume = 1.0;

		static inline double positionX = 0;
		static inline double positionY = 0;

		static inline double velocityX = 0;
		static inline double velocityY = 0;

		static inline double depth = 0;

		static inline double rotation = 0;

	};

}

