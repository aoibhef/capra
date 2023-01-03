#pragma once

#define GLFW_INCLUDE_NONE
#include "capra/core/module/module.hpp"
#include "capra/core/module/window.hpp"
#include "capra/gfx/gl/framebuffer.hpp"
#include "capra/gfx/gl/shader.hpp"
#include "capra/gfx/gl/static_buffer.hpp"
#include "capra/gfx/gl/vertex_array.hpp"
#include "capra/gfx/color.hpp"
#include "capra/util/enum_bitops.hpp"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "implot.h"
#include <concepts>
#include <memory>

namespace capra {

enum class ClearBit {
  stencil = GL_STENCIL_BUFFER_BIT,
  color = GL_COLOR_BUFFER_BIT,
  depth = GL_DEPTH_BUFFER_BIT
};

} // namespace capra

ENABLE_BITOPS(capra::ClearBit);

namespace capra {

class GLContext : public Module {
public:
  GLContext();

  void initialize_for_window(const Window &window);

  void clear(const RGB &color, const ClearBit &bit = ClearBit::color | ClearBit::depth);

  ShaderBuilder shader(const std::string &tag);
  ShaderBuilder shader();

  FramebufferBuilder framebuffer(GLsizei width, GLsizei height);

  template <Numeric T>
  std::unique_ptr<StaticBuffer<T>> static_buffer(
      const BufTarget &target,
      const BufUsage &usage,
      const std::vector<T> &data
  );

  std::unique_ptr<VertexArray> vertex_array(
      Shader &shader,
      const std::vector<BufAttrs> &buf_attrs
  );

  void imgui_newframe();
  void imgui_render();

  template <std::invocable<GladGLContext &> T>
  void run_block(T &&func) { func(gl); }

private:
  std::unique_ptr<GladGLContext> gl{nullptr};

  struct {
    ImGuiContext *ctx{nullptr};
    ImGuiIO *io{nullptr};
  } imgui_{};

  struct {
    ImPlotContext *ctx{nullptr};
  } implot_{};

  void received_msg_(const Msg &msg) override;

  void initialize_imgui_(GLFWwindow *glfw_window);

  static void GLAPIENTRY gl_message_callback_(
      GLenum source,
      GLenum type,
      GLuint id,
      GLenum severity,
      GLsizei length,
      const GLchar *message,
      const void *userParam
  );
};

template<> struct ModuleInfo<ModuleTag::GLContext> {
  const static std::vector<ModuleTag> dependencies;
};

template <Numeric T>
std::unique_ptr<StaticBuffer<T>> GLContext::static_buffer(
    const BufTarget &target,
    const BufUsage &usage,
    const std::vector<T> &data
) {
  return std::make_unique<StaticBuffer<T>>(*gl, target, usage, data);
}

} // namespace capra
