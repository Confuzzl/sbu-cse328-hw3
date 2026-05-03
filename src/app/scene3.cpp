#include "app/scene3.h"

#include "app/app.h"
#include "gl/gl_object.h"

#include <imgui.h>

using namespace scene3;

static Shape subdivideIco(const Shape &shape) {
  Shape out;

  const auto oldSize = shape.tris.size();
  out.tris.reserve(oldSize * 4);
  for (int i = 0; i < oldSize; i++) {
    const auto &v = shape.tris[i].vertices;
    const auto v01 = glm::normalize((v[0] + v[1]) / 2.0f);
    const auto v12 = glm::normalize((v[1] + v[2]) / 2.0f);
    const auto v20 = glm::normalize((v[2] + v[0]) / 2.0f);
    out.tris.emplace_back(v[0], v01, v20);
    out.tris.emplace_back(v[1], v12, v01);
    out.tris.emplace_back(v[2], v20, v12);
    out.tris.emplace_back(v01, v12, v20);
  }
  out.transform = shape.transform;
  out.color = shape.color;

  return out;
}

void World::setNewShown() {
  Shape out = base;
  for (int i = 0; i < subLevel; i++)
    out = subdivideIco(out);

  // out.transform = shown.shape.transform;
  shown.shape = std::move(out);
}

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
  // cam.pos = {0, 0, 3};

  base = Shape::icosahedron();
  base.color = LIGHT_BLUE;

  shown = Object{
      "Ellipsoid",
      base,
      RotationController{Rotator{}},
  };
}
void World::update(const float dt) { shown.rotation(dt); }

void Renderer::renderImpl(const float dt) const {
  BaseRenderer::renderImpl(dt);

  static constexpr auto MAX_TRIS = 20 * pow(4, World::MAX_SUB_LEVEL),
                        MAX_VERTS = 3 * MAX_TRIS;

  const auto &[name, shape, rotation] = world->shown;
  const auto t =
      glm::scale(shape.transform * rotation.mat, world->eccentricity);

  if (world->renderState == World::RenderState::WIREFRAME) {
    static GL::VBO<> WIRE{MAX_VERTS};

    for (const auto &tri : shape.tris)
      for (const auto v : tri.vertices)
        WIRE.write({v});

    app()
        .shaders.basic.setCamera(world->cam.matrix())
        .setModel(t)
        .setFragColor(shape.color)
        .draw(GL_LINE_LOOP, WIRE);
    return;
  }

  static GL::VBO<vert_lay::posnorm> NORM{MAX_VERTS};

  const auto nm = normal_matrix(t);
  for (const auto &tri : shape.tris) {
    if (world->renderState == World::RenderState::PHONG) {
      for (const auto v : tri.vertices)
        NORM.write(vert_lay::posnorm{v, glm::normalize(nm * v)});
    } else {
      const auto normal = glm::normalize(nm * tri.normal);
      for (const auto v : tri.vertices)
        NORM.write(vert_lay::posnorm{v, normal});
    }
  }
  switch (world->renderState) {
  case World::RenderState::FLAT:
    app()
        .shaders.flat.setCamera(world->cam.matrix())
        .setModel(t)
        .setLight(world->light)
        .setLightColor(world->lightColor)
        .setFragColor(shape.color)
        .draw(GL_TRIANGLES, NORM);
    break;
  case World::RenderState::NORMAL:
    app()
        .shaders.normal.setCamera(world->cam.matrix())
        .setModel(t)
        .draw(GL_TRIANGLES, NORM);
    break;
  case World::RenderState::PHONG:
    app()
        .shaders.phong.setCamera(world->cam.matrix())
        .setModel(t)
        .setLight(world->light)
        .setLightColor(world->lightColor)
        .setCameraPos(world->cam.pos)
        .setFragColor(shape.color)
        .draw(GL_TRIANGLES, NORM);
    break;
  }
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

  ImGui::SliderFloat("Eccen.X", &world->eccentricity.x, World::ECCEN_MIN,
                     World::ECCEN_MAX, "%.1f");
  ImGui::SliderFloat("Eccen.Y", &world->eccentricity.y, World::ECCEN_MIN,
                     World::ECCEN_MAX, "%.1f");
  ImGui::SliderFloat("Eccen.Z", &world->eccentricity.z, World::ECCEN_MIN,
                     World::ECCEN_MAX, "%.1f");
}