
#include "dsHUDWindow.h"

dsHUDWindow::Point::Point(float x, float y, float width, float height, float alpha) {
    this->x = x;
    this->y = y;
    this->w = width;
    this->h = height;
    this->a = alpha;
}

float dsHUDWindow::Point::GetX() const {
    return x;
}

float dsHUDWindow::Point::GetY() const {
    return y;
}
float dsHUDWindow::Point::GetWidth() const {
    return w;
}
float dsHUDWindow::Point::GetHeight() const {
    return h;
}
float dsHUDWindow::Point::GetAlpha()const {
    return a;
}

dsHUDWindow::Point dsHUDWindow::Point::Interpolate(const dsHUDWindow::Point & a, const dsHUDWindow::Point & b, float t) {
    return dsHUDWindow::Point(a.x * (1-t) + b.x * t,
        a.y * (1 - t) + b.y * t,
        a.w * (1 - t) + b.w * t,
        a.h * (1 - t) + b.h * t,
        a.a * (1 - t) + b.a * t);
}

dsHUDWindow::dsHUDWindow(DS::Data * data) : DS::Stage(data) {
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    points.reserve(10);
}

dsHUDWindow::~dsHUDWindow() {
   
}

void dsHUDWindow::Load() {
    
}

void dsHUDWindow::Render(int64_t start, int64_t end, int64_t time) {
    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    const float aspect = m_Data->GetAspect();
    const float width = m_Data->GetWidth();
    const float height = m_Data->GetHeight();
    const float t = (time - start) / 1.0e6;

    dev->Viewport(0,0, width, height);

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();

    dev->Ortho2D(m_Data->GetWidth(), m_Data->GetHeight());

    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    dev->Disable(Graph::STATE_ZBUFFER_WRITE);
    dev->Disable(Graph::STATE_DEPTH_TEST);
    dev->Disable(Graph::STATE_CULL_FACE);

    float a = 0.0;

    if (points.size() > 1) {
        if (t > points[points.size() - 1].first)
        {
            m_x = points[points.size() - 1].second.GetX();
            m_y = points[points.size() - 1].second.GetY();
            m_width = points[points.size() - 1].second.GetWidth();
            m_height = points[points.size() - 1].second.GetHeight();
            a = points[points.size() - 1].second.GetAlpha();
        }
        else if (t < points[0].first)
        {
            m_x = points[0].second.GetX();
            m_y = points[0].second.GetY();
            m_width = points[0].second.GetWidth();
            m_height = points[0].second.GetHeight();
            a = points[0].second.GetAlpha();
        }
        else {
            for (int i = 0; i < points.size() - 1; i++) {
                std::pair<float, Point> p_prev = points[i];
                std::pair<float, Point> p_next = points[i+1];

                if (t >= p_prev.first && t < p_next.first) {
                    float ai = (t - p_prev.first)/(p_next.first- p_prev.first);
                    Point pp = Point::Interpolate(p_prev.second, p_next.second, ai);
                    m_x = pp.GetX();
                    m_y = pp.GetY();
                    m_width = pp.GetWidth();
                    m_height = pp.GetHeight();
                    a = pp.GetAlpha();
                    break;
                }
            }
        }
    }
    else if (points.size() == 1) {
        m_x = points[0].second.GetX();
        m_y = points[0].second.GetY();
        m_width = points[0].second.GetWidth();
        m_height = points[0].second.GetHeight();
        a = points[0].second.GetAlpha();
    }

    float cs = 4;
    float th = 2;

    dev->PushMatrix();
    dev->Translate(m_x, m_y, 0);

    shp->Square(0, 0, m_width, m_height, Math::Color4ub(50, 50, 50,a * 200));

    Math::Color4ub borderCol(200, 200, 200, a * 255);

    // Borders
    shp->Square(th, -th/2, m_width, th, borderCol);
    shp->Square(-th/2, th, th, m_height -th *2 , borderCol);
    shp->Square(m_width -th/2, th, th, m_height - th * 2, borderCol);
    shp->Square(th, m_height -th/2, m_width, th, borderCol);

    RenderCross(0, 0, cs, a);
    RenderCross(m_width, 0, cs, a);
    RenderCross(m_width, m_height, cs, a);
    RenderCross(0, m_height, cs, a);

    dev->PopMatrix();
    

   
}

void dsHUDWindow::Add(float t, const Point & p) {
    points.push_back(std::pair<float, Point>(t, p));
}

void dsHUDWindow::RenderCross(float x, float y, float tk, float alpha) {
    float w = 20;
    float h = 20;

    Graph::Device * dev = m_Data->GetGraphicsDevice();
    Gui::ShapeRenderer * shp = m_Data->GetShapeRenderer();

    dev->PushMatrix();
    dev->Translate(x-w/2, y-h/2, 0);

    shp->Square(w/2-tk/2, 0, tk, h/2-tk/2, Math::Color4ub(255, 255, 255, 255 * alpha));
    shp->Square(0, h/2-tk/2, w, tk, Math::Color4ub(255, 255, 255, 255 * alpha));
    shp->Square(w / 2 - tk / 2, h / 2 + tk / 2, tk, h / 2 - tk / 2, Math::Color4ub(255, 255, 255, 255 * alpha));

    dev->PopMatrix();
}
