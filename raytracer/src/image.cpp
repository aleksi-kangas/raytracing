#include "image.h"

namespace rt {
Image::Image(int32_t width, int32_t height, uint32_t* data)
    : width_{width}, height_{height}, data_{data} {
  glGenBuffers(1, &buffer_);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer_);

  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  Update();
}

void Image::Update() const {
  const GLsizeiptr kBufferSize =
      static_cast<GLsizeiptr>(width_) * static_cast<GLsizeiptr>(height_) * static_cast<GLsizeiptr>(sizeof(uint32_t));
  glBufferData(GL_PIXEL_UNPACK_BUFFER, kBufferSize, nullptr, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, kBufferSize, data_);

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
               nullptr);
}

void Image::Resize(int32_t width, int32_t height, uint32_t* data) {
  if (texture_ && width_ == width && height_ == height) return;
  width_ = width;
  height_ = height;
  data_ = data;
  Update();
}

}  // namespace rt
