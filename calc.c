#include <stdio.h>

typedef struct
{
	float x, y;	
} vec2;

vec2 add(vec2 v1, vec2 v2)
{
	vec2 res;
	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	return res;
};

vec2 sub(vec2 v1, vec2 v2)
{
	vec2 res;
	res.x = v1.x - v2.x;
	res.y = v1.x - v1.y;
	return res;
}

vec2 mul(vec2 v1, vec2 v2)
{
	vec2 res;
	res.x = v1.x * v2.x;
	res.y = v1.x * v1.y;
	return res;
}

vec2 mult(vec2 v1, int i)
{
	vec2 res;
	res.x = v1.x*i;
	res.y = v1.y*i;
	return res;
}

vec2 div(vec2 v1, vec2 v2)
{
	vec2 res;
	res.x = v1.x / v2.x;
	res.y = v1.x / v1.y;
	return res;
}

void print_vec2(vec2 v)
{
	printf("%f, %f\n", v.x, v.y);
}

int main()
{
	
	vec2 aPos[4] = {
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f}
	};
	vec2 iPos = {0.0f, 500.0f};
	vec2 uRes = {800.0f, 600.0f};
	vec2 iSize = {100.0f, 100.0f};

	vec2 temp1;
	vec2 temp2;
	for(int i = 0; i < 4; i++)
	{	
		print_vec2((vec2){2 * (iPos.x / uRes.x - 0.5f) + aPos[i].x * (iSize.x/uRes.x) * 2, 2 * (iPos.y / uRes.y - 0.5f) + aPos[i].y * (iSize.y/uRes.y) * 2});
	}
	
	return 0;
}
