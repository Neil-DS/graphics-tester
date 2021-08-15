#pragma once

#include <vector>

#include "logging.h"


class MyTimer{
public:

  MyTimer(){
    QueryPerformanceFrequency(&performance_clock_frequency_);    
  }
  
  LARGE_INTEGER begin_;
  LARGE_INTEGER performance_clock_frequency_;

  //Current clock cycles since machine startup
  inline LARGE_INTEGER
  getFrequencyClock(void){
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return(result);
  }

  /**
     @brief: returns the time elapsed from periods 
  */
  inline float
  getSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end){
    return (float)(end.QuadPart - start.QuadPart) /
      (float)performance_clock_frequency_.QuadPart;
  }

  
private:

};

class WorldObject{
public:
  
  WorldObject(int Type, glm::vec3 Position, float* Color = nullptr):
    type_(Type),
    position_(Position)
  {
    if(Color){
      color_ = glm::vec4(Color[0], Color[1], Color[2], Color[3]);
    }else
      color_ = glm::vec4(1.0f);
  }
  
  int type_;
  glm::vec3 position_;
  glm::vec4 color_;  
  
};


