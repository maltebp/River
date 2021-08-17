#include "River/pch.h"

#include "Game.h"

#include "Graphics/GL.h"
#include "River/External/imgui/imgui.h"
#include "Audio/AL.h"
#include "Audio/AudioPlayer.h"
#include "Graphics/FrameBuffer.h"
#include "Keyboard/KeyboardController.h"
#include "River/Mouse/MouseController.h"


namespace River {


	void Game::start() {
		Game::start(nullptr);
	}


	void Game::start(std::function<void()> onStart) {

		if( started )
			throw new InvalidStateException("Game has already been started");

		Window::open();
	

		//std::cout << "GLEW initialized" << std::endl;
		//std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		//std::cout << "Graphics card: " << glGetString(GL_RENDERER) << std::endl;

		// Initialize audio system
		AL::initialize();

		FrameBuffer* frameBuffer = nullptr;

		if( onStart != nullptr ) {
			onStart();
		}

		while( !Window::shouldClose() ) {

			Resolution sceneViewSize = {500, 500};
			bool sceneViewSizeChanged = true;

			Window::update(
				[&](){
					
					if(editorMode) {

						if (frameBuffer == nullptr){

							if (frameBuffer != nullptr) {
								delete frameBuffer;
							}

							frameBuffer = new FrameBuffer();
							frameBuffer->addColorBuffer(sceneViewSize);
							frameBuffer->addDepthBuffer(sceneViewSize);
							frameBuffer->build();	
						}

						frameBuffer->bind();
					}

					rootLayer->clean();

					// Note on events:
					// A button press from the mouse is not registered as pressed
					// in the same event cycle as a Keyboard event. However, it
					// will be available in the next cycle, and for now it shouldn't
					// cause a problem

					Window::invokeEvents();

					// TODO: Use correct time here
					AudioPlayer::updatePlayers(0.0166666666666);		

					KeyboardController::invokeEvents();

					MouseController::invokeEvents();

					rootLayer->update();	

					if(editorMode) {
						frameBuffer->unbind();
					}	
				},
				[&]() {

					if (editorMode) {
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

							Resolution newSize = { (unsigned int)contentSize.x, (unsigned int)contentSize.y };
							sceneViewSizeChanged = newSize != sceneViewSize;
							sceneViewSize = newSize;

							ImGui::Image(
								(void*)(intptr_t)frameBuffer->getColorBufferImage(0),
								contentSize
							);

							ImGui::End();

							ImGui::PopStyleVar(); 	
						}

						ImGui::Text("Hello world!");
						
					}
				}
			);		
		}

	}


	void Game::removeLayer(Layer* layer) {
		rootLayer->removeLayer(layer);
	}


	void Game::clearLayers() {
		rootLayer->clearLayers();
	}


	void Game::exit() {
		Window::close();
	}

	
	void Game::enableEditorMode() {
		if( started ) {
			throw new InvalidStateException("Cannot enable editor mode when game is already started");
		}
		
		editorMode = true;
	}

	bool Game::isInEditorMode() {
		return editorMode;
	}
	
}