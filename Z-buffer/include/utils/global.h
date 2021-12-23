#ifndef GLOBAL_H
#define GLOBAL_H

const int IMAGE_WIDTH = 1024;
const int IMAGE_HEIGHT = 768;

const float PI = 3.14159265358979323846;

float getMaxOfThree(float a, float b, float c)
{
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

float getMinOfThree(float a, float b, float c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

#endif // !GLOBAL_H
