////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer PixelBufferType
{
	float4 color;
	float sigma_2;
	float clipEdge;
	float thet;
	int AAMode;
	int samplelvl;
	int noise;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 color : COLOR;
	float scale:	SCALE;
};

float normalFunc(float x)
{
	return thet*exp((x*x)/sigma_2);
}

float normalFuncRandom(float x, float2 pos)
{
	float tmp = sqrt(pos.x*pos.y);
	return thet * exp((x*x)/sigma_2*((tmp*100%97+tmp*100000%131)%noise+100-noise/2)/100);
}

void drawLine(float k, float2 pos, float2 start, float2 end, inout float count, inout float3 pix)
{
	float a = (start.y - end.y);
	float b = -(start.x - end.x);
	float quad = sqrt(a*a+b*b);

	a /= quad;	b /= quad;
	float c = -(start.x*a+start.y*b);
	
	float fx = abs(pos.x*a+pos.y*b+c);

	if (fx > clipEdge/2/k) return;

	float alpha = saturate(normalFunc(fx*k*2.0));
	count *= (1-alpha);
	pix = pix*(1-alpha)+color.xyz*alpha;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	float count = 1.0f;
	float3 res = float3(0, 0, 0);

	// draw lines
	drawLine(input.scale, input.tex, float2(0, 0), float2(1, 1), count, res);
	drawLine(input.scale, input.tex, float2(0, 0), float2(1, -1), count, res);
	drawLine(input.scale, input.tex, float2(0, 200), float2(10, 200), count, res);
	drawLine(input.scale, input.tex, float2(200, 0), float2(200, -1), count, res);
	drawLine(input.scale, input.tex, float2(-200, 0), float2(-202, -100), count, res);
	drawLine(input.scale, input.tex, float2(0, -100), float2(200, -101), count, res);

	if (count > 0.9999)
	{
		discard;
	}
	if (count < 1e-6) return float4(res, 1.0);
	return float4(res/count, 1-count);
}
