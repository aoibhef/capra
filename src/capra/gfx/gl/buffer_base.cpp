#include "capra/gfx/gl/buffer_base.hpp"
#include "capra/util/enum_bitops.hpp"
#include "capra/util/log.hpp"

namespace capra {

BufferBase::BufferBase(GladGLContext &gl) : gl(gl) {
  gen_id_();
}

BufferBase::~BufferBase() {
  del_id_();
}

BufferBase::BufferBase(BufferBase &&other) noexcept : gl(other.gl) {
  std::swap(id, other.id);
}

BufferBase &BufferBase::operator=(BufferBase &&other) noexcept {
  if (this != &other) {
    del_id_();

    std::swap(gl, other.gl);
    std::swap(id, other.id);
  }
  return *this;
}

void BufferBase::bind(const BufTarget &target) const {
  gl.BindBuffer(unwrap(target), id);
}

void BufferBase::unbind(const BufTarget &target) const {
  gl.BindBuffer(unwrap(target), 0);
}

void BufferBase::gen_id_() {
  gl.GenBuffers(1, &id);
  CAPRA_LOG_TRACE("Generated buffer ({})", id);
}

void BufferBase::del_id_() {
  if (id != 0) {
    gl.DeleteBuffers(1, &id);
    CAPRA_LOG_TRACE("Deleted buffer ({})", id);
  }
}

} // namespace capra
