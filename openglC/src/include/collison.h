#pragma once
#include <cglm/cglm.h>

typedef struct {
    float x;
    float y;
}point;

typedef struct {
    float x;
    float y;
}edge;

float intervalDistance(float minA, float maxA, float minB, float maxB);


/*projects the polygon made of points onto the axis.returns an interval into min and max. points should be ordered so each one connects to the next one
in the array by an edge. Last point will connect to first point.
ex: 0--1
    |  |
    3--2
*/
void projectPolygon(vec2 axis, point* points, int size, float* min, float* max);

/*test to see if 2 polygons made up of 2 point arrays are colliding and puts the min translation vector into translation vector. points should be ordered so each one connects to the next one
in the array by an edge. Last point will connect to first point.
ex: 0--1
    |  |
    3--2
*/

bool isColliding(point* polygon1, int size1, vec2 center1, point* polygon2, int size2, vec2 center2, vec2* translationVector);