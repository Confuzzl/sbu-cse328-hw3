#pragma once

#include <vector>

#include "app/base_renderer.h"
#include "app/base_scene.h"
#include "app/base_world.h"
#include "geom/shape.h"

namespace scene6 {
struct World : BaseWorld {
  static constexpr auto MAX_SUB_LEVEL = 2;
  int subLevel = 0;

  struct {
    glm::vec3 center;
    glm::vec3 scales;
    glm::vec3 powers;
    glm::mat4 transform{1.0f};
    Color color = ORANGE;
  } superquadric;
  RotationController sqRotation;

  enum struct ConfigMode { DEFAULT, CUSTOM } configMode = ConfigMode::DEFAULT;
  struct Config {
    glm::vec3 scales;
    glm::vec3 powers;
  } defaultConfig{};
  static constexpr Config FALLBACK_CONFIG{{1, 1, 1}, {1, 1, 1}};
  bool defaultSuccess = false;
  void updateDefaultConfig();

  Shape baseDodecahedron;
  Object shownDodecahedron;

  void setNewShown();

  World();

  void update(const float dt) override;
};
struct Renderer : BaseRenderer {
  World *world;
  Renderer(World *const world) : BaseRenderer(world), world{world} {}

private:
  void renderImpl(const float dt) const override;
  void renderSidebar(const float dt) override;
};

struct Scene : BaseScene {
  World *world; // shadows
  Scene();
};
} // namespace scene6