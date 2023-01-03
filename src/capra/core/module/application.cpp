#include "capra/core/module/application.hpp"

#include "capra/core/engine.hpp"
#include "capra/util/log.hpp"
#include "capra/util/overloaded.hpp"

namespace capra {

const std::vector<ModuleTag> ModuleInfo<ModuleTag::Application>::dependencies =
    {ModuleTag::GLContext, ModuleTag::InputMgr, ModuleTag::Window};

Application::Application() : Module(ModuleTag::Application) {
  MsgBus::subscribe(msg_endpoint_id_, MsgType::Initialize);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::Update);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::StartFrame);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::Draw);
  MsgBus::subscribe(msg_endpoint_id_, MsgType::EndFrame);
}

void Application::initialize() {}
void Application::deinitialize() {}
void Application::update(double dt) {}
void Application::draw() {}

void Application::initialize_dependencies_(std::shared_ptr<Engine> engine) {
  Module::initialize_dependencies_(engine);

  ctx = engine->get_module<GLContext>(ModuleTag::GLContext);
  input = engine->get_module<InputMgr>(ModuleTag::InputMgr);
  window = engine->get_module<Window>(ModuleTag::Window);
}

void Application::received_msg_(const Msg &msg) {
  std::visit(overloaded {
      [&](const Initialize &) {
        initialize();
      },
      [&](const Update &e) {
        update(e.dt);
        input->update(e.dt);
      },
      [&](const StartFrame &) {
        ctx->imgui_newframe();
      },
      [&](const Draw &) {
        draw();
      },
      [&](const EndFrame &) {
        draw_debug_overlay_();
        ctx->imgui_render();

        window->swap();
      },
      [&](const auto &e) { CAPRA_LOG_WARN("Application received unhandled event: {}", msg.type); }
  }, msg.data);
}

void Application::draw_debug_overlay_() {
  ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
  if (ImGui::Begin("Overlay_Perf", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("%s", fmt::format("FPS: {:.2f}{}", engine->framecounter.fps(), window->vsync ? " (vsync)" : "").c_str());

    auto dts = engine->framecounter.dts_vec();
    if (dts.size() > 0) {
      auto [dt_min, dt_max] = std::minmax_element(dts.begin(), dts.end());
      double mid = (*dt_max + *dt_min) / 2.0;

      ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, {0, 0});
      ImPlot::PushStyleColor(ImPlotCol_Line, capra::rgb(0x00ff00).vec4());
      ImPlot::PushStyleColor(ImPlotCol_Fill, capra::rgba(0x00ff0080).vec4());
      if (ImPlot::BeginPlot("FPS_History", {100, 35}, ImPlotFlags_CanvasOnly | ImPlotFlags_NoFrame | ImPlotFlags_NoInputs)) {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, dts.size(), ImPlotCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, mid - 1.5 * (*dt_max - mid), mid + 1.5 * (*dt_max - mid), ImPlotCond_Always);
        ImPlot::PlotShaded("History", &dts[0], dts.size());
        ImPlot::PlotLine("History", &dts[0], dts.size());
        ImPlot::EndPlot();
      }
      ImPlot::PopStyleColor(2);
      ImPlot::PopStyleVar();
    }
  }
  ImGui::End();
}

} // namespace capra
