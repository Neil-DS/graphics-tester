#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;

uniform vec3 light_position;
uniform vec4 light_color;

in vec4 FragPos;

/*
  prettysure there's a built in type for fragpos that I could test the light position against
 */
void main()
{
  
  /*
    so I think the gl_FragCoord and light_position are in different coordinate systems? light position is in the world position
    frag is probably in screen?

   */
  float dist = distance(FragPos, vec4(light_position, 1.0));
  if(dist < 7) {
    FragColor = ourColor * light_color;
  }
  else{
    FragColor = ourColor *0.1f;
  }
  
  
     
}
