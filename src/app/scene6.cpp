#include "app/scene6.h"

#include "app/app.h"
#include "gl/gl_object.h"

#include <imgui.h>

using namespace scene6;

static Shape subdivide(const Shape &shape) {
  Shape out;

  const auto oldSize = shape.tris.size();
  out.tris.reserve(oldSize * 4);
  for (int i = 0; i < oldSize; i++) {
    const auto &v = shape.tris[i].vertices;
    const auto v0 = glm::normalize(v[0]), v1 = glm::normalize(v[1]),
               v2 = glm::normalize(v[2]);
    const auto v01 = glm::normalize((v0 + v1) / 2.0f);
    const auto v12 = glm::normalize((v1 + v2) / 2.0f);
    const auto v20 = glm::normalize((v2 + v0) / 2.0f);
    out.tris.emplace_back(v0, v01, v20);
    out.tris.emplace_back(v1, v12, v01);
    out.tris.emplace_back(v2, v20, v12);
    out.tris.emplace_back(v01, v12, v20);
  }
  out.transform = shape.transform;
  out.color = shape.color;

  return out;
}

void World::setNewShown() {
  Shape out = baseDodecahedron;
  for (int i = 0; i < subLevel; i++)
    out = subdivide(out);

  shownDodecahedron.shape = std::move(out);
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
                    [this](GLFWwindow *window, double xpos, double ypos) {},
                }) {
  world = static_cast<World *>(BaseScene::world.get());
}

World::World() {
  // static constexpr auto compare_float = [](const float a, const float b) {
  //   static constexpr float EPS = 0.001;
  //   return abs(a - b) < EPS;
  // };
  // static constexpr auto sign = [](const float x) {
  //   if (compare_float(x, 0))
  //     return 0;
  //   return x > 0 ? +1 : -1;
  // };
  // static constexpr auto f = [](const float a, const float b) {
  //   const float s = sin(a);
  //   return sign(s) * pow(abs(s), b);
  // };
  // static constexpr auto g = [](const float a, const float b) {
  //   const float s = cos(a);
  //   return sign(s) * pow(abs(s), b);
  // };
  // const glm::vec3 scales{1}, powers{3};
  // const float A = scales.x, B = scales.y, C = scales.z;
  // const float r = powers.x, s = powers.y, t = powers.z;
  // static constexpr float STEP = 1.0 / 4;
  // static constexpr float PI = glm::pi<float>();
  // for (float y = 0; y <= 1; y += STEP) {
  //   for (float x = 0; x <= 1; x += STEP) {
  //     const float u = (x - 0.5) * 2 * PI;
  //     const float v = (y - 0.5) * PI;
  //    // https://en.wikipedia.org/wiki/Superquadrics#Parametric_description
  //    const float rho = 2 / r, sig = 2 / s, tau = 2 / t;
  //    const glm::vec3 p{
  //        A * g(v, rho) * g(u, rho),
  //        B * g(v, sig) * f(u, sig),
  //        C * f(v, tau),
  //    };
  //    // check etc/superquadric.md
  //    const glm::vec3 d_u = {
  //        A * g(v, rho) * -rho * sin(u) * pow(abs(cos(u)), rho - 1),
  //        B * g(v, sig) * +sig * cos(u) * pow(abs(sin(u)), sig - 1),
  //        0,
  //    };
  //    const glm::vec3 d_v = {
  //        A * g(u, rho) * -rho * sin(v) * pow(abs(cos(v)), rho - 1),
  //        B * f(u, sig) * -sig * sin(v) * pow(abs(cos(v)), sig - 1),
  //        C * tau * cos(v) * pow(abs(sin(v)), tau - 1),
  //    };
  //    glm::vec3 n = cross(d_u, d_v);
  //    std::cout << fmt::format("({:.3f},{:.3f}):\n", x, y);
  //    std::cout << fmt::format("\tp=({:.3f} {:.3f} {:.3f})\n", p.x, p.y, p.z);
  //    std::cout << fmt::format("\td_u=({:.3f} {:.3f} {:.3f}) [{:.3f}]\n",
  //    d_u.x,
  //                             d_u.y, d_u.z, glm::length(d_u));
  //    std::cout << fmt::format("\td_v=({:.3f} {:.3f} {:.3f}) [{:.3f}]\n",
  //    d_v.x,
  //                             d_v.y, d_v.z, glm::length(d_v));
  //    // if (compare_float(dot(d_v, d_v), 0)) { // at sharp edges
  //    //   n = p;
  //    // }
  //  }
  //}

  superquadric = {
      {},
      {1, 1, 1},
      {1, 1, 1},
      glm::translate(superquadric.transform, {-1.5, 0, 0}),
  };

  static constexpr glm::vec3 faces[12][5]{
      {
          {0.577350, 0.577350, 0.577350},
          {0.356822, 0.934172, 0.000000},
          {-0.356822, 0.934172, 0.000000},
          {-0.577350, 0.577350, 0.577350},
          {0.000000, 0.356822, 0.934172},
      },
      {
          {0.577350, 0.577350, 0.577350},
          {0.934172, 0.000000, 0.356822},
          {0.934172, 0.000000, -0.356822},
          {0.577350, 0.577350, -0.577350},
          {0.356822, 0.934172, 0.000000},
      },
      {
          {0.577350, 0.577350, 0.577350},
          {0.000000, 0.356822, 0.934172},
          {0.000000, -0.356822, 0.934172},
          {0.577350, -0.577350, 0.577350},
          {0.934172, 0.000000, 0.356822},
      },
      {
          {0.356822, 0.934172, 0.000000},
          {0.577350, 0.577350, -0.577350},
          {0.000000, 0.356822, -0.934172},
          {-0.577350, 0.577350, -0.577350},
          {-0.356822, 0.934172, 0.000000},
      },
      {
          {0.934172, 0.000000, 0.356822},
          {0.577350, -0.577350, 0.577350},
          {0.356822, -0.934172, 0.000000},
          {0.577350, -0.577350, -0.577350},
          {0.934172, 0.000000, -0.356822},
      },
      {
          {0.000000, 0.356822, 0.934172},
          {-0.577350, 0.577350, 0.577350},
          {-0.934172, 0.000000, 0.356822},
          {-0.577350, -0.577350, 0.577350},
          {0.000000, -0.356822, 0.934172},
      },
      {
          {-0.356822, 0.934172, 0.000000},
          {-0.577350, 0.577350, -0.577350},
          {-0.934172, 0.000000, -0.356822},
          {-0.934172, 0.000000, 0.356822},
          {-0.577350, 0.577350, 0.577350},
      },
      {
          {-0.577350, -0.577350, 0.577350},
          {-0.356822, -0.934172, 0.000000},
          {0.356822, -0.934172, 0.000000},
          {0.577350, -0.577350, 0.577350},
          {0.000000, -0.356822, 0.934172},
      },
      {
          {0.577350, -0.577350, -0.577350},
          {0.000000, -0.356822, -0.934172},
          {0.000000, 0.356822, -0.934172},
          {0.577350, 0.577350, -0.577350},
          {0.934172, 0.000000, -0.356822},
      },
      {
          {-0.577350, -0.577350, -0.577350},
          {-0.934172, 0.000000, -0.356822},
          {-0.577350, 0.577350, -0.577350},
          {0.000000, 0.356822, -0.934172},
          {0.000000, -0.356822, -0.934172},
      },
      {
          {-0.577350, -0.577350, -0.577350},
          {-0.356822, -0.934172, 0.000000},
          {-0.577350, -0.577350, 0.577350},
          {-0.934172, 0.000000, 0.356822},
          {-0.934172, 0.000000, -0.356822},
      },
      {
          {-0.577350, -0.577350, -0.577350},
          {0.000000, -0.356822, -0.934172},
          {0.577350, -0.577350, -0.577350},
          {0.356822, -0.934172, 0.000000},
          {-0.356822, -0.934172, 0.000000},
      },
  };
  baseDodecahedron.tris.reserve(12 * 5);
  for (const glm::vec3(&face)[5] : faces) {
    glm::vec3 center{};
    for (const auto v : face) {
      center += v;
    }
    center /= 5;
    for (int i = 0; i < 5; i++) {
      const int j = (i + 1) % 5;
      baseDodecahedron.tris.emplace_back(face[i], face[j], center);
    }
  }
  baseDodecahedron.color = RED;
  baseDodecahedron.transform =
      glm::translate(baseDodecahedron.transform, {+1.5, 0, 0});
  setNewShown();
}
void World::update(const float dt) {
  sqRotation(dt);
  shownDodecahedron.rotation(dt);
}

