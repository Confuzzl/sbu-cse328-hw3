#include "app/scene7.h"

#include "app/app.h"
#include "gl/gl_object.h"

#include <imgui.h>

using namespace scene7;

Scene::Scene()
    : BaseScene(std::in_place_type<World>, std::in_place_type<Renderer>,
                {
                    {
                        {GLFW_KEY_H, Key{Key::JUST,
                                         [this](const float dt) {
                                           world->flyMode =
                                               World::FlyMode::HORIZONTAL;
                                         }}},
                        {GLFW_KEY_V, Key{Key::JUST,
                                         [this](const float dt) {
                                           world->flyMode =
                                               World::FlyMode::VERTICAL;
                                         }}},
                    },
                    [](GLFWwindow *window, int button, int action, int mods) {},
                    [](GLFWwindow *window, double xoffset, double yoffset) {},
                    [this](GLFWwindow *window, double xpos, double ypos) {},
                }) {
  world = static_cast<World *>(BaseScene::world.get());
}

static constexpr auto WORLD_SIZE = 128.0f;
static constexpr auto TILES = 8;
static constexpr auto TILE_SIZE = WORLD_SIZE / TILES;
World::World() {
  renderState = RenderState::PHONG;

  // skyColor = 0x4CA4FD_rgb;

  // light = {0, WORLD_SIZE / 2, 0};
  //  lightColor = 0xE8ECC9_rgb;
  //  cam.speed = 20;
  //  cam.pos = {WORLD_SIZE / 2, WORLD_SIZE / 2, WORLD_SIZE / 2};
  //   cam.rotate(0, -90);
  cam.setRotation(0, -90);

  static constexpr auto building = [](const glm::vec2 offset,
                                      const glm::vec2 size) {
    Shape out{};
    out.tris.reserve(10);

    const float halfWidth = size.x / 2;

    const glm::vec3 vertices[]{
        {-halfWidth, 0, -halfWidth},      {+halfWidth, 0, -halfWidth},
        {+halfWidth, 0, +halfWidth},      {-halfWidth, 0, +halfWidth},
        {-halfWidth, size.y, -halfWidth}, {+halfWidth, size.y, -halfWidth},
        {+halfWidth, size.y, +halfWidth}, {-halfWidth, size.y, +halfWidth},
    };
    static constexpr glm::ivec3 INDICES[10]{
        {0, 5, 1}, {4, 5, 0}, //
        {1, 6, 2}, {5, 6, 1}, //
        {2, 7, 3}, {6, 7, 2}, //
        {3, 4, 0}, {7, 4, 3}, //
        {6, 4, 7}, {5, 4, 6}, //
    };
    for (const auto i : INDICES) {
      out.tris.emplace_back(vertices[i.x], vertices[i.y], vertices[i.z]);
    }

    // out.color = GRAY;
    out.transform = glm::translate(out.transform, {offset.x, 0, offset.y});

    return out;
  };

  for (int j = 0; j < TILES - 1; j++) {
    for (int i = 0; i < TILES - 1; i++) {
      static constexpr float OFFSET = TILE_SIZE / 4;
      const glm::vec2 min = {(i + 0.5) * TILE_SIZE + OFFSET,
                             (j + 0.5) * TILE_SIZE + OFFSET};
      const glm::vec2 max = {(i + 1) * TILE_SIZE + OFFSET,
                             (j + 1) * TILE_SIZE + OFFSET};
      const glm::vec2 mid = (min + max) / 2.0f;
      static constexpr auto RANGE = 0.5f * TILE_SIZE;

      if (i == 3 && j == 3) {
        auto dome = Shape::subdivideSphere(Shape::icosahedron());

        dome.transform = glm::translate(dome.transform, {mid.x, 0, mid.y});
        dome.transform = glm::scale(dome.transform, glm::vec3{6.0f});
        dome.color = WHITE;

        shapes.emplace_back(dome);
        continue;
      }

      static constexpr auto MAX_COUNT = 10;
      const int count = random_int(1, MAX_COUNT);
      for (int n = 0; n < count; n++) {
        auto b = building(min +
                              glm::vec2{
                                  random_float(0.0f, 1.0f),
                                  random_float(0.0f, 1.0f),
                              } * RANGE,
                          {
                              random_float(1.0f, 5.0f),
                              random_float(1.0f, 20.0f),
                          });
        static constexpr Color COLORS[]{0xD6BE9C_rgb, 0x6E675D_rgb,
                                        0xB77248_rgb, 0x905130_rgb};
        // colors::mix()
        b.color = COLORS[random_int(0, 3)];
        shapes.emplace_back(b);
      }
      // shapes.emplace_back(building(min, {1, 5})).color = RED;
      // shapes.emplace_back(building(max, {1, 5})).color = BLUE;
    }
  }
}
void World::update(const float dt) {
  static constexpr glm::vec3 CENTER{WORLD_SIZE / 2, 0, WORLD_SIZE / 2};

  {
    const float t = glfwGetTime() / 10;
    light = {cosf(t), sinf(t), 0};
    lightColor = colors::mix(GRAY, WHITE, (light.y + 1) / 2);

    skyColor = colors::mix(BLACK, 0x4CA4FD_rgb, (light.y + 1) / 2);
    light *= WORLD_SIZE; // radius
    light += CENTER;
  }
  {
    static constexpr float FLY_RADIUS = WORLD_SIZE / 4;
    const float t = glfwGetTime();
    if (flyMode == FlyMode::HORIZONTAL) {
      cam.pos = {cosf(t), 0, sinf(t)};
      cam.pos *= FLY_RADIUS;
      cam.pos += CENTER;
      cam.pos.y = WORLD_SIZE / 4;
    } else {
      cam.pos = {cosf(t), sinf(t), 0};
      cam.pos *= FLY_RADIUS;
      cam.pos += CENTER;
      cam.pos.y += WORLD_SIZE / 2;
    }
  }
}

