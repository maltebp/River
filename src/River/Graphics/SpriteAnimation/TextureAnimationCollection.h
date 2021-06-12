#pragma once

#include "TextureAnimation.h"
#include "River/Asset/Asset.h"
#include "River/Asset/AssetCollection.h"
#include "River/Asset/AssetCreator.h"
#include "River/Error.h"


namespace River {

	
	/**
	 * @brief	Holds mappings from keys to of TextureAnimations
	 */
	class TextureAnimationCollection : public Asset {
	public:
		
        ~TextureAnimationCollection() = default;

        TextureAnimation* getAnimation(const std::string& key);

    protected:

        void onLoad() override;

        void onUnload() override;

	private:

		TextureAnimationCollection() {}

		TextureAnimationCollection(const TextureAnimationCollection&) = delete;

		TextureAnimationCollection& operator=(const TextureAnimationCollection&) = delete;


	private:

        std::unordered_map<std::string, TextureAnimation*> animations;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Creator
	public:

        struct Mapping {

            std::string key;

            TextureAnimation* animation;

        };

		
		class Creator : public AssetCreator<Creator, TextureAnimationCollection> {
            friend TextureAnimationCollection;
		public:

			operator River::TextureAnimationCollection * () {
				return finish();
			}

        private:

            Creator(std::vector<Mapping> animations);

		};


		/**
		 * @brief	Defines a new AnimationCollection from a series of mappings from unique
		 * 			string keys to 
		 * 
		 * @param animations Vector of <string, TextureAnimation> pairs. Must not be empty,
		 * 					 and each string key must be unique
		 */
		static Creator create(std::vector<Mapping> animations) {
			return Creator(animations);
		}


	};
}


