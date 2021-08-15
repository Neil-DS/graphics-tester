#include "main.h"

void Application::Init(std::vector<WorldObject*> &drawList){
  drawList.push_back(&white_square);
  drawList.push_back(&yellow_square);  
  drawList.push_back(&red_square);
  drawList.push_back(&green_square);
  drawList.push_back(&blue_square);

  drawList.push_back(&grid_lines);
};

void Application::TestCode(const float &dt){
  static float rotation_angle = 0;

  rotation_angle += dt;

  float speed = 1.0f;
  float radius = 10.0f;
  //location to rotate around 
  float rotation_orgin_x = 20.0f;
  float rotation_orgin_y = 20.0f;
	    
  white_square.position_.x = cos((rotation_angle)*speed)*20.0f;
  white_square.position_.y = sin((rotation_angle)*speed)*20.0f;	    
}

/*
  Ok, now I want to get into the shader programming. 

  I want to have an understanding of how to compute using the gpu, I can pass objects and variables to it easily enough

  but what kind of tasks can I have the gpu perform per vertex or per fragment? 

  so I mean the fragment shader can change each fragments color based on some variables, like it's distance from a light source~

  I could test this by making a light square and then passing that as the 'lightsource' position to the shader?
  then I'd just need to know where I want to do the distance calculation~
 */
void Application::Update(const float &dt){
  TestCode(dt);
}
