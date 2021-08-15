#pragma once

#include <string>
#include <windows.h>

#define LOGW(x, ...) console.ConsoleOut(FOREGROUND_RED | FOREGROUND_GREEN, sizeof(__VA_ARGS__), x, __VA_ARGS__)
#define LOGE(x, ...) console.ConsoleOut(FOREGROUND_RED, sizeof(__VA_ARGS__), x, __VA_ARGS__)
#define LOGI(x, ...) console.ConsoleOut(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, sizeof(__VA_ARGS__), x, __VA_ARGS__)

namespace platform{

  /**
     @brief asks windows to give the application a console and then creates a static instance to be accessed by code
   */
  class Console{
  private:
    HANDLE h_console_out_{};
  
    
  public:
    /*
     * @brief outputs the log to a console window. does not work with visual studio.
     * @param formatting of the output
     * @param amount of args
     * @param the initial message 
     */
    Console() {
      AllocConsole();
      h_console_out_ = GetStdHandle(STD_OUTPUT_HANDLE);

    
    }

    DWORD ConsoleOut(WORD attributes, size_t count, const char *msg, ...);


    
  };

  

  inline Console console;  
}
