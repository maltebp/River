#pragma once

#include <River.h>


namespace GlobalAssets {

	using namespace River;


	inline AssetCollection* COLLECTION = new AssetCollection();


	namespace Fonts {
		inline Font* ARIAL = Font::create("assets/arial.ttf")
			.addToCollection(COLLECTION)
			.enableSizeAutoLoading();
	}


	namespace Images {

		inline Image* CAR = Image::create("assets/car.png")
			.setScaleMode(Image::ScaleMode::NEAREST);

		inline Image* LLAMA = Image::create("assets/llama.jpg")
			.setScaleMode(Image::ScaleMode::NEAREST)
			.addToCollection(COLLECTION);

	}



	namespace Textures {

		inline Texture* CAR = Texture::create(Images::CAR)
			.addToCollection(COLLECTION);

		inline Texture* PIXEL = Texture::create(
				Image::create("assets/pixel2.png")
				.setScaleMode(Image::ScaleMode::NEAREST)
			)
			.addToCollection(COLLECTION);

		inline Texture* SANTA = Texture::create(
				Image::create("assets/santa/Idle (1).png")
				.setPartiallyTransparent(true
			)).addToCollection(COLLECTION);
		;

	}


	namespace SpriteAnimation {
 
		inline TextureAnimation* SANTA_JUMP = TextureAnimation::create({
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
			.setDuration(0.5);

		inline TextureAnimationCollection* SANTA_ANIMATIONS = TextureAnimationCollection::create(
			{
				{ "jump", SpriteAnimation::SANTA_JUMP }
			}
		)
		.addToCollection(COLLECTION);

	}


	namespace Sounds {
		inline AudioAsset* COUNTDOWN = AudioAsset::create("assets/countdown.wav")
			.addToCollection(COLLECTION);

		inline AudioAsset* COINS = AudioAsset::create("assets/test.wav")
			.addToCollection(COLLECTION);

		inline AudioAsset* CLASSICAL_MUSIC = AudioAsset::create("assets/music.wav")
			.addToCollection(COLLECTION);
	}



}