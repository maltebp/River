#pragma once

#include "River/Graphics/Viewport.h"
#include "River/Graphics/FrameBuffer.h"


namespace River {

    class Window;

    
    class MainViewport : public Viewport {
        friend Window;
    public:

        MainViewport(const Resolution& resolution);

        virtual void bindFrameBuffer() override;

        virtual void unbindFrameBuffer() override; 

        void setResolution(const Resolution& resolution);       

    private:

        FrameBuffer* frameBuffer;

    };

}