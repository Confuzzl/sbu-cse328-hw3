#include "app/scene4.h"

#include "app/app.h"
#include "gl/gl_object.h"

#include <imgui.h>

using namespace scene4;

Scene::Scene()
    : BaseScene(std::in_place_type<World>, std::in_place_type<Renderer>,
                {
                    {

                    },
                    [](GLFWwindow *window, int button, int action, int mods) {},
                    [](GLFWwindow *window, double xoffset, double yoffset) {},
                    [this](GLFWwindow *window, double xpos, double ypos) {},
                }) {
  world = static_cast<World *>(BaseScene::world.get());
}

World::World() {
  sphere = {
      {},
      1,
      glm::translate(glm::mat4{1.0}, {-3, 0, 0}),
  };

  cylinder = {
      {},
      1,
      2,
  };

  cone = {
      {},
      1,
      2,
      glm::translate(glm::mat4{1.0}, {+3, 0, 0}),
  };

  names = {"Sphere", "Cylinder", "Cone"};
  rotations = {&sphere.rotation, &cylinder.rotation, &cone.rotation};
}
void World::update(const float dt) {
  for (const auto r : rotations)
    (*r)(dt);
}

void Renderer::renderImpl(const float dt) const {
  BaseRenderer::renderImpl(dt);

  {
    const auto &sphere = world->sphere;
    const auto &rotation = world->sphere.rotation.mat;
    static GL::VBO<vert_lay::sphere> SPHERE{1};
    SPHERE.write(vert_lay::sphere{sphere.center, sphere.radius});
    app()
        .shaders.sphere.setCamera(world->cam.matrix())
        .setModel(sphere.transform * rotation)
        .setLight(world->light)
        .setLightColor(world->lightColor)
        .setCameraPos(world->cam.pos)
        .setFragColor(ORANGE)
        .draw(SPHERE);
  }

  {
    const auto &cylinder = world->cylinder;
    const auto &rotation = world->cylinder.rotation.mat;
    static GL::VBO<vert_lay::cylinder> CYLINDER{1};
    CYLINDER.write({cylinder.center, cylinder.radius, cylinder.height});
    app()
        .shaders.cylinder.setCamera(world->cam.matrix())
        .setModel(cylinder.transform * rotation)
        .setLight(world->light)
        .setLightColor(world->lightColor)
        .setCameraPos(world->cam.pos)
        .setFragColor(PURPLE)
        .draw(CYLINDER);
  }

  {
    const auto &cone = world->cone;
    const auto &rotation = world->cone.rotation.mat;
    static GL::VBO<vert_lay::cone> CONE{1};
    CONE.write({cone.base, cone.radius, cone.height});
    app()
        .shaders.cone.setCamera(world->cam.matrix())
        .setModel(cone.transform * rotation)
        .setLight(world->light)
        .setLightColor(world->lightColor)
        .setCameraPos(world->cam.pos)
        .setFragColor(CYAN)
        .draw(CONE);
  }
}
void Renderer::renderSidebar(const float dt) {
  for (int i = 0; i < 3; i++) {
    const auto name = world->names[i];
    const auto rotator = world->rotations[i];
    auto &rotate = rotator->rotate;

    ImGui::SeparatorText(name);
    ImGui::Checkbox(fmt::format("rotate##{}", name).c_str(), &rotate);
    ImGui::SameLine();
    if (ImGui::Button(fmt::format("reset##{}", name).c_str())) {
      rotator->mat = {1.0};
    }
  }
}