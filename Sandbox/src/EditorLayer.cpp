#include "EditorLayer.h"

#include <River/External/imgui/imgui.h>


using namespace River;


namespace {

    const Resolution WINDOW_DEFAULT_RESOLUTION = { 800, 600 };

}



class EditorViewport : public River::Viewport {
public:

    
    EditorViewport()
        :   Viewport(WINDOW_DEFAULT_RESOLUTION),
            frameBuffer(createFrameBuffer(WINDOW_DEFAULT_RESOLUTION))
    { }


    static FrameBuffer* createFrameBuffer(const Resolution& resolution) {
        FrameBuffer* newFrameBuffer = new FrameBuffer();
        newFrameBuffer->addColorBuffer(resolution, 3u);
        newFrameBuffer->addDepthBuffer(resolution);
        newFrameBuffer->build();
        return newFrameBuffer;
    }

    
    void bindFrameBuffer() override {
        frameBuffer->bind();
    }

    
    void unbindFrameBuffer() override {
        frameBuffer->unbind();
    }


    void setResolution(const Resolution& resolution) {
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


    FrameBuffer* frameBuffer = nullptr;

};



EditorLayer::EditorLayer() {
    windowResolution = WINDOW_DEFAULT_RESOLUTION;
    viewport = new EditorViewport();
    gameLayer = pushLayer<MainLayer>(viewport);
}


void EditorLayer::onUpdate() {
        viewport->setResolution(windowResolution);
}


void EditorLayer::onUpdateImGui() {

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            ImGui::Separator();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::DockSpaceOverViewport();

    // Scene Tool View
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });

        ImGui::Begin("Scene", NULL);
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();

        ImVec2 contentSize = { max.x - min.x, max.y - min.y };

        windowResolution = { (unsigned int)contentSize.x, (unsigned int)contentSize.y };

        ImGui::Image(
            (void*)(intptr_t)viewport->frameBuffer->getColorBufferImage(0),
            contentSize
        );

        ImGui::End();

        ImGui::PopStyleVar(); 	
    }

}