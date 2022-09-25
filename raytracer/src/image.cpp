#include "image.h"

namespace rt {
Image::Image(int32_t width, int32_t height)
    : width_{width}, height_{height} {
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
}

void Image::Update(const std::vector<uint32_t>& buffer) const {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               static_cast<GLsizei>(width_),
               static_cast<GLsizei>(height_),
               0,
               GL_RGBA,
               GL_UNSIGNED_INT_8_8_8_8_REV,
               buffer.data());
}

intptr_t Image::Texture() const { return texture_; }

int32_t Image::Width() const { return width_; }

int32_t Image::Height() const { return height_; }

float Image::AspectRatio() const {
  return static_cast<float>(width_) / static_cast<float>(height_);
}

void Image::Resize(int32_t width, int32_t height) {
  if (texture_ && width_ == width && height_ == height) return;
  width_ = width;
  height_ = height;
}

}  // namespace rt
