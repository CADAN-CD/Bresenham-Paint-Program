#include "CircleMidPoint.h"
#include <cmath>
std::vector<float> CircleMidPoint(float cx, float cy, int r, int width, int height) {
    std::vector<float> pts;
    //x,y are the coordinates of the first octant 
    int x = 0;
    int y = r;
    int p = 1 - r;

    while (x <= y) {
        pushPoint(pts, (float)x, (float)y, cx, cy, width, height);
        //we move foward
        if (p < 0) {
            p += 2 * x + 3;
        }
        //we move down and then foward
        else {
            y -= 1;
            p += 2 * (x - y) + 5;
        }

        x++;
    }

    return pts;
}
// cx, cy is for the center of the circle 
// px,py is the point in the center of the pixel
//cx + px  = center + offset  =   absolute point in the screen
void pushPoint(std::vector<float>& pts, float px, float py, float cx, float cy, int width, int height) {
	// fx, fy coordinates relative to the center of the circle, so we can use them to calculate the absolute coordinates of the point in the screen
	//auto push is a small function that takes arbitrary coordinates and automatically converts them to the range of [-1, 1]
	auto push = [&](float fx, float fy) {
		//convert the coordinates to the range of [-1, 1] for OpenGL
		pts.push_back(((cx + fx) / (width * 0.5f)) - 1.0f);
		pts.push_back(((cy - fy) / (height * 0.5f)) - 1.0f);
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
