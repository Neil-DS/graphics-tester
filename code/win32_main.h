#include <iostream>
#include <windows.h>
#include <Winuser.h>
#include <string>
#include <complex>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include "shader.h"
#include "common.h"
#include "main.h"

#define NOW() app_timer.getSecondsElapsed(app_timer.begin_, app_timer.getFrequencyClock())

/*
  pretty sure this creates a object on the gpu with the parameters I give. think I could probably do any shape I want as long as I gave it the right
  vertexs

  what's currently tripping me up is the geometric object on the gpu vs something in the 'world', should I have a different object? 
  
  i kind of remember textures and how they are similar to objects where you register and attach as needed.
 */
class Primitive{
public:
  glm::vec3 position_{};

  /**
   * @brief: responsible for creating the vao/vbo for displaying a basic shape like like a square or line
   * @param: the gpu shader reference
   * @param: starting world position of the object
   * @param: pointer to the vertices to pass to the gpu,
   * @param: size of the vertex array in bytes. 
   * @param: the type of primitive drawing method to use, should be one of the open gl defined methods e.g. GL_LINES, GL_TRIANGLES
   */
  Primitive(uint32_t &Shader, glm::vec3 Pos, float *Vertices, size_t Size, int Method, glm::vec4 Color,
	    float Length=1.0f, float Height=1.0f) :
    shader_(Shader),
    position_(Pos),
    primitive_draw_method_(Method),
    color_(Color),
    length_(Length),
    height_(Height)    
  {
    vertice_amount_ = Size/sizeof(Vertices[0]);
    
    //this is shader dependant and if you change the name in the shader you have to change it here, and if you have a different shaders with different names
    world_model_ = glGetUniformLocation(Shader, "worldPos");    
    frag_color_ = glGetUniformLocation(Shader, "ourColor");    
    
    //generate
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    //bind
    glBindVertexArray(vao_);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, Size, Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //enable above attribute.
    glEnableVertexAttribArray(0); 

    //unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //unbind VAO
    glBindVertexArray(0);
  }

  ~Primitive(){
    glDeleteVertexArrays(1, &vao_);
  }  
  
  //Draws an instance of the object
  void draw(WorldObject &object, glm::mat4 &ortho){
    assert(primitive_draw_method_ > -1);
    
    model_ = glm::mat4(1.0f);
    model_ = glm::translate(model_, object.position_);
    model_ = glm::scale(model_, glm::vec3(length_, height_, 1.0f));

    glUseProgram(shader_);
    
    glUniformMatrix4fv(world_model_, 1, false, &model_[0][0]);
    glUniform4fv(frag_color_, 1, &object.color_[0]);
  
    glBindVertexArray(vao_);
    glDrawArrays(primitive_draw_method_, 0, vertice_amount_);
    
    glUseProgram(0);
  }  
  
private:  
  int primitive_draw_method_ = -1;
  float length_, height_ = 1.0f;
  uint32_t vertice_amount_ = 0;
  glm::mat4 model_ = glm::mat4(1.0f);
  
  glm::vec4 color_{1.0f};
  unsigned int vbo_, vao_ = 0;

  uint32_t &shader_;
  //uniform handles:
  int world_model_ = 0;
  int frag_color_ = 0;  
};
