#include "bresenham.h"
#include <cmath>
//                                    starting point(x1,y1) final point(x2,y2) height and widht of the screen
std::vector<float> bresenhamLine(float x1, float y1, float x2, float y2, int width, int height) {
    std::vector<float> pts;

    int dx = (int)(x2 - x1); //x steps
    int dy = (int)(y2 - y1); //y steps
    //direction of the line
    int sx = (dx >= 0) ? 1 : -1;  // left or right direction
    int sy = (dy >= 0) ? 1 : -1;  // up or down direction

    //absolute magnitudes that decide the slop
    int adx = std::abs(dx);  
    int ady = std::abs(dy);  

    //we start from the first point
    //we store it so we can return it at the end of the function without damaging the original value of x1 and y1
    float x = x1, y = y1;

    // dominante X (octantes 1,4,5,8)
    if (adx >= ady) {
		//desicion parameter that decides when to step in the y direction 
        int p = 2 * ady - adx;
		//we loop through the x steps and decide when to step in the y direction
        for (int i = 0; i <= adx; i++) {
            // divides to tell openGL which percentage
                // of the width and height we are at,
                // then multiplies by 2 to get the range from -1 to 1, 
                // then subtracts 1 to shift the range from 0..2 to -1..1
            //                                      floats bc c++ works with doubles by default 
            pts.push_back((x / width) * 2.0f - 1.0f);
            pts.push_back((y / height) * 2.0f - 1.0f);
            if (p >= 0) {
				//the actual point is not on the line, so we sum the step in
                // the y direction to get the actual point on the line
                y += sy;
				//we update the desicion parameter to decide when to step in the y direction again
                p += 2 * ady - 2 * adx; 
            }
            else { 
                //we move forward x direction without changing the y direction
                p += 2 * ady; 
            }
			//moving foward x direction
            x += sx;
        }
    }
    //dominante Y (octantes 2,3,6,7)
    else {
        int p = 2 * adx - ady;
        for (int i = 0; i <= ady; i++) {
            pts.push_back((x / width) * 2.0f - 1.0f);
            pts.push_back((y / height) * 2.0f - 1.0f);
            if (p >= 0) { 
				//the actual point is not on the line, so we sum the step in
				// the x direction to get the actual point on the line
                x += sx;
				//we update the desicion parameter to decide when to step in the x direction again
                p += 2 * adx - 2 * ady; 
            }
            else { 
				//we move forward y direction without changing the x direction
                p += 2 * adx; 
            }
            //moving foward y direction
            y += sy;
        }
    }

    return pts;
}
	

