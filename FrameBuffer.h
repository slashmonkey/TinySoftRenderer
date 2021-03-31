
#ifndef SOFTRENDERER_FRAMEBUFFER_H
#define SOFTRENDERER_FRAMEBUFFER_H

#include <vector>

enum class Buffers{
    Color,
    Depth
};

inline Buffers operator|(Buffers a, Buffers b)
{
    return Buffers((int)a | (int)b);
}

inline Buffers operator&(Buffers a, Buffers b)
{
    return Buffers((int)a & (int)b);
}

struct Color;

class FrameBuffer {
public:
    FrameBuffer(int _width, int _height);
    ~FrameBuffer();

    Color get_color(int ind) const;
    void set_pixel(int ind, const Color& color);

    Color* get_color_buffer() { return color_buf.data(); }

    int get_width() const { return width; }
    int get_height() const { return height; };

    void clear(Buffers buffers);

private:
    int width;
    int height;
    std::vector<Color> color_buf;
    std::vector<float> depth_buf;
};


#endif //SOFTRENDERER_FRAMEBUFFER_H
