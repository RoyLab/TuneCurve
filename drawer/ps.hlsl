////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
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
}

float getDegree(float2 pos, float2 bound)
{
	const float PI = 3.141592654f;
	float x = atan(pos.y/pos.x);
	if (pos.x < 0) x += PI;
	while (x < bound[0]) x += 2.0f*PI;
	while (x > bound[1]) x -= 2.0f*PI;
	return x;
}


float4 PMain(PixelInputType input) : SV_TARGET
{
	const float PI = 3.141592654f;
	const float m = sqrt(-c/coef[0]);
	const float n = sqrt(-c/coef[1]);
	float fx = f(input.tex);

	float thetLow, thetHigh;
	float2 bound;
	if (input.tex.x > 0)
	{
		if (input.tex.y > 0)
		{
			thetLow = 0.0f;
			thetHigh = PI/2.0f;
			bound = float2(-PI/2.0, PI);
		}
		else
		{
			thetLow = -PI/2.0f;
			thetHigh = 0.0f;
			bound = float2(-PI, PI/2.0);
		}
	}
	else
	{
		if (input.tex.y > 0)
		{
			thetLow = PI/2.0f;
			thetHigh = PI;
			bound = float2(0.0f, 1.5f*PI);
		}
		else
		{
			thetLow = -PI;
			thetHigh = -PI/2.0f;
			bound = float2(-1.5*PI, 0.0f);
		}
	}


	float thet = (thetLow + thetHigh) / 2.0f;
	const int CYCLE_COUNT = 15;
	for (int i = 0; i < CYCLE_COUNT; i++)
	{
		float2 curPos = float2(m*cos(thet), n*sin(thet));
		float2 ratioVec = (fx > 0)?(input.tex-curPos):(curPos-input.tex);
		float ratio = getDegree(ratioVec, bound);
		float ratioNormal = getDegree(float2(coef[0]*curPos.x, coef[1]*curPos.y), bound);

		if (ratio < ratioNormal)
		{
			if (fx > 0)
				thetHigh = thet;
			else
				thetLow = thet;
		}
		else
		{
			if (fx > 0)
				thetLow = thet;	
			else
				thetHigh = thet;
		}
		thet = (thetLow + thetHigh) / 2.0f;
	}

	float p1 = length(f1(input.tex));
	float p2 = length(f2(input.tex));
	float p0 = abs(f(input.tex));
	float tmp = p1/p2/2.0f;

	float d;
	if (AAMode == 1) d = sqrt(tmp*tmp + p0/p2)-tmp;
	else d = abs(f(input.tex.xy)/length(f1(input.tex)));
	float4 res = float4(0, 0, 0, 0);
	if (d < width) res += float4(1, 0, 0, 1);

	d = length(input.tex-float2(m*cos(thet), n*sin(thet)));
	if (d < width) 
	{
		if (res.a > 0.9f)
			res = float4(1, 1, 0, 1);
		else 
			res += float4(0, 1, 0, 1);
	}
	if (d < 0.5f) return float4(0, 0, 1, 1);
	res.a = 1.0f;
	return res;
}