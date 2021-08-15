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

//will want to make this a timer class to get rid of globals and allow for more than one clock
#define NOW() app_timer.getSecondsElapsed(app_timer.begin_, app_timer.getFrequencyClock())

bool GLOBAL_RUNNING=true;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
  LRESULT Result = 0;  
  switch(msg)
    {
    case WM_CLOSE:
      {
	GLOBAL_RUNNING = false;
      } break;
    case WM_CREATE:
      {
	
      }	break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
      {
	//log
      }break;     
    case WM_DESTROY:
      {
	GLOBAL_RUNNING = false;
      } break;
    case WM_QUIT:
      {
      }break;    
    default:
      {
	Result = DefWindowProc(hWnd, msg, wParam, lParam);
      }break;
    }
  
  return(Result);
}

static void
Win32ProcessPending(){
  MSG message;
  
  while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
      uint32_t key_code = (uint32_t)message.wParam;
      switch(message.message)
	{
	case WM_QUIT:
	  {
	    GLOBAL_RUNNING = false;
	  }break;
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	  {
	    if(key_code == VK_ESCAPE)
	      GLOBAL_RUNNING = false;
	  }break; 
	default:
	  {
	    TranslateMessage(&message);
	    DispatchMessage(&message);
	  }break;
	}      
    }
}

void CreateGLContext(HDC hdc){
  
  PIXELFORMATDESCRIPTOR pfd =
    {
     sizeof(PIXELFORMATDESCRIPTOR),
     1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
     PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
     32,                   // Colordepth of the framebuffer.
     0, 0, 0, 0, 0, 0,
     0,
     0,
     0,
     0, 0, 0, 0,
     24,                   // Number of bits for the depthbuffer
     8,                    // Number of bits for the stencilbuffer
     0,                    // Number of Aux buffers in the framebuffer.
     PFD_MAIN_PLANE,
     0,
     0, 0, 0
    };

  int iPixelFormat;  
  // get the best available match of pixel format for the device context   
  iPixelFormat = ChoosePixelFormat(hdc, &pfd); 
 
  // make that the pixel format of the device context  
  SetPixelFormat(hdc, iPixelFormat, &pfd);

  HGLRC glHandleThing = wglCreateContext(hdc);

  wglMakeCurrent(hdc, glHandleThing); 
}

//
class Primitive{
public:
  glm::vec3 position_{};

  /**
   * @brief: responsible for creating the vao/vbo for display a basic shape like like a square or line
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

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow){  
  WNDCLASSA wnd_class = {};
  HWND window = {};

  MyTimer app_timer;
  app_timer.begin_ = app_timer.getFrequencyClock();
  //used as a starting period for calculating timing 
  timeBeginPeriod(1);

  wnd_class.style = CS_OWNDC;//CS_HREDRAW|CS_VREDRAW;
  wnd_class.lpfnWndProc = WndProc;
  wnd_class.hInstance = hInstance;
  wnd_class.lpszClassName = "OpenGLDefaultApp";

  if(RegisterClassA(&wnd_class))
    {

      window =
	CreateWindowExA(
			0, // WS_EX_TOPMOST|WS_EX_LAYERED,
			wnd_class.lpszClassName,
			"OpenGLDefaultApp",
			WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			//WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			970,
			970,
			0,
			0,
			hInstance,
			0);     
      
      if(window){      
	/*
	//windowed fullscreen   	
	LONG lStyle = GetwindowLong(window, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	lStyle |= WS_THICKFRAME;
	SetwindowLong(window, GWL_STYLE, lStyle);
	//windows chaches info, so have to tell it something has changed so it can redraw it properly.
	SetwindowPos(window, HWND_NOTOPMOST, 100, 100, WIDTH, HEIGHT, SWP_FRAMECHANGED);       

	SetCursor((HCURSOR)LoadImageA(
	hInstance,
	IDC_ARROW,
	IMAGE_CURSOR,
	0,
	0,
	LR_DEFAULTCOLOR));
	*/	
	CreateGLContext(GetDC(window)); // make the openGL context current.
	
	GLenum err = glewInit();
	if(GLEW_OK != err){
	  std::cout << "err";
	  return 1;
	}

	Shader shader("data\\shader.vs", "data\\shader.fs");
	
	float cube_v[] = {
			-0.5f, -0.5f, 0.0f, // left  
			0.5f, -0.5f, 0.0f, // right 
			0.5f,  0.5f, 0.0f,  // top right

			0.5f,  0.5f, 0.0f,  // top right
			-0.5f,  0.5f, 0.0f,  // top left
			-0.5f, -0.5f, 0.0f // left  
	};  
  
