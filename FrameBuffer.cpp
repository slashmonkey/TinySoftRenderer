
#include "FrameBuffer.h"
#include "Color.h"

FrameBuffer::FrameBuffer(int _width, int _height) : width(_width), height(_height) {
    color_buf.resize(width * height);
    depth_buf.resize(width * height);
}

FrameBuffer::~FrameBuffer() {}

void FrameBuffer::clear(Buffers buffers) {
    if((buffers & Buffers::Color) == Buffers::Color){
        std::fill(color_buf.begin(), color_buf.end(), Black);
    }
    if((buffers & Buffers::Depth) == Buffers::Depth){
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

void FrameBuffer::set_pixel(int ind, const Color& color) {
    color_buf[ind] = color;
}

Color FrameBuffer::get_color(int ind) const {
    return color_buf[ind];
}

void FrameBuffer::set_depth(int ind, float depth) {
    depth_buf[ind] = depth;
}

float FrameBuffer::get_depth(int ind) {
    return depth_buf[ind];
}
