#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "logging.h"
#include "common.h"

enum PRIMITIVE_TYPE{MY_TRIANGLE, MY_LINE};
		   

class Application{

public:  
  Application() {}

  void Init(std::vector<WorldObject*> &drawList);
  void Update(const float &dt);
  void TestCode(const float &dt);

  WorldObject yellow_square{MY_TRIANGLE, {-15.0f, -15.0f, 1.0f}, &(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))[0]};
  WorldObject red_square{MY_TRIANGLE, {15.0f, -15.0f, 1.0f}, &(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))[0]};
  WorldObject green_square{MY_TRIANGLE, {15.0f, 15.0f, 1.0f}, &(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))[0]};
  WorldObject blue_square{MY_TRIANGLE, {-15.0f, 15.0f, 1.0f}, &(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))[0]};
  WorldObject white_square{MY_TRIANGLE, {5.0f, 5.0f, 1.0f}};

  WorldObject grid_lines{MY_LINE, {0.0f, 0.0f, 1.0f}};
};
