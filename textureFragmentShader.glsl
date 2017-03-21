#version 330 core
          
in vec2 v_uv;                            
uniform sampler2D tex;                       

out vec4 fragColor;

void main() 
{ 
	fragColor = texture2D( tex, v_uv/6 ); 
}