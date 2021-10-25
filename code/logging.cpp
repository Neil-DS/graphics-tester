#include "logging.h"

DWORD platform::Console::ConsoleOut(WORD attributes, size_t count, const char *msg, ...){    
  DWORD chars_written = 0;
  std::string input_string = msg;
      
  SetConsoleTextAttribute(h_console_out_, attributes);

  
  va_list args;
  va_start(args, msg);

  size_t found = input_string.find("{}");
  for(size_t i = 0; i < count; i++){
    if(found != std::string::npos)
      input_string.replace(found, 2, va_arg(args, const char *));
    else //no more {}
      break;
    found = input_string.find("{}", found+1);	
  }

  /*
  va_end(args);

  
  va_list args;
  va_start(args, msg);

  //gets the first {}, what if there is none? 
  size_t found = input_string.find("{}");
  if(found != std::string::npos){
    
  }
  
  va_end(args);
  */  


  
  input_string += '\n';    
  WriteFile(h_console_out_, input_string.c_str(), input_string.size(), &chars_written, NULL);

  return chars_written;
}    

