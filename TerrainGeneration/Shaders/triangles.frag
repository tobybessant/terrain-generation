#version 450 core

out vec4 fColour;
in vec4 fragColour;

uniform bool wireframe;

void main()
{
	fColour = fragColour;
}
