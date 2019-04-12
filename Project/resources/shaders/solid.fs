#version 330 core
out vec4 FragColor;
in vec4 vCol;
// Antialias stroke alpha coeff
float stroke_alpha(float distance, float linewidth, float antialias)
{
	float t = linewidth/2.0 - antialias;
	float signed_distance = distance;
	float border_distance = abs(signed_distance) - t;
	float alpha = border_distance/antialias;
	alpha = exp(-alpha * alpha);
	if( border_distance > (linewidth/2.0 + antialias) )
		return 0.0;
	else if( border_distance < 0.0 )
		return 1.0;
	else
		return alpha;
}

void main()
{
	//float lWidth = 2.5;
	//float antialias = 1.0;
    FragColor = vCol;
} 