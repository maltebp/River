#pragma once

#include <River.h>


namespace GlobalAssets {

	inline River::AssetCollection* COLLECTION = new River::AssetCollection();;

	namespace Images {

	}

	namespace Textures {

		inline River::Texture* SANTA_JUMP_1 = River::Texture::create(River::Image::create("assets/santa/Jump (1).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_2 = River::Texture::create(River::Image::create("assets/santa/Jump (2).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_3 = River::Texture::create(River::Image::create("assets/santa/Jump (3).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_4 = River::Texture::create(River::Image::create("assets/santa/Jump (4).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_5 = River::Texture::create(River::Image::create("assets/santa/Jump (5).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_6 = River::Texture::create(River::Image::create("assets/santa/Jump (6).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_7 = River::Texture::create(River::Image::create("assets/santa/Jump (7).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

		inline River::Texture* SANTA_JUMP_8 = River::Texture::create(River::Image::create("assets/santa/Jump (8).png").finish(), true)
			.setAssetCollection(COLLECTION)
			.setPartiallyTransparent()
			.finish();

	}



}