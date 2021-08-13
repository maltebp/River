#pragma once

#include "River/Graphics/ResolutionEvent.h"
#include "River/Graphics/FrameBuffer.h"


namespace River {


    class Viewport {
    public:

        void bindFrameBuffer();

        void unbindFrameBuffer();

        Resolution getResolution() const;   

        ListenerMap<ResolutionEvent&> resolutionChangedListeners;

    protected:

        Viewport(const Resolution& initialResolution, FrameBuffer* frameBuffer);

        void setResolution(const Resolution& resolution);

        void setFrameBuffer(FrameBuffer* frameBuffer);

    private:

        Resolution resolution;

        FrameBuffer* frameBuffer;

        ListenerInvoker<ResolutionEvent&> resolutionChangedInvoker = resolutionChangedListeners;

    };


}