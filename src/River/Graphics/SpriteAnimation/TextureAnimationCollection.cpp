#include "River/pch.h"

#include "TextureAnimationCollection.h"


namespace River {

    
    using Creator = TextureAnimationCollection::Creator;
    using Mapping = TextureAnimationCollection::Mapping;


    void TextureAnimationCollection::onLoad() {
        for( auto [key, animation] : animations ) {
            animation->load();
        }
    }

    
    void TextureAnimationCollection::onUnload() {
        for( auto [key, animation] : animations ) {
            animation->unload();
        }
    }


    TextureAnimation* TextureAnimationCollection::getAnimation(const std::string& key) {
        auto iterator = animations.find(key);
        
        if ( iterator == animations.end() ) {
            return nullptr;
        } else {
            return iterator->second;
        }       
    }
    

    Creator::Creator(std::vector<Mapping> animations) {
        
        asset = new TextureAnimationCollection();

        if( animations.size() == 0 ) {
            throw new InvalidArgumentException("Animation collection must contain at least 1 animation");
        }
        
        for( Mapping mapping : animations ) {
            
            if( mapping.animation == nullptr ) {
                throw new InvalidArgumentException("Animation must not be nullptr");
            }

            if( mapping.key.empty() ) {
                throw new InvalidArgumentException("Animation key not be empty");
            }

            if( asset->animations.find(mapping.key) != asset->animations.end() ) {
                throw new InvalidArgumentException("Animation with key already exists in collection");
            };

            asset->animations[mapping.key] = mapping.animation;
        }   

    }

}