void Renderer::renderImpl(const float dt) const {
  {
    static constexpr float SUN_RAD = 20;
    static GL::Texture sun{"sun.png"};
    static constexpr vert_lay::postex light[]{
        {{-SUN_RAD, -SUN_RAD, 0}, {0, 0}},
        {{+SUN_RAD, -SUN_RAD, 0}, {1, 0}},
        {{+SUN_RAD, +SUN_RAD, 0}, {1, 1}},
        {{-SUN_RAD, +SUN_RAD, 0}, {0, 1}},
    };
    static constexpr auto indices = {
        0, 1, 2, 0, 2, 3,
    };
    static GL::EBO ebo{indices};
    static GL::VBO<vert_lay::postex> LIGHT{4};
    LIGHT.writeList(light);

    // https://gamedev.stackexchange.com/a/150705
    auto billboard = glm::transpose(world->cam.matrix());
    billboard[0][3] = 0;
    billboard[1][3] = 0;
    billboard[2][3] = 0;
    billboard[3] = glm::vec4{world->light, 1.0};

    app()
        .shaders.texcol.setCamera(world->cam.matrix())
        .bindTextureSampler(sun)
        .setModel(billboard)
        .setFragColor(world->lightColor)
        .draw(GL_TRIANGLES, LIGHT, ebo);
  }

  {
    static GL::Texture groundTex{"ground.png"};
    static constexpr vert_lay::postex ground[]{
        {{0, 0, 0}, {0, 0}},
        {{0, 0, WORLD_SIZE}, {0, TILES}},
        {{WORLD_SIZE, 0, WORLD_SIZE}, {TILES, TILES}},
        {{WORLD_SIZE, 0, 0}, {TILES, 0}},
    };
    static constexpr auto indices = {
        0, 1, 2, 0, 2, 3,
    };
    static GL::EBO ebo{indices};
    static GL::VBO<vert_lay::postex> GROUND{4};
    GROUND.writeList(ground);

    app()
        .shaders.texcol.setCamera(world->cam.matrix())
        .bindTextureSampler(groundTex)
        .setModel({1.0})
        .setFragColor(world->lightColor)
        .draw(GL_TRIANGLES, GROUND, ebo);
  }

  static constexpr auto MAX_VERTICES = 1000;
  if (world->renderState == World::RenderState::WIREFRAME) {
    static GL::VBO<> WIRE{MAX_VERTICES};
    for (const auto &shape : world->shapes) {
      for (const auto &tri : shape.tris)
        for (const auto v : tri.vertices)
          WIRE.write({v});

      app()
          .shaders.basic.setCamera(world->cam.matrix())
          .setModel(shape.transform)
          .setFragColor(shape.color)
          .draw(GL_LINE_LOOP, WIRE);
    }
    return;
  }
  static GL::VBO<vert_lay::posnorm> NORM{MAX_VERTICES};
  for (const auto &shape : world->shapes) {
    const auto t = shape.transform;
    const auto nm = normal_matrix(t);
    for (const auto &tri : shape.tris) {
      const auto normal = glm::normalize(nm * tri.normal);
      for (const auto v : tri.vertices)
        NORM.write(vert_lay::posnorm{v, normal});
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
void Renderer::renderSidebar(const float dt) {}