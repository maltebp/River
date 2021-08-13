#include "River/pch.h"

#include "Viewport.h"


namespace River {


        Viewport::Viewport(const Resolution& initialResolution) 
            :   resolution(initialResolution)
        { }


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