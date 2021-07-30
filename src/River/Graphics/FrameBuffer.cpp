#include "River/pch.h"

#include "FrameBuffer.h"


namespace River {


    using State = FrameBuffer::State;    
    using ScaleMode = Image::ScaleMode;
    using WrapMode = Image::WrapMode;

    
    FrameBuffer::FrameBuffer() {
    }


    FrameBuffer::~FrameBuffer() {
        if( state == State::BOUND || state == State::CURRENT ) {
            unbind();
        }

        GL(glDeleteFramebuffers(1, &id));
        GL(glDeleteTextures(1, &depthBuffer));
        GL(glDeleteTextures(colorBuffers.size(), colorBuffers.data()));
    }


    unsigned int FrameBuffer::addColorBuffer( 
        const Resolution& resolution,
        unsigned int channels,
        ScaleMode scaleMode,
        WrapMode wrapMode
    ) {
        return addColorBuffer(resolution, channels, scaleMode, scaleMode, wrapMode, wrapMode);
    }


    unsigned int FrameBuffer::addColorBuffer(
        const Resolution& resolution,
        unsigned int channels,
        ScaleMode scaleDownMode, ScaleMode scaleUpMode,
        WrapMode wrapModeHorizontal, WrapMode wrapModeVertical
    ) {

        if( state != State::NEW ) {
            throw new InvalidStateException("FrameBuffer has already been built");
        }

        // Note: More than 8 FrameBuffer color buffer attachments may be supported, but
        // it is not guaranteed. To avoid having to deal with a varying amount we hard
        // code this to 8 instead, which IS guaranteed by the OpenGL standard.
        if( colorBuffers.size() >= 8 ) {
            throw new InvalidStateException("FrameBuffer already has 8 color buffers (which is max)");
        }

        if( resolution.width == 0 || resolution.height == 0 ) {
            throw new InvalidArgumentException("Width and height of resolution of FrameBuffer's color buffer must be larger than 0");
        }

        GLint currentTexture;
        GL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture));

        GLuint textureId;
		GL(glGenTextures(1, &textureId));
		GL(glBindTexture(GL_TEXTURE_2D, textureId));
        colorBuffers.push_back(textureId);

        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Image::toGLScaleMode(scaleDownMode)));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Image::toGLScaleMode(scaleUpMode)));

		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Image::toGLWrapMode(wrapModeHorizontal)));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Image::toGLWrapMode(wrapModeVertical)));		

        GLint internalFormat;
        switch(channels) {
            // Note: We only allow 1 byte colors
            case 1: internalFormat = GL_R8;       break;
            case 2: internalFormat = GL_RG8;      break;
            case 3: internalFormat = GL_RGB8;     break;
            case 4: internalFormat = GL_RGBA8;    break;
            default:
                throw new River::InvalidArgumentException("Invalid number of channels for FrameBuffer color buffer");
        }

		// Set data
		GL(glTexImage2D(
            GL_TEXTURE_2D,
            0, 
            internalFormat,
            resolution.width, resolution.width, 
            0, 
            GL_RGB, // This has no effect when we are not passing any data 
            GL_UNSIGNED_BYTE, // This has no effect, when not passing any data 
            nullptr // We are not passing any data
        ));

		GL(glBindTexture(GL_TEXTURE_2D, currentTexture));
    }


    void FrameBuffer::addDepthBuffer(
        const Resolution& resolution,
        ScaleMode scaleMode,
        WrapMode wrapMode
    ) {
        addDepthBuffer(resolution, scaleMode, scaleMode, wrapMode, wrapMode);
    }


    void FrameBuffer::addDepthBuffer(
        const Resolution& resolution,
        ScaleMode scaleDownMode,
        ScaleMode scaleUpMode,
        WrapMode wrapModeHorizontal,
        WrapMode wrapModeVertical
    ) {
        
        if( state != State::NEW ) {
            throw new InvalidStateException("FrameBuffer has already been built");
        }

        if( depthBuffer != 0 ) {
            throw new InvalidStateException("FrameBuffer already has a depth buffer");
        }

        if( resolution.width == 0 || resolution.height == 0 ) {
            throw new InvalidArgumentException("Width and height of resolution of FrameBuffer's color buffer must be larger than 0");
        }

        GLint currentTexture;
        GL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture));

		GL(glGenTextures(1, &depthBuffer));
		GL(glBindTexture(GL_TEXTURE_2D, depthBuffer));

        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Image::toGLScaleMode(scaleDownMode)));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Image::toGLScaleMode(scaleUpMode)));

		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Image::toGLWrapMode(wrapModeHorizontal)));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Image::toGLWrapMode(wrapModeVertical)));		

		// Set data
		GL(glTexImage2D(
            GL_TEXTURE_2D,
            0, 
            GL_DEPTH_COMPONENT32,
            resolution.width, resolution.width, 
            0, 
            GL_DEPTH_COMPONENT, // This has no effect when we are not passing any data 
            GL_UNSIGNED_INT, // This has no effect, when not passing any data 
            nullptr // We are not passing any data
        ));

		GL(glBindTexture(GL_TEXTURE_2D, currentTexture));
    }


    void FrameBuffer::build() {       

        if( state != State::NEW ) {
            throw new InvalidStateException("FrameBuffer has already been built");
        }

        if( colorBuffers.size() == 0 && depthBuffer == 0 ) {
            throw new InvalidStateException("FrameBuffer must have at least one buffer");
        }

        GLint currentBuffer;
        GL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentBuffer));

        GL(glGenFramebuffers(1, &id));
        GL(glBindFramebuffer(GL_FRAMEBUFFER, id));

        for( int i = 0; i < colorBuffers.size(); i++ ) {
            GL(glFramebufferTexture2D(
                GL_FRAMEBUFFER, 
                GL_COLOR_ATTACHMENT0 + i, 
                GL_TEXTURE_2D, 
                colorBuffers[i], 
                0
            ));
        }

        if( depthBuffer != 0 ) {
            GL(glFramebufferTexture2D(
                GL_FRAMEBUFFER, 
                GL_DEPTH_ATTACHMENT, 
                GL_TEXTURE_2D, 
                depthBuffer, 
                0
            ));
        }

        GLenum status = GL(glCheckFramebufferStatus(GL_FRAMEBUFFER));
        switch(status) {
            case 0:
                throw new Exception("FrameBuffer build error: Unknown error");
            case GL_FRAMEBUFFER_UNDEFINED:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_UNDEFINED");
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
            case GL_FRAMEBUFFER_UNSUPPORTED:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_UNSUPPORTED");
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                throw new Exception("FrameBuffer build error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
            case GL_FRAMEBUFFER_COMPLETE:
                break;
            default:
                throw new Exception("Unhandled Framebuffer build status");
        }
                
        GL(glBindFramebuffer(GL_FRAMEBUFFER, currentBuffer));

        state = State::UNBOUND;        
    }


    void FrameBuffer::bind() {

        if( state == State::NEW ) {
            throw new InvalidStateException("FrameBuffer has not been built");
        }

        auto it = std::find(bindingStack.begin(), bindingStack.end(), this);
        if( it != bindingStack.end() ) {
            bindingStack.erase(it);
        }

        if( bindingStack.size() > 0 ) {
            bindingStack.back()->state = State::BOUND;
        }

        state = State::CURRENT;
        bindingStack.push_back(this);
        
        GL(glBindFramebuffer(GL_FRAMEBUFFER, id));
    }


    void FrameBuffer::unbind() {

        auto it = std::find(bindingStack.begin(), bindingStack.end(), this);
        if( it == bindingStack.end() ) {
            throw new InvalidStateException("FrameBuffer is not bound");
        }

        bindingStack.pop_back();
        state = State::UNBOUND;

        if( bindingStack.size() == 0 ) {
            GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }else{
            FrameBuffer* top = bindingStack.back();
            GL(glBindFramebuffer(GL_FRAMEBUFFER, top->id));
            top->state = State::CURRENT;
        }
    }

    State FrameBuffer::getState() {
        return state;
    }

    FrameBuffer* FrameBuffer::getCurrent() {
        if( bindingStack.size() == 0 ) return nullptr;
        return bindingStack.back();
    }

}