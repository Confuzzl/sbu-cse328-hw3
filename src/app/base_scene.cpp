#include "app/base_scene.h"

InputHandler BaseScene::getDefaultInput() {
  return {
      {
          {GLFW_KEY_W, Key{Key::HOLD,
                           [this](const float dt) {
                             world->cam.move(+world->cam.flatFront, dt);
                           }}},
          {GLFW_KEY_A, Key{Key::HOLD,
                           [this](const float dt) {
                             world->cam.move(-world->cam.right, dt);
                           }}},
          {GLFW_KEY_S, Key{Key::HOLD,
                           [this](const float dt) {
                             world->cam.move(-world->cam.flatFront, dt);
                           }}},
          {GLFW_KEY_D, Key{Key::HOLD,
                           [this](const float dt) {
                             world->cam.move(+world->cam.right, dt);
                           }}},
          {GLFW_KEY_SPACE,
           Key{Key::HOLD,
               [this](const float dt) { world->cam.move(+Camera::UP, dt); }}},
          {GLFW_KEY_LEFT_CONTROL,
           Key{Key::HOLD,
               [this](const float dt) { world->cam.move(-Camera::UP, dt); }}},
          {GLFW_KEY_UP,
           Key{Key::HOLD,
               [this](const float dt) { world->cam.rotate(0, +100 * dt); }}},
          {GLFW_KEY_DOWN,
           Key{Key::HOLD,
               [this](const float dt) { world->cam.rotate(0, -100 * dt); }}},
          {GLFW_KEY_LEFT,
           Key{Key::HOLD,
               [this](const float dt) { world->cam.rotate(-100 * dt, 0); }}},
          {GLFW_KEY_RIGHT,
           Key{Key::HOLD,
               [this](const float dt) { world->cam.rotate(+100 * dt, 0); }}},
          {GLFW_KEY_X,
           Key{Key::JUST, [this](const float dt) { world->showAxes ^= true; }}},
          {GLFW_KEY_L,
           Key{Key::JUST,
               [this](const float dt) { world->light = world->cam.pos; }}},
          {GLFW_KEY_F1, Key{Key::JUST,
                            [this](const float dt) {
                              world->renderState =
                                  BaseWorld::RenderState::WIREFRAME;
                            }}},
          {GLFW_KEY_F2, Key{Key::JUST,
                            [this](const float dt) {
                              world->renderState = BaseWorld::RenderState::FLAT;
                            }}},
          {GLFW_KEY_F3, Key{Key::JUST,
                            [this](const float dt) {
                              world->renderState =
                                  BaseWorld::RenderState::NORMAL;
                            }}},
          {GLFW_KEY_F4, Key{Key::JUST,
                            [this](const float dt) {
                              world->renderState =
                                  BaseWorld::RenderState::PHONG;
                            }}},
      },
      [](GLFWwindow *window, int button, int action, int mods) {},
      [](GLFWwindow *window, double xoffset, double yoffset) {},
      [](GLFWwindow *window, double xpos, double ypos) {},
  };
}

void BaseScene::addNewInputs(InputHandler &&input) {
  for (auto &&[i, k] : input.keys) {
    this->input.keys.insert_or_assign(i, std::move(k));
  }
  this->input.mouseButtonCallback = [input](GLFWwindow *window, int button,
                                            int action, int mods) {
    input.mouseButtonCallback(window, button, action, mods);
  };
  this->input.scrollCallback = [input](GLFWwindow *window, double xoffset,
                                       double yoffset) {
    input.scrollCallback(window, xoffset, yoffset);
  };
  this->input.cursorCallback = [this, input](GLFWwindow *window, double xpos,
                                             double ypos) {
    world->cam.rotate(Cursor::dGamePos.x, Cursor::dGamePos.y);
    input.cursorCallback(window, xpos, ypos);
  };
}