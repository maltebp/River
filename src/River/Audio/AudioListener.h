#pragma once

namespace River {

	class AudioListener {
	public:

		// Is master volume
		static void setVolume(double volume);

		static double getVolume();

		static void setPosition(double positionX, double positionY);

		static double getPositionX();

		static double getPositionY();

		static void setVelocity(double velocityX, double velocityY);

		static double getVelocityX();

		static double getVelocityY();

		static void setDepth(double depth);

		static double getDepth();

		// Will clamp the angle
		static void setRotation(double rotation);

		static double getRotation();


	private:

		static inline double volume = 1.0;

		static inline double positionX = 0;
		static inline double positionY = 0;

		static inline double velocityX = 0;
		static inline double velocityY = 0;

		static inline double depth = 0;

		static inline double rotation = 0;

	};

}

