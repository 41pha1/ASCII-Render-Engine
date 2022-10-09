#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <sys/time.h>

#include "Vector.h"

using namespace std;

const int resy = 49;
const int resx = 100;
float screen[resx][resy];

char gradient[] = " .:;+=xX$&";
int gradients = sizeof(gradient)/sizeof(gradient[0]);

const Vector worldUp(0,1,0);

const int maxIts = 100;
const float maxDis = 10;
const float minDis = 0.01;

double iTime = 0;

char toChar(float value)
{
	int i = (int) (value * (gradients-2));

	if(i == 0 && value != 0)
		i = 1;

	return gradient[i];
}

void printImage()
{
	for(int y = 0; y < resy; y++)
		cout << "\e[A";

	string text;
	for(int y = 0; y < resy; y++)
	{
		for(int x = 0; x < resx; x++)
		{
			char c = toChar(screen[x][resy-y-1]);
			text.append(string(1, c));
		}
		text.append("\n");
	}
	cout << text;
}

float sdBox(Vector pos)
{
	Vector ap(abs(pos.x), abs(pos.y), abs(pos.z));
	Vector q = ap.sub(Vector(0.5, 0.5, 0.5));
	float f = Vector(max(q.x, 0.), max(q.y, 0.), max(q.z, 0.)).length();

	f += min(max(q.x,max(q.y,q.z)),0.0);

	return f;
}

float sd(Vector pos)
{
	pos = pos.rotateX(iTime*0.6);
	pos = pos.rotateZ(iTime*0.3);
	pos = pos.rotateY(iTime*0.1);
	float qx = sqrt(pos.x*pos.x+pos.z*pos.z) - 0.5;
	float sd = sqrt(qx*qx+pos.y*pos.y)-0.15;
	return sd;
//	return sdBox(pos);
//	return pos.length()-0.5;
}

Vector calcNormal(Vector pos)
{
	float epsilon = 0.01;

	Vector p1 = pos.add(Vector(epsilon, 0, 0));
	Vector p2 = pos.sub(Vector(epsilon, 0, 0));
	float x = sd(p1) - sd(p2);

	p1 = pos.add(Vector(0, epsilon, 0));
	p2 = pos.sub(Vector(0, epsilon, 0));
	float y = sd(p1) - sd(p2);

	p1 = pos.add(Vector(0, 0, epsilon));
	p2 = pos.sub(Vector(0, 0, epsilon));
	float z = sd(p1) - sd(p2);

	Vector normal(x,y,z);

	return normal.normalized();
}

float march(Vector ro, Vector rd)
{
	Vector cp = ro;
	float totDis = 0;
	for(int i = 0; i < maxIts; i++)
	{
		float dis = sd(cp);

		totDis += dis;

		if(dis < minDis)
			break;

		if(totDis > maxDis)
			return maxDis;

		Vector step = rd.mul(dis);
		cp = cp.add(step);
	}
	return totDis;
}

float getPixel(int x, int y)
{
	Vector lightPos(-1, 1, 0);
	Vector ro(-1.3, 0, 0);
	Vector ta(0,0,0);

	Vector front = ta.sub(ro);
	Vector right = front.cross(worldUp);
	Vector top = right.cross(front);

	float uvx = (x-(resx/2.))/(resy*3.1);
	float uvy = (y-(resy/2.))/resy;
	float zoom = 1;

	Vector nf = front.mul(zoom);
	Vector nr = right.mul(uvx);
	Vector nt = top.mul(uvy);

	Vector sum = nf.add(nr);
	sum = sum.add(nt);
	Vector rd = sum.normalized();

	float dis = march(ro, rd);
	Vector toPoint = rd.mul(dis);
	Vector pos = ro.add(toPoint);
	Vector normal = calcNormal(pos);
	Vector toLight = lightPos.sub(pos);

	float dif = normal.dot(toLight)*0.7;
	float value = dif;

	if(value < 0)
		value = 0.01;
	if(value > 1)
		value = 1;

	if(dis == maxDis)
		value = 0;

	return value;
}

void renderImage()
{
	for(int x = 0; x < resx; x++)
	{
		for(int y = 0; y < resy; y++)
		{
			screen[x][y] = getPixel(x,y);
		}
	}
}


int main()
{
	while(true)
	{
		renderImage();
		printImage();
		usleep(20000);
		iTime = iTime + 0.2;
	}
	return 0;
}
