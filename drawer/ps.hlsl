////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer PixelBufferType
{
	float sigma_2;
	float clipEdge;
	float thet;
	int AAMode;
	int samplelvl;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 color : COLOR;
	float tolerance : TOLERANCE;
};

float normalFunc(float x)
{
	return thet*exp((x*x)/sigma_2);
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PMain(PixelInputType input) : SV_TARGET
{
	float clipDistance = input.tolerance*clipEdge;
	float distance = abs(input.tex.x)+abs(input.tex.y);
	if (distance < 1.0f-clipDistance) discard;
	if (distance > (1.0f+clipDistance)*1.41421356/*sqrt(2.0)*/) discard;

	distance = abs(sqrt(input.tex.x*input.tex.x + input.tex.y*input.tex.y) - 1.0);
	if (distance > clipDistance)
		discard;

	float alpha = 1.0f;

	if (AAMode == 2/*GAUSS*/)
	{
		alpha = normalFunc(distance/input.tolerance);
	}
	else if (AAMode == 1/*MSAA*/)
	{
		if (samplelvl != 0)
		{
			float4 minmax;
			minmax[0] = input.tex.x-input.tolerance;
			minmax[1] = input.tex.y-input.tolerance;
			minmax[2] = input.tex.x+input.tolerance;
			minmax[3] = input.tex.y+input.tolerance;
			float step = input.tolerance/samplelvl;
			float2 samplePos = float2(minmax[0]+step, minmax[1]+step);
			float contrib = 1.0/samplelvl/samplelvl;

			alpha = 0.0;
			while (1)
			{
				while (1)
				{
					if (abs(sqrt(samplePos.x*samplePos.x + 
						samplePos.y*samplePos.y) - 1.0) < input.tolerance)
						alpha += contrib;
					samplePos.y += step*2;
					if (samplePos.y > minmax[3]) break;
				}
				samplePos.x += step*2;
				if (samplePos.x > minmax[2]) break;

				samplePos.y = minmax[1] + step;
			}
		}
	}
	return float4(input.color, alpha);
}
