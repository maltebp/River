#include "River/pch.h"

#include "MainViewport.h"


namespace River {

    namespace {
        
        FrameBuffer* createFrameBuffer(const Resolution& resolution) {
            FrameBuffer* frameBuffer = new FrameBuffer();
            frameBuffer->addColorBuffer(resolution, 3u);
            frameBuffer->addDepthBuffer(resolution);
            frameBuffer->build();
            return frameBuffer;
        }

    }


    MainViewport::MainViewport(const Resolution& initialResolution)
        :   Viewport(initialResolution),
            frameBuffer(createFrameBuffer(initialResolution))
    { }


    // Note: This Viewport's framebuffer is just an intermediate between to
    // default framebuffer. At some we'll have to change it, such that a
    // FrameBuffer can actually be the default FrameBuffer
    void MainViewport::bindFrameBuffer() {
        frameBuffer->bind();
    }


    void MainViewport::unbindFrameBuffer() {
        frameBuffer->unbind();
    }


    void MainViewport::setResolution(const Resolution& resolution) {
        
        if( getResolution() == resolution ) return;

        if( resolution.width == 0 || resolution.height == 0 ) {
            throw new InvalidArgumentException("Resolution cannot be 0x0");
        }
        
        FrameBuffer* newFrameBuffer = createFrameBuffer(resolution);

        frameBuffer->swapBinding(*newFrameBuffer);

        delete frameBuffer;

        frameBuffer = newFrameBuffer;

        setResolutionInternal(resolution);
    }

}