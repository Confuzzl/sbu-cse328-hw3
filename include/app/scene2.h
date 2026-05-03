#pragma once

#include <vector>

#include "app/base_renderer.h"
#include "app/base_scene.h"
#include "app/base_world.h"
#include "app/scene3.h"
#include "geom/shape.h"

namespace scene2 {
struct World : ::scene3::World {
  // static constexpr auto MAX_SUB_LEVEL = 3;
  // int subLevel = 0;

  // Shape base;
  // Object shown;

  World();

  // void update(const float dt) override;

  // void setNewShown();
};
struct Renderer : /*BaseRenderer*/ ::scene3::Renderer {
  // World *world;
  // Renderer(World *const world) : BaseRenderer(world), world{world} {}
  using ::scene3::Renderer::Renderer;

private:
  // void renderImpl(const float dt) const override;
  void renderSidebar(const float dt) override;
};

struct Scene : BaseScene {
  World *world; // shadows
  Scene();
};
} // namespace scene2