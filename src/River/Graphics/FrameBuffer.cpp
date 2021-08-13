#include "River/pch.h"

#include "FrameBuffer.h"

#include "Window.h"


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
        GL(glDeleteTextures((GLsizei)colorBuffers.size(), colorBuffers.data()));
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
            resolution.width, resolution.height, 
            0, 
            GL_RGB, // This has no effect when we are not passing any data 
            GL_UNSIGNED_BYTE, // This has no effect, when not passing any data 
            nullptr // We are not passing any data
        ));

		GL(glBindTexture(GL_TEXTURE_2D, currentTexture));

        if( colorBuffers.size() == 1 ) {
            defaultRenderAreaSize = resolution;
        }

        return (unsigned int)colorBuffers.size() - 1;
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
            resolution.width, resolution.height, 
            0, 
            GL_DEPTH_COMPONENT, // This has no effect when we are not passing any data 
            GL_UNSIGNED_INT, // This has no effect, when not passing any data 
            nullptr // We are not passing any data
        ));

		GL(glBindTexture(GL_TEXTURE_2D, currentTexture));

        if( colorBuffers.size() == 0 ) {
            defaultRenderAreaSize = resolution;
        }
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


    void FrameBuffer::setRenderArea(dvec2 position, Resolution size) {
        renderAreaPosition = position;
        renderAreaSize = size;

        if( state == State::CURRENT ) {
            useRenderArea();
        }
    }


    void FrameBuffer::bind() {
        if( state == State::NEW ) {
            throw new InvalidStateException("FrameBuffer has not been built");
        }
        bindBefore(bindingStack.end());
    }


    void FrameBuffer::unbind() {

        if( state != State::CURRENT && state != State::BOUND ) {
            throw new InvalidStateException("FrameBuffer is not bound");
        }

        auto it = std::find(bindingStack.begin(), bindingStack.end(), this);
        if( it == bindingStack.end() ) {
            throw new InvalidStateException("FrameBuffer state says that it is bound, but it is not");
        }

        bindingStack.erase(it);
        state = State::UNBOUND;

        if( bindingStack.size() == 0 ) {
            Window::useRenderArea();
            GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }else{
            FrameBuffer* top = bindingStack.back();
            GL(glBindFramebuffer(GL_FRAMEBUFFER, top->id));
            top->state = State::CURRENT;
            top->useRenderArea();
        }
    }


    void FrameBuffer::switchBinding(FrameBuffer& other) {
        if( this->state == State::NEW ) {
            throw new InvalidStateException("FrameBuffer has not been built");
        }
        if( other.state == State::NEW ) {
            throw new InvalidArgumentException("FrameBuffer has not been built");
        }
        if( this == &other ) {
            throw new InvalidArgumentException("Cannot switch binding position with itself");
        }

        if( this->state == State::UNBOUND && other.state == State::UNBOUND ) return;
        
        auto findIndex = [](FrameBuffer* element){
            for( int i=0; i < bindingStack.size(); i++ ) {
                if( bindingStack[i] == element ) return i;
            }
            return -1;
        };

        int thisIndex = findIndex(this);
        int otherIndex = findIndex(&other);

        if( thisIndex >= 0 ) {
            other.bindBefore(bindingStack.begin()+thisIndex);
        }
        else {
            // 'other' must be currently bound, because 'this' was not
            other.unbind();
        }

        if( otherIndex >= 0) {
            this->bindBefore(bindingStack.begin()+otherIndex);
        }
        else {
            // 'this' must be currently bound, because 'other' was not
            this->unbind();
        }
    }


    void FrameBuffer::bindBefore(std::vector<FrameBuffer*>::iterator targetIterator) {
        // Inserts the this framebuffer BEFORE the given iterator

        // Unbind if already bound
        if( this->state == State::BOUND || this->state == State::CURRENT ) {
            auto thisIterator = std::find(bindingStack.begin(), bindingStack.end(), this);
            
            if( targetIterator == thisIterator || --targetIterator == thisIterator ) {
                // Binding this FrameBuffer at the target iterator would not change anything
                return;
            }

            auto target = *targetIterator;
            unbind();
            targetIterator = std::find(bindingStack.begin(), bindingStack.end(), this);
        } 

        bool targetIsLast = targetIterator == bindingStack.end();
        bindingStack.insert(targetIterator, this);

        if( targetIsLast ) {
            if( bindingStack.size() > 1 ) {
                (*std::prev(bindingStack.end()))->state = State::BOUND;
            }
            
            state = State::CURRENT;
            GL(glBindFramebuffer(GL_FRAMEBUFFER, id));
            useRenderArea();
        }   
        else {
            state = State::BOUND;
        }
    }

    
    State FrameBuffer::getState() {
        return state;
    }


    GLuint FrameBuffer::getColorBufferImage(unsigned int index) {
        if( index >= colorBuffers.size() ) return 0;
        return colorBuffers[index];
    }


    GLuint FrameBuffer::getDepthBufferImage() {
        return depthBuffer;
    }


    FrameBuffer* FrameBuffer::getCurrent() {
        if( bindingStack.size() == 0 ) return nullptr;
        return bindingStack.back();
    }


    void FrameBuffer::useRenderArea() {
        if( renderAreaSize != Resolution(0,0) ) {
            GL(glViewport(
                (int)renderAreaPosition.x, (int)renderAreaPosition.y,
                 (GLsizei)renderAreaSize.width, (GLsizei)renderAreaSize.height
            ));
        }
        else {
            GL(glViewport(0,0, (GLsizei)defaultRenderAreaSize.width, (GLsizei)defaultRenderAreaSize.height));
        }
    }

}