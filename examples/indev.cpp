#define CAPRA_PRINT_HELPERS
#include "capra/capra.hpp"

class Indev : public capra::Application {
public:
  std::unique_ptr<capra::Shader> shader{nullptr};
  std::unique_ptr<capra::StaticBuffer<float>> vbo{nullptr};
  std::unique_ptr<capra::VertexArray> vao{nullptr};

  void initialize() override {
    window->open({
        .title = "Indev",
        .size = {1920, 1080},
        .flags = capra::WindowFlags::centered
    });
    ctx->initialize_for_window(*window);

    shader = ctx->shader()
        .vert_from_src(R"glsl(
#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 in_color;

out vec4 out_color;

uniform mat4 mvp;

void main() {
  out_color = in_color;
  gl_Position = mvp * vec4(in_pos, 1.0);
}
)glsl")
        .frag_from_src(R"glsl(
#version 330

in vec4 out_color;

out vec4 FragColor;

void main() {
  FragColor = out_color;
}
)glsl")
        .link();

    vbo = ctx->static_buffer<float>(capra::BufTarget::array, capra::BufUsage::static_draw, {
         0.0,  0.3, 0.0,   1.0, 0.0, 0.0, 1.0,
        -0.3, -0.3, 0.0,   0.0, 1.0, 0.0, 1.0,
         0.3, -0.3, 0.0,   0.0, 0.0, 1.0, 1.0
    });

    vao = ctx->vertex_array(*shader, {
        {*vbo, {{"in_pos", 3, capra::AttrType::f, false, 7, 0},
                {"in_color", 4, capra::AttrType::f, false, 7, 3}}}
    });
  }

  void deinitialize() override {}

  void update(double dt) override {
    if (input->pressed("escape"))
      engine->shutdown();

    if (input->pressed("1"))
      window->set_vsync(!window->vsync);
  }

  void draw() override {
    ctx->clear(capra::rgb(0x000000));

    shader->use();
    shader->uniform_mat4f("mvp", glm::mat4(1.0f));
    vao->draw_arrays(capra::DrawMode::triangles, 0, 3);
  }
};

CAPRA_RUN(Indev)
