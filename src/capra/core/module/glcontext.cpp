#include "capra/core/module/glcontext.hpp"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"

namespace capra {

const std::vector<ModuleTag> ModuleInfo<ModuleTag::GLContext>::dependencies = {};

GLContext::GLContext() : Module(ModuleTag::GLContext) {}

void GLContext::initialize_from_window(const Window &window) {
  auto glad_version = gladLoadGL(glfwGetProcAddress);
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

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_message_callback_, nullptr);
}

void GLContext::clear(const RGB &color, const ClearBit &bit) {
  glClearColor(
      static_cast<float>(color.r) / 255.0f,
      static_cast<float>(color.g) / 255.0f,
      static_cast<float>(color.b) / 255.0f,
      static_cast<float>(color.a) / 255.0f);
  glClear(unwrap(bit));
}

void GLContext::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const auto &e) { CAPRA_LOG_WARN("Window received unhandled event: {}", msg.type); }
  }, msg.data);
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