	float line_v[] = {
			-0.5f, 0.0f, 0.0f, //vertical line
			0.5f, 0.0f, 0.0f, 
		      
			0.0f,  -0.5f, 0.0f,  // horizontal line
			0.0f,  0.5f, 0.0f
	};

	std::vector<WorldObject*> drawList;

	Application app{};
	app.Init(drawList);
	
	int vertexProjection = glGetUniformLocation(shader.get_handle(), "projectionMatrix");

	glUseProgram(shader.get_handle());

	//platform::LOGW("COMMON{} WHY");
	
	//ortho
	glm::mat4 ortho = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f);	    
	glUniformMatrix4fv(vertexProjection, 1, false, &ortho[0][0]);

	//--------LIGHT------------------------	
	int light_position = glGetUniformLocation(shader.get_handle(), "light_position");
	int light_color = glGetUniformLocation(shader.get_handle(), "light_color");

	glUseProgram(0);	    

	//primitive which sets up the vbo/vao with a simple draw function
	Primitive square{shader.get_handle(), {0.0f, 0.0f, 0.0f}, cube_v, sizeof(cube_v), GL_TRIANGLES, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)};	
	Primitive line{shader.get_handle(), {0.0f, 0.0f, 1.0f}, line_v, sizeof(line_v), GL_LINES, glm::vec4(1.0f), 90.0f, 90.0f};
		
	float dt = 0;
	float TimeSinceLastFrame = 0;

	float lastFrameUpdate = NOW();
	float nextFrameUpdate = lastFrameUpdate;
	float lastGameUpdate = lastFrameUpdate;
	float nextGameUpdate = lastFrameUpdate;
	
	float targetSecPerFrame = 1.0f / 60.0f;
	float targetSecPerUpdate = 1.0f / 25.0f;
	
	/*
	  TODO: if I make this load a dll for the objects I could hot-load and possibly add a console input functionality? 
	  this would also get more more familiar with windows dlls and overall compiling.  

a	  
	*/
	while(GLOBAL_RUNNING){
	  
	  //----------------- UPDATE WORLD ------------------
	  if(NOW() >= nextGameUpdate){
	    
	    dt = NOW() - lastGameUpdate; //game timestep
	    nextGameUpdate = NOW() + targetSecPerUpdate;  

	    Win32ProcessPending(); //input	    
 
	    app.Update(dt); //world

	    lastGameUpdate = NOW();
	  }//GAME TICK UPDATE ENFORCER

	  //----------------- UPDATE FRAMES ------------------
	  if(NOW() >= nextFrameUpdate){	    
	    TimeSinceLastFrame = NOW() - lastFrameUpdate;
	    
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    //making the light source the same position as the white square in the drawlist.
	    glUseProgram(shader.get_handle());
	    //hacky as hell drawList[1] is just the white square
	    glUniform3fv(light_position, 1, &drawList[0]->position_[0]);
	    glUniform4fv(light_color, 1, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);
	    glUseProgram(0);

	    /*
	      draws the objects based on it's primitive drawtype

	      TODO: I could put some consideration into cpu cache and whether I should split up update and render calls
	      * so i'm not fetching the data twice once to update then again to draw~
	     */
	    for(auto &object : drawList){
	      if(object->type_ == MY_TRIANGLE){
		square.draw(*object, ortho);
	      }
	      else if(object->type_ == MY_LINE)
		line.draw(*object, ortho);
	    }
	    
	    if(SwapBuffers(GetDC(window)) == false){
	      platform::LOGE("error swapping buffers winapi {}", "\n");
	      return 1;
	    }
	    
	    nextFrameUpdate = NOW() + targetSecPerFrame;
	    lastFrameUpdate = NOW();
	  }
	  
	  SetWindowTextA(window, std::to_string(TimeSinceLastFrame).c_str());

	  //update timer values and sleep for desired period
	  float nextWakeTime;
	  nextWakeTime = std::min(nextFrameUpdate, nextGameUpdate);
	  
	  float sleepFor = (1000.0f*(nextWakeTime - NOW()));
	  if(sleepFor > 0)
	    Sleep((DWORD)sleepFor);	  
	}

      }else{//error on create window
	//log
      }      
    
    }else{//error on register class
    //log
  }

  timeEndPeriod(1);

  return EXIT_SUCCESS;
  
}
