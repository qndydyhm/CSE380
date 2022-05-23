precision mediump float;

varying vec4 v_Position;

uniform vec4 circle_Color;
uniform vec4 default_Color;

// HOMEWORK 3
/*
	The fragment shader is where pixel colors are decided.
	You'll have to modify this code to make the circle vary between 2 colors.
	Currently this will render the exact same thing as the gradient_circle shaders
*/
void main(){
	// Default alpha is 0
	float alpha = 0.0;

	// Radius is 0.5, since the diameter of our quad is 1
	float radius = 0.5;
	float ratio = (1.0-(v_Position.x+0.25+v_Position.y+0.25));

	// Get the distance squared of from (0, 0)
	float dist_sq = v_Position.x*v_Position.x + v_Position.y*v_Position.y;

	if(dist_sq < radius*radius){
		// Multiply by 4, since distance squared is at most 0.25
		alpha = 1.0;
	}

	// Use the alpha value in our color
	gl_FragColor = vec4(default_Color[0]*ratio+circle_Color[0]*(1.0-ratio), default_Color[1]*ratio+circle_Color[1]*(1.0-ratio), default_Color[2]*ratio+circle_Color[2]*(1.0-ratio), default_Color[3]*ratio+circle_Color[3]*(1.0-ratio));
	gl_FragColor.a = alpha;
}