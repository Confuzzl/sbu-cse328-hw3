#include "app/scene5.h"

#include "app/app.h"
#include "geom/rotator.h"
#include "gl/gl_object.h"

#include <imgui.h>

using namespace scene5;

Scene::Scene()
    : BaseScene(std::in_place_type<World>, std::in_place_type<Renderer>,
                {
                    {
                        {GLFW_KEY_EQUAL, Key{Key::JUST,
                                             [this](const float dt) {
                                               if (world->subLevel <
                                                   World::MAX_SUB_LEVEL) {
                                                 world->subLevel++;
                                                 world->resolution *= 2;
                                               }
                                             },
                                             GLFW_MOD_SHIFT}},
                        {GLFW_KEY_MINUS, Key{Key::JUST,
                                             [this](const float dt) {
                                               if (world->subLevel != 0) {
                                                 world->subLevel--;
                                                 world->resolution /= 2;
                                               }
                                             }}},
                    },
                    [](GLFWwindow *window, int button, int action, int mods) {},
                    [](GLFWwindow *window, double xoffset, double yoffset) {},
                    [this](GLFWwindow *window, double xpos, double ypos) {},
                }) {
  world = static_cast<World *>(BaseScene::world.get());
}

World::World() {
  torus = {{0, 0, 0}, 0.5, 2};
  torus.color = LIGHT_GREEN;
}
void World::update(const float dt) { rotation(dt); }

void Renderer::renderImpl(const float dt) const {
  BaseRenderer::renderImpl(dt);

  const auto &torus = world->torus;
  static GL::VBO<vert_lay::torus> TORUS{1};
  TORUS.write({torus.center, torus.innerRadius, torus.outerRadius});
  app()
      .shaders.torus.setResolution(world->resolution)
      .setCamera(world->cam.matrix())
      .setModel(torus.transform * world->rotation.mat)
      .setLight(world->light)
      .setLightColor(world->lightColor)
      .setCameraPos(world->cam.pos)
      .setFragColor(torus.color)
      .draw(TORUS);
}
void Renderer::renderSidebar(const float dt) {
  ImGui::TextUnformatted("+ and - to change quality");
  ImGui::Text("subdivide=%d", world->subLevel);
  ImGui::Text("resolution=%d", static_cast<int>(world->resolution));

  ImGui::Separator();

  ImGui::SeparatorText("Torus");
  ImGui::Checkbox("rotate", &world->rotation.rotate);
  ImGui::SameLine();
  if (ImGui::Button("reset")) {
    world->rotation.mat = {1.0};
  }

  ImGui::SliderFloat("Out rad", &world->torus.outerRadius,
                     World::OUTER_RADIUS_MIN, World::OUTER_RADIUS_MAX, "%.1f");
  static float ratio = world->torus.innerRadius / world->torus.outerRadius;
  ImGui::SliderFloat("In ratio", &ratio, 0.01, 1, "%.2f");
  world->torus.innerRadius = world->torus.outerRadius * ratio;
}