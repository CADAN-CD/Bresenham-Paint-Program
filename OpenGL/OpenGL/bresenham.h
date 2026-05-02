#pragma once //include this file once
//library for std::vector
#include <vector>

//returns the points of the line (-1..1)

std::vector<float> bresenhamLine(float x0, float y0,
    float x1, float y1,
    int width, int height);
