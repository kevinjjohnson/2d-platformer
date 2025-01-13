#include "include/collison.h"
#include <float.h>
#include <math.h>

float intervalDistance(float minA, float maxA, float minB, float maxB) {
	if (minA < minB)
		return minB - maxA;
	else
		return minA - maxB;
}

void projectPolygon(vec2 axis, point* points, int size, float* min, float* max) {
	if (size <= 0) {
		//assert("FUCK YOU");
		size = 1;
	}
	float dp;
	dp = glm_vec2_dot(axis, (vec2){points[0].x, points[0].y});
	//printf("\ndot product is: %f\n", dp);
	*min = dp;
	*max = dp;
	int i;
	for (i = 0; i < size; i++) {
		dp = glm_vec2_dot(axis, (vec2) { points[i].x, points[i].y });
		if (dp < *min)
			*min = dp;
		else if (dp > *max)
			*max = dp;
	}
}

bool isColliding(point* polygon1, int size1, vec2 center1, point* polygon2, int size2, vec2 center2, vec2* translationVector) {
	if (size1 <= 1 || size2 <= 1)
		//assert("kill yourself one of those is a point");
		size1 = .5;
	float minInterval = FLT_MAX;
	int i;
	point p1, p2;
	edge e;
	vec2 finalAxis = { 0.0f, 0.0f };
	vec2 axis = { 0.0f, 0.0f };
	float minA, maxA, minB, maxB, dist;
	for (i = 0; i < size1 + size2; i++) {
		if (i < size1) {
			p1 = polygon1[i];
			if (i == size1 - 1) {
				p2 = polygon1[0];
			} else {
				p2 = polygon1[i + 1];
			}
		} else {
			p1 = polygon2[i - size1];
			if (i - size1 == size2 - 1) {
				p2 = polygon2[0];
			}else {
				p2 = polygon2[i - size1 + 1];
			}
		}
		//probably refactor this later but for now its fine whatever
		//also check if negatives will fuck shit up idk
		e.x = p2.x - p1.x;
		e.y = p2.y - p1.y;
		axis[0] = -e.y;
		axis[1] = e.x;
		glm_vec2_normalize(axis);

		projectPolygon(axis, polygon1, size1, &minA, &maxA);
		projectPolygon(axis, polygon2, size2, &minB, &maxB);
		//if they arent intersecting on one axis then they arent on any so return early;
		dist = intervalDistance(minA, maxA, minB, maxB);
		if (dist > 0)
			return false;
		//should be negative so make it positive again 
		dist = fabs(dist);
		if (dist < minInterval) {
			minInterval = dist;
			finalAxis[0] = axis[0];
			finalAxis[1] = axis[1];
			vec2 d;
			glm_vec2_sub(center1, center2, d);
			if (glm_vec2_dot(d, finalAxis) < 0) {
				glm_vec2_scale(finalAxis, -1.0f, finalAxis);
				printf("axis inverted");
			}
		}
	}
	printf("TRANSLATION AXIS: (%f, %f)\n", finalAxis[0], finalAxis[1]);
	glm_vec2_scale(finalAxis, minInterval, *translationVector);
	return true;
}