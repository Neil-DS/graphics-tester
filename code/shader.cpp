#include "shader.h"

void Shader::ReadShaderFiles(){
  
  std::ifstream vertex_file_handle{};
  std::ifstream fragment_file_handle{};
  vertex_file_handle.open(vertex_directory_);
  fragment_file_handle.open(fragment_directory_);
  
  if(vertex_file_handle.is_open() && fragment_file_handle.is_open()){
    std::stringstream vertex_buffer;
    std::stringstream fragment_buffer;
    
    vertex_buffer << vertex_file_handle.rdbuf();
    fragment_buffer << fragment_file_handle.rdbuf();
    
    vertex_shader_info_ = vertex_buffer.str();
    fragment_shader_info_ = fragment_buffer.str();

    vertex_file_handle.close();
    fragment_file_handle.close();

    vertex_data_ = vertex_shader_info_.c_str();
    fragment_data_ = fragment_shader_info_.c_str();
    
  }else{
    throw std::runtime_error("failed to open a shader file");
  }

}

void Shader::CheckError(){
  
}

void Shader::Init(){

  ReadShaderFiles();

    //---------VERTEX SHADER
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, get_vertex_data(), NULL);
  glCompileShader(vertex_shader);
  int success;
  std::string info_log{"", 1024};  
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(vertex_shader, 1024, NULL, info_log.data());
     
      platform::LOGE("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n {}\n", info_log.c_str());
    }
  
  //---------FRAGMENT SHADER
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, get_fragment_data(), NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      info_log.clear();
      glGetShaderInfoLog(fragmentShader, 1024, NULL, info_log.data());
      
      platform::LOGE("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}\n", info_log.c_str());
    }
  
  //---------LINK SHADERS TOGETHER
  handle_ = glCreateProgram();
  glAttachShader(handle_, vertex_shader);
  glAttachShader(handle_, fragmentShader);
  glLinkProgram(handle_);
  
  glGetProgramiv(handle_, GL_LINK_STATUS, &success);
  if (!success)
    {
      info_log.clear();
      glGetProgramInfoLog(handle_, 1024, NULL, info_log.data());
      
      platform::LOGE("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}\n", info_log.c_str());
    }
  
  glDeleteShader(vertex_shader);
  glDeleteShader(fragmentShader);
  
}



