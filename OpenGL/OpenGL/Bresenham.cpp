#include "bresenham.h"
#include <cmath>
std::vector<float> bresenhamLine(float x1, float y1, float x2, float y2, int width, int height){
	//vector to store the points of the line
	std::vector<float> pts;
	float  dx, dy, p, x, y;

	//dx and dy are the differences between the two points
	dx = x2 - x1; //total distance in x
	dy = y2 - y1; //total distance in y
	//start from begining point
	x = x1; 
	y = y1;
	//the p parameter decides if the line goes straight
	//or diagonally 
	p = 2 * dy - dx;
	//we need to reach x2 starting in x1, so we make a while loop
	while (x1 <= x2) {
		//convert the coordenates in pixels
			//normalize the x and y values to be between -1 and 1
		float pixelX = (x1 / (float)width) * 2.0f - 1.0f;
		float pixelY = (y / (float)height) * 2.0f - 1.0f;  
		//stores the prixel coordinates in the vector pts
		pts.push_back(pixelX);
		pts.push_back(pixelY);

		//bresenham's algorithm

		if (p < 0) {
			//if p<0 moves horizontaly x increases
			p = p + 2 * dy;
		}
		else {
			//if p>=0 moves diagonaly x and y increases
			y = y + 1; //we go up a pixel
			p = p + 2 * dy - 2 * dx;

		}
		x1 = x1 + 1; //always goes right in x
		
	}
	return(pts);
	}
	

