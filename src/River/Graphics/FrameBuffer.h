#pragma once

#include "River/Graphics/GL.h"
#include "River/Primitives/Resolution.h"
#include "River/Graphics/Texture/Image.h"


namespace River {

    
    class FrameBuffer {
    public:

        enum class State {
            NEW,
            UNBOUND,
            BOUND,
            CURRENT
        };

    public:

        FrameBuffer();

        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;

        unsigned int addColorBuffer(
            const Resolution& resolution,
            unsigned int numChannels = 4,
            Image::ScaleMode scaleMode = Image::ScaleMode::LINEAR,
            Image::WrapMode wrapMode = Image::WrapMode::NONE
        );

        /**
         * @brief	Adds an additional color buffer to this framebuffer
         * 
         * @return  The index of the color buffer. This is just the number of buffers created before
         *          this color buffer (first buffer is 0, second is 1 and so forth)
         * 
         * @throws  River::InvalidStateException    If too many color buffers has already been added,
         *                                          or of the FrameBuffer has already been built
         */
        unsigned int addColorBuffer(
            const Resolution& resolution,
            unsigned int numChannels,
            Image::ScaleMode scaleDownMode, Image::ScaleMode scaleUpMode,
            Image::WrapMode wrapModeHorizontal, Image::WrapMode wrapModeVertical
        );

        void addDepthBuffer(
            const Resolution& resolution,
            Image::ScaleMode scaleMode = Image::ScaleMode::LINEAR,
            Image::WrapMode wrapMode = Image::WrapMode::NONE
        );

        /**
         * @brief	Adds a depth buffer to this FrameBuffer. A FrameBuffer may only have one depth
         *          buffer.
         * 
         * @throws  River::InvalidStateException    If the FrameBuffer already has a depth buffer,
         *                                          or of the FrameBuffer has already been built
         */
        void addDepthBuffer(
            const Resolution& resolution,
            Image::ScaleMode scaleDownMode,
            Image::ScaleMode scaleUpMode,
            Image::WrapMode wrapModeHorizontal,
            Image::WrapMode wrapModeVertical
        );

        /**
         * @brief	FrameBuffer must be be built before being used (calling use())
         * 
         */
        void build();

        /**
         * @brief   Pushes this FrameBuffer to the top of the stack, causing it to be bound for
         *          usage, and thus unbinding current FrameBuffer (or default). This does not clear
         *          any of the buffers, so this should be done manually.
         * 
         * @throws  River::InvalidStateException    If the FrameBuffer is already in the stack
         */
        void bind();

        /**
         * @brief	Pops this FrameBuffer from the stack, causing the next one on the stack to
         *          be bound for usage if it exists, or otherwise enable the default.
         * 
         * @throws River::InvalidStateException     If this FrameBuffer has not been built, or if
         *                                          this FrameBuffer is not on the top of the stack
         */
        void unbind();

        State getState();

        /**
         * @brief   Fetches the Image containing the data of the color buffer with the given index
         * 
         * @param index Index of the color buffer
         * 
         * @return  GL ID on the depth texture buffer, or 0 if the color buffer with the given index
         *          does not exist
         */
        GLuint getColorBufferImage(unsigned int index);

        /**
         * @brief   Fetches the Image containing the data of the depth buffer with the given index
         * 
         * @param index Index of the color buffer
         * 
         * @return  GL ID on the depth texture buffer 
         */
        GLuint getDepthBufferImage();

        FrameBuffer& operator=(const FrameBuffer&) = delete;

        static FrameBuffer* getCurrent();

    private:

        GLuint id;

        State state = State::NEW;

        std::vector<GLuint> colorBuffers;

        GLuint depthBuffer = 0;

        static inline std::unordered_map<GLuint, FrameBuffer*> frameBufferMap;

        static inline std::vector<FrameBuffer*> frameBufferStack;

    };


}	