#include "River/pch.h"

#include "TextureAnimation.h"


namespace River {

    using Creator = TextureAnimation::Creator;
    using FrameParameter = TextureAnimation::FrameParameter;


    const Texture* TextureAnimation::getFrame(unsigned int index) const {
        return frames[index].texture;
    }
    

    const Texture* TextureAnimation::getFrameAtTime(float time) const {
        
        if( time < 0 ) {
            throw InvalidArgumentException("Time must not be negative");
        }

        time = fmod(time, totalDuration);

        float durationSum = 0;

        for( const FrameData& frame : frames ) {
            // Branching could be avoided, but not worth it yet
            durationSum += frame.duration;
            if( time < durationSum ) return frame.texture;
        }

        /* Notes on this lookup:
            * This "calculation" has to happen each frame, for each AnimatedSprite component
            * active in the system. However, since the list is expected to be relatively small
            * and it's sequential memory (vector), I expect it has a very small performance impact,
            * compared to the complexity of implementing a faster lookup system.
        */

        return nullptr;
    }
 

    float TextureAnimation::getFrameDuration(unsigned int frameIndex) const {
        if( frameIndex >= frames.size() ){
            throw new InvalidArgumentException("Frame index is out of bounds");
        }
        return frames[frameIndex].duration;
    }


    float TextureAnimation::getTotalDuration() const {
        return totalDuration;
    }


    unsigned int TextureAnimation::getNumFrames() const {
        return (unsigned int)frames.size();
    }


    void TextureAnimation::onLoad() {
        for( FrameData& frame : frames ) {
            frame.texture->load();
        }
    }


    void TextureAnimation::onUnload() {
        for( FrameData& frame : frames ) {
            frame.texture->unload();
        }
    }


    void TextureAnimation::recalculateTotalDuration() {
        totalDuration = 0;
        for( FrameData& frame : frames ){
            totalDuration += frame.duration;
        }
    }


    FrameParameter::FrameParameter(Texture* texture) 
        :   texture(texture),
            dedicatedTexture(false)
    { }

    
    FrameParameter::FrameParameter(Texture::Creator& textureCreator)
        :   texture(textureCreator.finish()),
            dedicatedTexture(true)
    { }


    FrameParameter::FrameParameter(Image::Creator& imageCreator) 
        :   texture(Texture::create(imageCreator.finish(), true).finish()),
            dedicatedTexture(true)
    { }


    Creator::Creator(std::vector<FrameParameter> frameParameters) {
        
        asset = new TextureAnimation();

        if( frameParameters.size() == 0 ) {
            throw new InvalidArgumentException("Animation frames must contain at least one frame");
        }
        
        for( FrameParameter& frameParameter : frameParameters ) {

            if( frameParameter.texture == nullptr) {
                throw new InvalidArgumentException("Frame Texture source must not be nullptr");
            }

            asset->frames.push_back({
                frameParameter.texture, 
                TextureAnimation::DEFAULT_FRAME_LENGTH, 
                frameParameter.dedicatedTexture
            });
        }

    }

    
    Creator& Creator::setDuration(float duration) {
        
        if( duration <= 0 ){
            throw new InvalidArgumentException("Animation duration of TextureAnimation must be larger than 0");
        }

        float frameDuration = duration / asset->frames.size();
        
        for( FrameData& frame : asset->frames ){
            frame.duration = frameDuration;
        }

        return *this;
    }    


    Creator& Creator::setFrameDuration(unsigned int frameIndex, float duration) {
        
        if( duration <= 0 ){
            throw new InvalidArgumentException("Animation duration of TextureAnimation must be larger than 0");
        }

        if( frameIndex >= asset->frames.size() ) {
            throw new InvalidArgumentException("Frame index is out of bounds");
        }

        asset->frames[frameIndex].duration = duration;

        return *this;	
    }   


    void Creator::onFinish() {
        asset->recalculateTotalDuration();
    }


    Creator::operator TextureAnimation*() {
        return finish();
    }


    Creator TextureAnimation::create(std::vector<FrameParameter> frames) {
        return Creator(frames);
    }


}