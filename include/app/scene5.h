#pragma once

#include "app/base_renderer.h"
#include "app/base_scene.h"
#include "app/base_world.h"
#include "geom/shape.h"

namespace scene5 {
struct World : BaseWorld {
  World();

  static constexpr float OUTER_RADIUS_MIN = 1, OUTER_RADIUS_MAX = 5;
  struct {
    glm::vec3 center;
    float innerRadius;
    float outerRadius;
    glm::mat4 transform{1.0f};
    Color color = WHITE;
  } torus;
  RotationController rotation;

  static constexpr auto MAX_SUB_LEVEL = 3;
  int subLevel = 0;
  float resolution = 16;

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
} // namespace scene5