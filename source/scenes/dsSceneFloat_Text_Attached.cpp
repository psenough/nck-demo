
#include "dsSceneFloat_Text_Attached.h"
#include "nckCamera.h"
#include "../dsUtils.h"


dsSceneFloat_Text_Attached::dsSceneFloat_Text_Attached(DS::Data * data, dsSceneMap * map) : dsSceneFloat_Text(data){
    this->map = map;
}

dsSceneFloat_Text_Attached::~dsSceneFloat_Text_Attached() {
  
}

void dsSceneFloat_Text_Attached::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    const float w = 1920;
    const float h = 1920 * m_Data->GetHeight() / m_Data->GetWidth();

    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
   
    dev->Ortho2D(w, h);

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
 
    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    Math::Vec2 projected = map->GetObjectPositionWithId(id);
    Math::Vec2 endPos = Math::LinearInterpolation(showPosA, showPosB, alpha);

    if (projected.GetX() > 0 && projected.GetY() > 0) {
        Math::Vec2 n = Math::Normalize(endPos - projected);

        Math::Vec2 a = projected + n * 50;
        Math::Vec2 b = endPos - n * 50;

        dev->Color(255, 0, 0, 255);
        dev->Begin(Graph::PRIMITIVE_LINES);
        dev->Vertex(a.GetX(), a.GetY());
        dev->Vertex(b.GetX(), b.GetY());
        dev->End();
    }

    dsSceneFloat_Text::Render(start, end, time);
}

void dsSceneFloat_Text_Attached::setId(const std::string & id) {
    this->id = id;
}