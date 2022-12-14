#include "capra/core/module/glcontext.hpp"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"
#include "capra/util/rnd.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace capra {

const std::vector<ModuleTag> ModuleInfo<ModuleTag::GLContext>::dependencies = {};

GLContext::GLContext() : Module(ModuleTag::GLContext) {}

void GLContext::initialize_for_window(const Window &window) {
  gl = std::make_unique<GladGLContext>();
  auto glad_version = gladLoadGLContext(gl.get(), glfwGetProcAddress);
  if (glad_version == 0) {
    CAPRA_LOG_CRITICAL("Failed to initialize OpenGL");
    std::exit(EXIT_FAILURE);
  }

  auto glad_major = GLAD_VERSION_MAJOR(glad_version);
  auto glad_minor = GLAD_VERSION_MINOR(glad_version);
  CAPRA_LOG_DEBUG("Initialized OpenGL v{}.{}", glad_major, glad_minor);

  WindowOpenParams open_params = window.get_open_params();
  if (glad_major != open_params.gl_version.x || glad_minor != open_params.gl_version.y)
    CAPRA_LOG_WARN("Requested OpenGL v{}.{}", open_params.gl_version.x, open_params.gl_version.y);

  gl->Enable(GL_DEBUG_OUTPUT);
  gl->DebugMessageCallback(gl_message_callback_, nullptr);

  initialize_imgui_(window.glfw_handle);
}

void GLContext::clear(const RGB &color, const ClearBit &bit) {
  gl->ClearColor(
      static_cast<float>(color.r) / 255.0f,
      static_cast<float>(color.g) / 255.0f,
      static_cast<float>(color.b) / 255.0f,
      static_cast<float>(color.a) / 255.0f);
  gl->Clear(unwrap(bit));
}

ShaderBuilder GLContext::shader(const std::string &tag) {
  return {*gl, tag};
}

ShaderBuilder GLContext::shader() {
  return shader(rnd::base58(11));
}

FramebufferBuilder GLContext::framebuffer(GLsizei width, GLsizei height) {
  return {*gl, width, height};
}

std::unique_ptr<VertexArray> GLContext::vertex_array(
    Shader &shader,
    const std::vector<BufAttrs> &buf_attrs
) {
  return std::make_unique<VertexArray>(*gl, shader, buf_attrs);
}

void GLContext::imgui_newframe() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GLContext::imgui_render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLContext::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const auto &e) { CAPRA_LOG_WARN("Window received unhandled event: {}", msg.type); }
  }, msg.data);
}

void GLContext::initialize_imgui_(GLFWwindow *glfw_window) {
  IMGUI_CHECKVERSION();
  imgui_.ctx = ImGui::CreateContext();
  imgui_.io = &ImGui::GetIO();
  imgui_.io->IniFilename = nullptr;

  ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
  ImGui_ImplOpenGL3_Init();

  implot_.ctx = ImPlot::CreateContext();

  CAPRA_LOG_DEBUG("Initialized ImGui");
}

void GLAPIENTRY GLContext::gl_message_callback_(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei,
    const GLchar *message,
    const void *
) {
#define STRINGIFY(e) case e: return #e;
  std::string source_str = ([source](){
    switch (source) {
      STRINGIFY(GL_DEBUG_SOURCE_API)
      STRINGIFY(GL_DEBUG_SOURCE_WINDOW_SYSTEM)
      STRINGIFY(GL_DEBUG_SOURCE_SHADER_COMPILER)
      STRINGIFY(GL_DEBUG_SOURCE_THIRD_PARTY)
      STRINGIFY(GL_DEBUG_SOURCE_APPLICATION)
      STRINGIFY(GL_DEBUG_SOURCE_OTHER)
      default: return "?";
    }
  })();

  std::string type_str = ([type](){
    switch (type) {
      STRINGIFY(GL_DEBUG_TYPE_ERROR)
      STRINGIFY(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
      STRINGIFY(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
      STRINGIFY(GL_DEBUG_TYPE_PORTABILITY)
      STRINGIFY(GL_DEBUG_TYPE_PERFORMANCE)
      STRINGIFY(GL_DEBUG_TYPE_MARKER)
      STRINGIFY(GL_DEBUG_TYPE_PUSH_GROUP)
      STRINGIFY(GL_DEBUG_TYPE_POP_GROUP)
      STRINGIFY(GL_DEBUG_TYPE_OTHER)
      default: return "?";
    }
  })();

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      CAPRA_LOG_ERROR("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      CAPRA_LOG_WARN("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    case GL_DEBUG_SEVERITY_LOW:
      CAPRA_LOG_DEBUG("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      CAPRA_LOG_TRACE("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    default:
      break; // won't happen
  }
#undef STRINGIFY
}

} // namespace capra
