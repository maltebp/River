#pragma once

#include "River/Graphics/ResolutionEvent.h"
#include "River/Graphics/FrameBuffer.h"


namespace River {


    class Viewport {
    public:

        virtual void bindFrameBuffer() = 0;

        virtual void unbindFrameBuffer() = 0;

        Resolution getResolution() const;   

        ListenerMap<ResolutionEvent&> resolutionChangedListeners;

    protected:

        Viewport(const Resolution& initialResolution = Resolution(0,0) );

        void setResolution(const Resolution& resolution);

    private:

        Resolution resolution;

        ListenerInvoker<ResolutionEvent&> resolutionChangedInvoker = resolutionChangedListeners;

    };


}