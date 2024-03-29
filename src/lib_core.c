#include "lib_core.h"

int randomNum(int range_start, int range_end) {
    struct timespec tp;
    clockid_t clk_id;
    clk_id = CLOCK_MONOTONIC;
    clock_gettime(clk_id, &tp);
    srand(tp.tv_nsec);
    return (rand() % (range_end - range_start + 1)) + range_start;
}

float limitNum(float number, float limit) {
	int vorzeichen = (number < 0) ? -1 : 1;
	float numberMag = number * vorzeichen;
	if (numberMag > limit) numberMag = limit;

	return numberMag * vorzeichen;
}

float constrainNum(float value, float min, float max) {
  return fminf(max, fmaxf(min, value));
}

float mapNum(float n, float range_old, float range_new) {
  return (n / range_old * range_new);
}

Vector2 Vec2Scale(Vector2 v, float n) {
	return (Vector2) {v.x * n, v.y * n};
}

Vector2 Vec2Div(Vector2 v, float n) {
	Vector2 result = {v.x/n, v.y/n};
	return result;

}

Vector2 Vec2Add(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x + v2.x, v1.y + v2.y};
	return result;
}

Vector2 Vec2Sub(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x - v2.x, v1.y - v2.y};
	return result;
}

Vector2 Vec2Normalize(Vector2 v) {
    Vector2 result = {0};
    float len = sqrtf((v.x*v.x) + (v.y*v.y));
    if (len > 0.001) {
        result.x = v.x / len;
        result.y = v.y / len;
    }
    return result;	
}

Vector2 Vec2Limit(Vector2 v, float max) {
	Vector2 result = v;
	float len = sqrtf(result.x * result.x + result.y * result.y);
	if (len > max) {
		result.x = result.x / len * max;
		result.y = result.y / len * max;
	}
	return result;
}

Vector2 Vec2SetMag(Vector2 v, float magnitude) {
    Vector2 result = {0};
    float len = sqrtf((v.x*v.x) + (v.y*v.y));
    if (len > 0.001) {
        result.x = v.x / len * magnitude;
        result.y = v.y / len * magnitude;
    }
    return result;	

}

float Vec2DotProduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float Vec2CrossPoduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Vector2 Vec2Perp(Vector2 v) {
	return (Vector2){-v.y, v.x};
}

float Vec2Mag(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float Vec2Magsq(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

float Vec2Dist(Vector2 v1, Vector2 v2) {
	Vector2 vdist = Vec2Sub(v1, v2);
	return Vec2Mag(vdist);
}

Vector2 Vec2Rotate(Vector2 v, Vector2 base, float n) {
	Vector2 direction = Vec2Sub(v, base);
	float x = direction.x * cosf(n) - direction.y * sinf(n);
	float y = direction.x * sinf(n) + direction.y * cosf(n);
	return (Vector2){x = x + base.x, y = y + base.y};
}

float Vec2Heading(Vector2 v) {
	return atan2f(v.y, v.x);
}

float Vec2Angle(Vector2 v1, Vector2 v2) {
    float result = 0.0f;
    float dot = v1.x*v2.x + v1.y*v2.y;
    float det = v1.x*v2.y - v1.y*v2.x;
    result = atan2f(det, dot);

    return result;
}

float minDistance(Vector2 p, Vector2 start_a, Vector2 end_a) {
	float dist = -1.0f;

	//Vektor start_a to end_a (line_a)
	Vector2 line_a = Vec2Sub(end_a, start_a);
	//Vektor imaginary line start_a to p
	Vector2 start_a_to_p = Vec2Sub(p, start_a);
	//Magnitude of line_a
	float magnitude = Vec2Mag(line_a);

	//Scalarprojecton from line (start_a to p) on line_a
	line_a = Vec2Normalize(line_a);
	float sp = Vec2DotProduct(line_a, start_a_to_p);

	//Scalarprojection in magnitude of line_a?
	if (sp > 0.0001 && sp <= magnitude) {
		line_a= Vec2Scale(line_a, sp);
		dist = Vec2Mag(Vec2Sub(start_a_to_p, line_a));
	}
	return dist;
}