#include "capra/core/module/window.hpp"

#include "capra/core/glfw_callbacks.hpp"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"
#include "capra/util/platform.hpp"

namespace capra {

const std::vector<ModuleTag> ModuleInfo<ModuleTag::Window>::dependencies = {};

Window::Window() : Module(ModuleTag::Window) {
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowClose);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowSize);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwFramebufferSize);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowContentScale);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowPos);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowIconify);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowMaximize);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowFocus);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::GlfwWindowRefresh);
}

void Window::open(const WindowOpenParams &params) {
  open_params_ = params;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.gl_version.x);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.gl_version.y);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (set(params.flags, WindowFlags::fullscreen) || set(params.flags, WindowFlags::borderless))
    open_fullscreen_(params);
  else
    open_windowed_(params);

  internal::register_glfw_callbacks(glfw_handle);

  glfwMakeContextCurrent(glfw_handle);

  vsync = set(params.flags, capra::WindowFlags::vsync);
  glfwSwapInterval(vsync ? 1 : 0);

  if (!set(params.flags, WindowFlags::fullscreen) &&
      !set(params.flags, WindowFlags::borderless) &&
      !set(params.flags, WindowFlags::hidden))
    glfwShowWindow(glfw_handle);
}

WindowOpenParams Window::get_open_params() const {
  return open_params_;
}

void Window::set_x(int xpos) {
  glfwSetWindowPos(glfw_handle, xpos, y);
}

void Window::set_y(int ypos) {
  glfwSetWindowPos(glfw_handle, x, ypos);
}

void Window::set_w(int width) {
  glfwSetWindowSize(glfw_handle, width, h);
}

void Window::set_h(int height) {
  glfwSetWindowSize(glfw_handle, w, height);
}

void Window::set_decorated(bool is_decorated) {
  decorated = is_decorated;
  glfwSetWindowAttrib(glfw_handle, GLFW_DECORATED, is_decorated ? GLFW_TRUE : GLFW_FALSE);
}

void Window::set_resizable(bool is_resizable) {
  resizable = is_resizable;
  glfwSetWindowAttrib(glfw_handle, GLFW_RESIZABLE, is_resizable ? GLFW_TRUE : GLFW_FALSE);
}

void Window::set_vsync(bool is_enabled) {
  vsync = is_enabled;
  glfwSwapInterval(vsync ? 1 : 0);
}

bool Window::should_close() const {
  return glfwWindowShouldClose(glfw_handle);
}

void Window::set_should_close(bool should) {
  glfwSetWindowShouldClose(glfw_handle, should ? GLFW_TRUE : GLFW_FALSE);
}

void Window::swap() const {
  glfwSwapBuffers(glfw_handle);
}

void Window::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const GlfwWindowSize &e) {
        CAPRA_LOG_INFO("Window size event");
        w = e.width;
        h = e.height;
      },
      [&](const GlfwWindowClose &) { MsgBus::send<MsgType::Shutdown>(); },
      [&](const auto &e) { CAPRA_LOG_WARN("Window received unhandled event: {}", msg.type); }
  }, msg.data);
}

GLFWmonitor *Window::get_monitor_(int monitor_num) {
  int monitor_count = 0;
  auto monitors = glfwGetMonitors(&monitor_count);

  if (monitor_num >= monitor_count) {
    CAPRA_LOG_WARN("Monitor {} out of range (only {} monitors available); defaulting to primary", monitor_num, monitor_count);
    return monitors[0];
  }
  return monitors[monitor_num];
}

void Window::open_fullscreen_(const WindowOpenParams &params) {
#if defined(CAPRA_PLATFORM_WINDOWS)
  GLFWmonitor *monitor = get_monitor_(params.monitor_num);
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);  // Why is this necessary?
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  if (set(params.flags, WindowFlags::borderless)) {
    borderless = true;

    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

    glfw_handle = glfwCreateWindow(mode->width, mode->height, params.title.c_str(), nullptr, nullptr);
  } else
    glfw_handle = glfwCreateWindow(mode->width, mode->height, params.title.c_str(), monitor, nullptr);

  if (!glfw_handle) {
    const char *description;
    int code = glfwGetError(&description);
    CAPRA_LOG_CRITICAL("Failed to create GLFW window: ({}) {}", code, description);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  } else
    CAPRA_LOG_DEBUG("Created GLFW window");

  w = mode->width;
  h = mode->height;

  if (set(params.flags, WindowFlags::borderless)) {
    int base_x, base_y;
    glfwGetMonitorPos(monitor, &base_x, &base_y);
    glfwSetWindowPos(glfw_handle, base_x, base_y);
  }
#elif defined(CAPRA_PLATFORM_LINUX)
  /* We are making the assumption that the user is running a version of X11
   * that treats *all* fullscreen windows as borderless fullscreen windows.
   * This seems to generally be true for a good majority of systems. This may
   * also just act exactly like a normal fullscreen, there's not really any
   * way to tell ahead of time.
   */

  GLFWmonitor *monitor = get_monitor_(params.monitor_num);
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);  // Why is this necessary?
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  if (set(params.flags, WindowFlags::borderless)) {
    borderless = true;

    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
  }

  glfw_handle = glfwCreateWindow(mode->width, mode->height, params.title.c_str(), monitor, nullptr);
  if (!glfw_handle) {
    const char *description;
    int code = glfwGetError(&description);
    CAPRA_LOG_CRITICAL("Failed to create GLFW window:\n* ({}) {}", code, description);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  } else
    CAPRA_LOG_DEBUG("Created GLFW window");

  w = mode->width;
  h = mode->height;
#endif
}

void Window::open_windowed_(const WindowOpenParams &params) {
  GLFWmonitor *monitor = get_monitor_(params.monitor_num);
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, set(params.flags, WindowFlags::resizable) ? GLFW_TRUE : GLFW_FALSE);
  if (set(params.flags, WindowFlags::undecorated)) {
    decorated = false;
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  }

  glfw_handle = glfwCreateWindow(params.size.x, params.size.y, params.title.c_str(), nullptr, nullptr);
  if (!glfw_handle) {
    const char *description;
    int code = glfwGetError(&description);
    CAPRA_LOG_CRITICAL("Failed to create GLFW window:\n* ({}) {}", code, description);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  } else
    CAPRA_LOG_DEBUG("Created GLFW window");

  w = params.size.x;
  h = params.size.y;

  int base_x, base_y;
  glfwGetMonitorPos(monitor, &base_x, &base_y);
  if (set(params.flags, WindowFlags::centered))
    glfwSetWindowPos(
        glfw_handle,
        base_x + (mode->width - params.size.x) / 2,
        base_y + (mode->height - params.size.y) / 2
    );
  else
    glfwSetWindowPos(
        glfw_handle,
        base_x + params.pos.x,
        base_y + params.pos.y
    );
}

} // namespace capra
