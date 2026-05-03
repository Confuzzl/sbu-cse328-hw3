#include "app/scene2.h"

#include "app/app.h"
#include "gl/gl_object.h"

#include <imgui.h>

using namespace scene2;

Scene::Scene()
    : BaseScene(std::in_place_type<World>, std::in_place_type<Renderer>,
                {
                    {
                        {GLFW_KEY_EQUAL, Key{Key::JUST,
                                             [this](const float dt) {
                                               if (world->subLevel <
                                                   World::MAX_SUB_LEVEL) {
                                                 world->subLevel++;
                                                 world->setNewShown();
                                               }
                                             },
                                             GLFW_MOD_SHIFT}},
                        {GLFW_KEY_MINUS, Key{Key::JUST,
                                             [this](const float dt) {
                                               if (world->subLevel != 0) {
                                                 world->subLevel--;
                                                 world->setNewShown();
                                               }
                                             }}},

                    },
                    [](GLFWwindow *window, int button, int action, int mods) {},
                    [](GLFWwindow *window, double xoffset, double yoffset) {},
                    [](GLFWwindow *window, double xpos, double ypos) {},
                }) {
  world = static_cast<World *>(BaseScene::world.get());
}

World::World() {
  cam.pos = {0, 0, 3};

  base.tris.reserve(20);
  static constexpr glm::vec3 vertices[60]{
      {0.525731, 0, 0.850651},   {0, 0.850651, 0.525731},
      {-0.525731, 0, 0.850651},  {0, 0.850651, 0.525731},
      {-0.850651, 0.525731, 0},  {-0.525731, 0, 0.850651},
      {0, 0.850651, 0.525731},   {0, 0.850651, -0.525731},
      {-0.850651, 0.525731, 0},  {0.850651, 0.525731, 0},
      {0, 0.850651, -0.525731},  {0, 0.850651, 0.525731},
      {0.525731, 0, 0.850651},   {0.850651, 0.525731, 0},
      {0, 0.850651, 0.525731},   {0.525731, 0, 0.850651},
      {0.850651, -0.525731, 0},  {0.850651, 0.525731, 0},
      {0.850651, -0.525731, 0},  {0.525731, 0, -0.850651},
      {0.850651, 0.525731, 0},   {0.850651, 0.525731, 0},
      {0.525731, 0, -0.850651},  {0, 0.850651, -0.525731},
      {0.525731, 0, -0.850651},  {-0.525731, 0, -0.850651},
      {0, 0.850651, -0.525731},  {0.525731, 0, -0.850651},
      {0, -0.850651, -0.525731}, {-0.525731, 0, -0.850651},
      {0.525731, 0, -0.850651},  {0.850651, -0.525731, 0},
      {0, -0.850651, -0.525731}, {0.850651, -0.525731, 0},
      {0, -0.850651, 0.525731},  {0, -0.850651, -0.525731},
      {0, -0.850651, 0.525731},  {-0.850651, -0.525731, 0},
      {0, -0.850651, -0.525731}, {0, -0.850651, 0.525731},
      {-0.525731, 0, 0.850651},  {-0.850651, -0.525731, 0},
      {0, -0.850651, 0.525731},  {0.525731, 0, 0.850651},
      {-0.525731, 0, 0.850651},  {0.850651, -0.525731, 0},
      {0.525731, 0, 0.850651},   {0, -0.850651, 0.525731},
      {-0.850651, -0.525731, 0}, {-0.525731, 0, 0.850651},
      {-0.850651, 0.525731, 0},  {-0.525731, 0, -0.850651},
      {-0.850651, -0.525731, 0}, {-0.850651, 0.525731, 0},
      {0, 0.850651, -0.525731},  {-0.525731, 0, -0.850651},
      {-0.850651, 0.525731, 0},  {-0.850651, -0.525731, 0},
      {-0.525731, 0, -0.850651}, {0, -0.850651, -0.525731},
  };

  for (int i = 0; i < 60; i += 3) {
    base.tris.emplace_back(vertices[i + 0], vertices[i + 1], vertices[i + 2]);
  }
  base.color = LIGHT_RED;

  shown = Object{"Ball", base, Rotator{}};
}
void Renderer::renderSidebar(const float dt) {
  ImGui::TextUnformatted("+ and - to change quality");
  ImGui::Text("subdivide=%d", world->subLevel);

  ImGui::Separator();

  auto &[name, shape, rotation] = world->shown;
  ImGui::SeparatorText(name);

  ImGui::Checkbox(fmt::format("rotate##{}", name).c_str(), &rotation.rotate);
  ImGui::SameLine();
  if (ImGui::Button(fmt::format("reset##{}", name).c_str())) {
    rotation.mat = {1.0};
  }
}