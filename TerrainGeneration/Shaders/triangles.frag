#version 450 core

out vec4 fColour;
in vec4 fragColour;

uniform bool wireframe;

void main()
{
	if(wireframe) {
		fColour = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		fColour = fragColour;
	}
}
