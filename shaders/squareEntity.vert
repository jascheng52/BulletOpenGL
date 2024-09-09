#version 330 core

layout (location = 0) in vec2 initPos;


out vec4 PlayerColor;


uniform float xPos;
uniform float yPos;
uniform mat4 rotMat;
uniform float scale;


uniform float windWidth;
uniform float windHeight;
uniform vec3  rgbColor;

void main()
{
    float halfH = windHeight/2;
    float halfW = windWidth/2;
    vec4 scaledPos = rotMat * vec4(initPos.x,initPos.y,0,1);
    scaledPos = scaledPos * vec4(scale,scale,0,0);
    gl_Position = vec4((scaledPos.x + xPos)/halfW, (scaledPos.y + yPos)/halfH, 0 ,1);
    
PlayerColor = vec4(rgbColor, 1.0);

}