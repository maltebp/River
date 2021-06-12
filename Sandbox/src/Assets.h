#pragma once

#include <River.h>


namespace GlobalAssets {

	using namespace River;


	inline River::AssetCollection* COLLECTION = new River::AssetCollection();


	namespace Fonts {
		inline River::Font* ARIAL = River::Font::create("assets/arial.ttf")
			.addToCollection(COLLECTION)
			.enableSizeAutoLoading()
			.finish();
	}


	namespace Images {

		inline River::Image* CAR = River::Image::create("assets/car.png")
			.setScaleMode(River::Image::ScaleMode::NEAREST)
			.finish();

		
	}



	namespace Textures {

		inline River::Texture* CAR = River::Texture::create(Images::CAR, true)
			.addToCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* PIXEL = River::Texture::create(River::Image::create("assets/pixel.png").setScaleMode(River::Image::ScaleMode::NEAREST).finish(), true)
			.addToCollection(COLLECTION)
			.finish();

		inline Texture* SANTA = Texture::create(Image::create("assets/santa/Idle (1).png").setPartiallyTransparent(true).finish(), true).finish();

	}


	namespace SpriteAnimation {
 
		inline River::TextureAnimation* SANTA_JUMP = River::TextureAnimation::create({
				{ Image::create("assets/santa/Jump (1).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (2).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (3).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (4).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (5).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (6).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (7).png").setPartiallyTransparent(true) },
				{ Image::create("assets/santa/Jump (8).png").setPartiallyTransparent(true) }
			})
			.addToCollection(COLLECTION)
			.setDuration(0.5)
			.finish();

		inline River::TextureAnimationCollection* SANTA_ANIMATIONS = River::TextureAnimationCollection::create(
			{
				{ "jump", SpriteAnimation::SANTA_JUMP }
			}
		)
		.addToCollection(COLLECTION);

	}


	namespace Sounds {
		inline River::AudioAsset* COUNTDOWN = River::AudioAsset::create("assets/countdown.wav")
			.addToCollection(COLLECTION)
			.finish();

		inline River::AudioAsset* COINS = River::AudioAsset::create("assets/test.wav")
			.addToCollection(COLLECTION)
			.finish();

		inline River::AudioAsset* CLASSICAL_MUSIC = River::AudioAsset::create("assets/music.wav")
			//.addToCollection(COLLECTION)
			.finish();
	}



}