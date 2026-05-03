#pragma once

#include <vector>

#include "app/base_renderer.h"
#include "app/base_scene.h"
#include "app/base_world.h"
#include "geom/shape.h"

namespace scene3 {
struct World : BaseWorld {
  static constexpr auto MAX_SUB_LEVEL = 3;
  int subLevel = 0;

  Shape base;
  Object shown;

  static constexpr float ECCEN_MIN = 0.1, ECCEN_MAX = 4.0;
  glm::vec3 eccentricity{1.0, 1.0, 1.0};

  World();

  void update(const float dt) override;

  void setNewShown();
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
} // namespace scene3