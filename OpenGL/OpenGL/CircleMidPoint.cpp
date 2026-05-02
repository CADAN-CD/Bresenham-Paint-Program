#include "CircleMidPoint.h"
#include <cmath>
std::vector<float> CircleMidPoint(float cx, float cy, int r, int width, int height) {
	std::vector<float> pts;
	int x, y, p;
	x = 0;
	y = -r;
	p = -r;
	while (x < -y) {
		pushPoint(pts, (float)x, (float)y, cx, cy, width, height);
		p >= 0 ? y += 1, p += 2 * x + 3 : p += 2*x+1;
		x += 1;
	}
	
}
// cx, cy is for the center of the circle 
// px,py is the point in the center of the pixel
//cx + px  = center + offset  =   absolute point in the screen
void pushPoint(std::vector<float>& pts, float px, float py, float cx, float cy, int width, int height) {
	// fx, fy coordinates relative to the center of the circle, so we can use them to calculate the absolute coordinates of the point in the screen
	//auto push is 
	auto push = [&](float fx, float fy) {
		//converts to NCD coordinates and pushes to the vector of points
		pts.push_back(((cx + fx) / width) * 2.0f - 1.0f);
		pts.push_back(((cy + fy) / height) * 2.0f - 1.0f);
		};
	push(px, -py); // Oct 1
	push(py, -px); // Oct 2
	push(py, px); // Oct 3
	push(px, py); // Oct 4
	push(-px, py); // Oct 5
	push(-py, px); // Oct 6
	push(-py, -px); // Oct 7
	push(-px, -py); // Oct 8
}
