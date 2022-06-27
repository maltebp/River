
#include <River.h>

#include "MainLayer.h"


class EditorViewport;


class EditorLayer : public River::Layer {
public:

    EditorLayer();

protected:

    void onUpdate() override;

    void onUpdateImGui() override;

private:

    EditorViewport* viewport;    

    MainLayer* gameLayer;

    River::Resolution windowResolution;

};