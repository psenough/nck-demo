#ifndef DS_HUD_WINDOW_H
#define DS_HUD_WINDOW_H

#include "../dsStage.h"

class dsHUDWindow : public DS::Stage {
public:
    dsHUDWindow(DS::Data * data);
    virtual ~dsHUDWindow();
    void Load();
    void Render(int64_t start, int64_t end, int64_t time);
 
    static class Point {
    public:
        Point(float x, float y, float width, float height, float alpha);
        float GetX() const;
        float GetY() const;
        float GetWidth() const;
        float GetHeight() const;
        float GetAlpha() const;
        static Point Interpolate(const Point & a, const Point & b, float t);
    private:
        float x, y, w, h, a;
    };

    void Add(float t, const Point & p);

protected:
    std::vector<std::pair<float,Point>> points;
    void RenderCross(float x, float y, float tk, float alpha);
    float m_x, m_y, m_width, m_height;
};

#endif