void Renderer::renderImpl(const float dt) const {
  BaseRenderer::renderImpl(dt);

  static GL::VBO<vert_lay::superquadric> SQ{1};
  const auto &[center, scales, powers, transform, color] = world->superquadric;
  SQ.write({center, scales, powers});
  app()
      .shaders.superquadric.setCamera(world->cam.matrix())
      .setModel(transform * world->sqRotation.mat)
      .setLight(world->light)
      .setLightColor(world->lightColor)
      .setCameraPos(world->cam.pos)
      .setFragColor(color)
      .draw(SQ);

  {
    static constexpr auto MAX_TRIS = 12 * 5 * pow(4, World::MAX_SUB_LEVEL),
                          MAX_VERTS = 3 * MAX_TRIS;

    const auto &[name, shape, rotation] = world->shownDodecahedron;
    const auto t = shape.transform * rotation.mat;

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
}
void Renderer::renderSidebar(const float dt) {
  ImGui::TextUnformatted("+ and - to change quality");
  ImGui::Text("subdivide=%d", world->subLevel);

  ImGui::Separator();

  ImGui::SeparatorText("Superquadric");
  ImGui::Checkbox("rotate##superquadric", &world->sqRotation.rotate);
  ImGui::SameLine();
  if (ImGui::Button("reset##superquadric")) {
    world->sqRotation.mat = {1.0};
  }
  ImGui::SliderFloat("A", &world->superquadric.scales.x, 0.1, 4, "%.1f");
  ImGui::SliderFloat("B", &world->superquadric.scales.y, 0.1, 4, "%.1f");
  ImGui::SliderFloat("C", &world->superquadric.scales.z, 0.1, 4, "%.1f");

  ImGui::SliderFloat("r", &world->superquadric.powers.x, 0.1, 4, "%.1f");
  ImGui::SliderFloat("s", &world->superquadric.powers.y, 0.1, 4, "%.1f");
  ImGui::SliderFloat("t", &world->superquadric.powers.z, 0.1, 4, "%.1f");

  ImGui::SeparatorText("Dodecahedron");
  ImGui::Checkbox("rotate##dodecahedron",
                  &world->shownDodecahedron.rotation.rotate);
  ImGui::SameLine();
  if (ImGui::Button("reset##dodecahedron")) {
    world->shownDodecahedron.rotation.mat = {1.0};
  }
}