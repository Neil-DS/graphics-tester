#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "logging.h"

class Shader{
  
public:
  /**
   *brief: responsible for loading the shaders into memory, compiling, and linking them.
   */
  Shader(std::string VertexDir, std::string FragmentDir) :
    vertex_directory_(VertexDir),
    fragment_directory_(FragmentDir)    
  {    
    Init();
  }
  
  ~Shader(){
          
  }

  /**
   *brief: opens the shader files and loads them into a buffer
   */
  void ReadShaderFiles();
  
  void CheckError();
  
  void Init();
  
  inline const char** get_vertex_data(){
    return &vertex_data_;
  }

  inline const char** get_fragment_data(){
    return &fragment_data_;
  }

  inline unsigned int& get_handle(){
    return handle_;
  }

private:
  unsigned int handle_{NULL}; //has value?
  
  std::string vertex_directory_{};
  
  std::string fragment_directory_{};
  
  std::string vertex_shader_info_{};
  
  std::string fragment_shader_info_{};

  const char* vertex_data_{NULL};
  
  const char* fragment_data_{NULL};
  
};

