#version 430 core

in  vec4 v_color;
layout (location = 0) out vec4 f_color;

uniform float _Time;

vec2 squareCpx(vec2 a)
{
	return vec2(a.x*a.x - a.y*a.y, 2*a.x*a.y);
}
        
float cpxSquareMod(vec2 z)
{
	return dot(z,z);
}

vec4 setFragColor(float dist, const float ITER, float count)
{
	vec3 color_ratio = vec3(1.0, 1.0, 1.0);//_ColorRatio.xyz;
	
	// Set the fragments' color
    if ( dist <= 4 )
    	return vec4(0,0,0,1);	// The fragment is part of the set
    else
    {
    	count /= ITER/8;
    	
		if (dist <= 9)
		{
			vec4 color = vec4(count, count, count, 1);
			color.rgb *= color_ratio;
			return color;
		}
		else
		{
			vec4 color = vec4(count*dist/9, count*dist/9, count*dist / 9, 1);
			color.rgb *= color_ratio;
			return color;
		}
    }
}

vec2 collapsePixels(vec2 px, vec2 target)
{
	// The following value is an interesting target
	//float2 target = float2(-3.0/4.0, 0.25);
	
	px += abs(sin(0.5*_Time)) * normalize(-target + px);

	return px;
}

vec4 fadeOut(vec4 pixColor, float startTime, float duration)
{
	vec4 color	= pixColor;
	float t		= _Time - startTime;

	color *= ((-duration * t + duration) / duration);

	return color;
}

vec4 mandelbrot(vec2 fragCoord)
{
	// Frame settings
	//float2 target = float2(-3.0 / 4.0, 0.25);
	//float2 target = float2(-0.04524074130409, 0.9868162207157852);
	vec2 target		= vec2(0.281717921930775, 0.5771052841488505);
	float h			= 0.0000000000000001;
	float xl			= target.x-h, xr = target.x+h, yb = target.y-h, yt = target.y+h;
	float y_intercept = 2000;
    
	// Manage scene's traveling speed
	float zoom = 0.0;
	if ( _Time <= 23.0 )
		zoom = y_intercept + 400*pow(_Time, 1 + (_Time - 23.0) / 23.0);
	else
		zoom = y_intercept + 400 * pow(_Time, 2);
	
	// Convert pixels from pixel coordinates to complex coordinates
	vec2 c = fragCoord / zoom;
	c.x += xl; c.y += yb;
    
	// Initializing the Mandelbrot series
	vec2 z = vec2(0,0);
	  	
	// Check if the fragment pertains to the Mandelbrot set
	int count = 0;
	const int ITER = 4196; // The greater it is, the more accurate the result becomes
	while ( cpxSquareMod(z) <= 4 && count < ITER ) // While |z| <= 2, the pixel is assumed to be part of the Mandelbrot set
	{
		z = squareCpx(z) + c;
		count++;
	}
	  
	// Compute final color
	if (_Time < 66)
		return setFragColor(cpxSquareMod(z), ITER, count);
	else
		return fadeOut(setFragColor(cpxSquareMod(z), ITER, count), 66, 2);
}

void main()
{
	f_color = mandelbrot(gl_FragCoord.xy);
}