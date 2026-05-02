#pragma once
#include <vector>

void pushPoint(std::vector<float>& pts, float px, float py, float cx, float cy, int width, int height);
std::vector<float> CircleMidPoint(float cx, float cy, int r, int width, int height);