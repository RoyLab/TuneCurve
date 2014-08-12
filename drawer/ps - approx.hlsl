////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
//cbuffer PixelBufferType
//{
//	float sigma_2;
//	float clipEdge;
//	float thet;
//	int AAMode;
//	int samplelvl;
//	int noise;
//};

cbuffer PixelBufferType
{
	 float4 coef;
	 float c;
	 float width;
	 int AAMode;
}

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 color : COLOR;
};

float f(float2 pos)
{
	return coef[0]*pos.x*pos.x+coef[1]*pos.y*pos.y+coef[2]*pos.x+coef[3]*pos.y+c;
}

float2 f1(float2 pos)
{
	return float2(2*pos.x*coef[0]+coef[2], 2*pos.y*coef[1]+coef[3]);
}

float3 f2(float2 pos)
{
	return float3(1.41421356*coef[0], 0, 1.41421356*coef[1]);
	//return float3(2*coef[0], 0, 2*coef[1]);
}

float4 PMain(PixelInputType input) : SV_TARGET
{
	float p1 = length(f1(input.tex));
	float p2 = length(f2(input.tex));
	float p0 = abs(f(input.tex));
	float tmp = p1/p2/2.0f;

	//float d = sqrt(tmp*tmp + p0/p2)-tmp;
	float d = abs(f(input.tex.xy)/length(f1(input.tex)));

	if (d > width) discard;
	if (d < 0.5) return float4(1, 0, 0, 1);
	if (abs(input.tex.x - input.tex.y) < 1.0)
		return float4(0,1,0,1);
	return float4(1, 1, 1, 1);
}



//float normalFunc(float x)
//{
//	return thet*exp((x*x)/sigma_2);
//}
//
//float normalFuncRandom(float x, float2 pos)
//{
//	return thet * exp((x*x)/sigma_2*((pos.x*pos.x+pos.y*pos.y)%noise+100-noise/2)/100);
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// Pixel Shader
//////////////////////////////////////////////////////////////////////////////////
//float4 PMain(PixelInputType input) : SV_TARGET
//{
//	float clipDistance = input.tolerance*clipEdge;
//	float distance = abs(input.tex.x)+abs(input.tex.y);
//	if (distance < 1.0f-clipDistance) discard;
//	if (distance > (1.0f+clipDistance)*1.41421356/*sqrt(2.0)*/) discard;
//
//	distance = abs(sqrt(input.tex.x*input.tex.x + input.tex.y*input.tex.y) - 1.0);
//	if (distance > clipDistance)
//		discard;
//
//	float alpha = 1.0f;
//
//	if (AAMode == 2/*GAUSS*/)
//		alpha = normalFunc(distance/input.tolerance);
//	else if (AAMode == 3)
//		alpha = normalFuncRandom(distance/input.tolerance, input.position.xy);
//	else if (AAMode == 1/*MSAA*/)
//	{
//		if (samplelvl != 0)
//		{
//			float4 minmax;
//			minmax[0] = input.tex.x-input.tolerance;
//			minmax[1] = input.tex.y-input.tolerance;
//			minmax[2] = input.tex.x+input.tolerance;
//			minmax[3] = input.tex.y+input.tolerance;
//			float step = input.tolerance/samplelvl;
//			float2 samplePos = float2(minmax[0]+step, minmax[1]+step);
//			float contrib = 1.0/samplelvl/samplelvl;
//
//			alpha = 0.0;
//			while (1)
//			{
//				while (1)
//				{
//					if (abs(sqrt(samplePos.x*samplePos.x + 
//						samplePos.y*samplePos.y) - 1.0) < input.tolerance)
//						alpha += contrib;
//					samplePos.y += step*2;
//					if (samplePos.y > minmax[3]) break;
//				}
//				samplePos.x += step*2;
//				if (samplePos.x > minmax[2]) break;
//
//				samplePos.y = minmax[1] + step;
//			}
//		}
//	}
//	return float4(input.color, alpha);
//}
