// a particle vertex shader
#version 410

in vec4 Position;
in vec4 Colour;

uniform mat4 ProjectionViewModel;

out vec4 vColour;

void main()
{
    vColour = Colour;
    gl_Position = ProjectionViewModel * Position;
}