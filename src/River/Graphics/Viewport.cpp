#include "River/pch.h"

#include "Viewport.h"


namespace River {


        Viewport::Viewport(const Resolution& initialResolution, FrameBuffer* frameBuffer) 
            :   resolution(initialResolution),  
                frameBuffer(frameBuffer)
        {
            if( frameBuffer == nullptr ) {
                throw new InvalidArgumentException("FrameBuffer must not be null");
            }  

            if( frameBuffer->getState() == FrameBuffer::State::NEW ) {
                throw new InvalidArgumentException("FrameBuffer must be built");
            }
        }


        void Viewport::bindFrameBuffer() {
            frameBuffer->bind();
        }


        void Viewport::unbindFrameBuffer() {
            frameBuffer->unbind();
        }


        void Viewport::setFrameBuffer(FrameBuffer* frameBuffer) {
            if( frameBuffer == this->frameBuffer ) return;

            if( frameBuffer == nullptr ) {
                throw new InvalidArgumentException("FrameBuffer must not be null");
            }  

            if( frameBuffer->getState() == FrameBuffer::State::NEW ) {
                throw new InvalidArgumentException("FrameBuffer must be built");
            }
            
            // Not entirely sure if it is good idea to switch binding here, if it should
            // be up to the viewport inheritor
            frameBuffer->switchBinding(*(this->frameBuffer));
            this->frameBuffer = frameBuffer;
        }


        void Viewport::setResolution(const Resolution& resolution) {
            bool resolutionChanged = resolution != this->resolution;
            if( !resolutionChanged ) return;

            this->resolution = resolution;

            ResolutionEvent event = resolution;
            for( auto& [handle, listener] : resolutionChangedInvoker.listeners ) {
                listener(event);
                if( event.isConsumed() ) break;
            }
        }


        Resolution Viewport::getResolution() const {
            return resolution;
        }       

}