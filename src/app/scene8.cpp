#include "app/scene8.h"

#include "app/app.h"
#include "gl/gl_object.h"
#include "util.h"

#include <imgui.h>

using namespace scene8;

Scene::Scene()
    : BaseScene(std::in_place_type<World>, std::in_place_type<Renderer>,
                {
                    {},
                    [](GLFWwindow *window, int button, int action, int mods) {},
                    [](GLFWwindow *window, double xoffset, double yoffset) {},
                    [](GLFWwindow *window, double xpos, double ypos) {},
                }) {
  world = static_cast<World *>(BaseScene::world.get());
}

World::World() : BaseWorld() {
  Shape cube;
  cube.tris.reserve(12);
  static constexpr glm::vec3 vertices[8]{
      {-1, -1, +1}, {+1, -1, +1}, {+1, -1, -1}, {-1, -1, -1},
      {-1, +1, -1}, {+1, +1, -1}, {+1, +1, +1}, {-1, +1, +1},
  };
  static constexpr int indices[12][3]{
      {0, 2, 1}, {0, 3, 2}, {0, 1, 6}, {0, 6, 7}, {1, 2, 5}, {1, 5, 6},
      {2, 3, 4}, {2, 4, 5}, {3, 0, 7}, {3, 7, 4}, {7, 6, 5}, {7, 5, 4},
  };
  for (int i = 0; i < 12; i++) {
    cube.tris.emplace_back(vertices[indices[i][0]], vertices[indices[i][1]],
                           vertices[indices[i][2]]);
  }
  cube.color = LIGHT_BLUE;
  this->cube = Object{
      "Cube",
      cube,
      RotationController{Rotator{}},
  };
}
void World::update(const float dt) {
  cube.rotation(dt);
  const float t = glfwGetTime() / 1;
  light = {cosf(t), sinf(t), 0.5};
  light *= 2;
}
void Renderer::renderImpl(const float dt) const {
  BaseRenderer::renderImpl(dt);

  static GL::Texture diffuse{"brick.png"}, normal{"brick_normal.png"};
  static constexpr float RADIUS = 2;
  static constexpr vert_lay::postex VERTICES[]{
      {{-RADIUS, -RADIUS, 0}, {0, 0}},
      {{+RADIUS, -RADIUS, 0}, {1, 0}},
      {{+RADIUS, +RADIUS, 0}, {1, 1}},
      {{-RADIUS, +RADIUS, 0}, {0, 1}},
  };
  static constexpr glm::ivec3 indices[] = {
      {0, 1, 2},
      {0, 2, 3},
  };
  static GL::VBO<vert_lay::normalmap> VBO{6};
  // https://learnopengl.com/Advanced-Lighting/Normal-Mapping
  for (const auto i : indices) {
    const auto a = VERTICES[i.x], b = VERTICES[i.y], c = VERTICES[i.z];
    const auto e1 = b.pos - a.pos, e2 = c.pos - a.pos;
    const auto duv1 = b.tex - a.tex, duv2 = c.tex - a.tex;
    const float f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);

    const glm::vec3 normal = cross(e1, e2);
    const glm::vec3 tangent{
        f * (duv2.y * e1.x - duv1.y * e2.x),
        f * (duv2.y * e1.y - duv1.y * e2.y),
        f * (duv2.y * e1.z - duv1.y * e2.z),
    };
    // const glm::vec3 bitangent{
    //     f * (+duv2.x * e1.x + duv1.x * e2.x),
    //     f * (+duv2.x * e1.y + duv1.x * e2.y),
    //     f * (+duv2.x * e1.z + duv1.x * e2.z),
    // };

    VBO.write({a.pos, normal, tangent /*, bitangent*/, a.tex});
    VBO.write({b.pos, normal, tangent /*, bitangent*/, b.tex});
    VBO.write({c.pos, normal, tangent /*, bitangent*/, c.tex});
  }
  app()
      .shaders.normalMap.setCamera(world->cam.matrix())
      .bindTextureDiffuseMap(diffuse)
      .bindTextureNormalMap(normal)
      .setModel({1.0})
      .setLight(world->light)
      //.setLightColor(world->lightColor)
      .setCameraPos(world->cam.pos)
      //.setFragColor(WHITE)
      .draw(GL_TRIANGLES, VBO);
}
void Renderer::renderSidebar(const float dt) {}