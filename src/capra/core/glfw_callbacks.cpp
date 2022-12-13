#include "capra/util/log.hpp"

#include "capra/core/glfw_callbacks.hpp"
#include "capra/core/msg_bus.hpp"

namespace capra::internal {

void register_glfw_callbacks(GLFWwindow *window) {
  glfwSetErrorCallback(error_callback);

  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetWindowContentScaleCallback(window, window_content_scale_callback);
  glfwSetWindowPosCallback(window, window_pos_callback);
  glfwSetWindowIconifyCallback(window, window_iconify_callback);
  glfwSetWindowMaximizeCallback(window, window_maximize_callback);
  glfwSetWindowFocusCallback(window, window_focus_callback);
  glfwSetWindowRefreshCallback(window, window_refresh_callback);

  glfwSetKeyCallback(window, key_callback);
  glfwSetCharCallback(window, character_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetCursorEnterCallback(window, cursor_enter_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetJoystickCallback(joystick_callback);
  glfwSetDropCallback(window, drop_callback);

  glfwSetMonitorCallback(monitor_callback);

  CAPRA_LOG_DEBUG("Registered GLFW callbacks");
}

void error_callback(int code, const char *description) {
  CAPRA_LOG_ERROR("GLFW ({}): {}", code, description);
}

void window_size_callback(GLFWwindow *window, int width, int height) {
  MsgBus::send_nowait<MsgType::GlfwWindowSize>(window, width, height);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  MsgBus::send_nowait<MsgType::GlfwFramebufferSize>(window, width, height);
}

void window_content_scale_callback(GLFWwindow *window, float xscale, float yscale) {
  MsgBus::send_nowait<MsgType::GlfwWindowContentScale>(window, xscale, yscale);
}

void window_pos_callback(GLFWwindow *window, int xpos, int ypos) {
  MsgBus::send_nowait<MsgType::GlfwWindowPos>(window, xpos, ypos);
}

void window_iconify_callback(GLFWwindow *window, int iconified) {
  MsgBus::send_nowait<MsgType::GlfwWindowIconify>(window, iconified);
}

void window_maximize_callback(GLFWwindow *window, int maximized) {
  MsgBus::send_nowait<MsgType::GlfwWindowMaximize>(window, maximized);
}

void window_focus_callback(GLFWwindow *window, int focused) {
  MsgBus::send_nowait<MsgType::GlfwWindowFocus>(window, focused);
}

void window_refresh_callback(GLFWwindow *window) {
  MsgBus::send_nowait<MsgType::GlfwWindowRefresh>(window);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
//  if (!ImGui::GetIO().WantCaptureKeyboard)
  MsgBus::send_nowait<MsgType::GlfwKey>(window, key, scancode, action, mods);
}

void character_callback(GLFWwindow *window, unsigned int codepoint) {
//  if (!ImGui::GetIO().WantCaptureKeyboard)
  MsgBus::send_nowait<MsgType::GlfwCharacter>(window, codepoint);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
//  if (!ImGui::GetIO().WantCaptureMouse)
  MsgBus::send_nowait<MsgType::GlfwCursorPos>(window, xpos, ypos);
}

void cursor_enter_callback(GLFWwindow *window, int entered) {
//  if (!ImGui::GetIO().WantCaptureMouse)
  MsgBus::send_nowait<MsgType::GlfwCursorEnter>(window, entered);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
//  if (!ImGui::GetIO().WantCaptureMouse)
  MsgBus::send_nowait<MsgType::GlfwMouseButton>(window, button, action, mods);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
//  if (!ImGui::GetIO().WantCaptureMouse)
  MsgBus::send_nowait<MsgType::GlfwScroll>(window, xoffset, yoffset);
}

void joystick_callback(int jid, int event) {
  MsgBus::send_nowait<MsgType::GlfwJoystick>(jid, event);
}

void drop_callback(GLFWwindow *window, int count, const char** paths) {
  MsgBus::send_nowait<MsgType::GlfwDrop>(window, count, paths);
}

void monitor_callback(GLFWmonitor *monitor, int event) {
  MsgBus::send_nowait<MsgType::GlfwMonitor>(monitor, event);
}

} // namespace capra::internal