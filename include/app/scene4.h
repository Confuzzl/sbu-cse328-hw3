#pragma once

#include "app/base_renderer.h"
#include "app/base_scene.h"
#include "app/base_world.h"
#include "geom/shape.h"

#include <array>

namespace scene4 {
struct World : BaseWorld {
  World();

  struct {
    glm::vec3 center;
    float radius;
    glm::mat4 transform{1.0f};

    RotationController rotation;
  } sphere;

  struct {
    glm::vec3 center;
    float radius;
    float height;
    glm::mat4 transform{1.0f};

    RotationController rotation;
  } cylinder;

  struct {
    glm::vec3 base;
    float radius;
    float height;
    glm::mat4 transform{1.0f};

    RotationController rotation;
  } cone;

  std::array<const char *, 3> names;
  std::array<RotationController *, 3> rotations;

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
} // namespace scene4