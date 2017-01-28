#version 330

uniform sampler2D color;

in vec2 P;
in vec2 uv;
out vec3 pixel;

void main()
{
	// retrieve input pixel
	pixel = texture( color, uv ).rgb;
	// darken towards edges
	//float dx = P.x - 0.5, dy = P.y - 0.5;
	//float distance = sqrt( dx * dx + dy * dy );
	//float scale = 1 - max( 0, distance * 2.2 - 0.8 );
	//pixel *= scale;
}

// EOF