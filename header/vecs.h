#ifndef __vecs_h
#define __vecs_h

#include <memory>
#include <cmath>
#include <iostream>
using namespace std;

#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))

#ifndef PI
#define PI 3.141593
#endif

#define dotProduct(vec1, vec2)	vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]
#define vecMagnitude(vector) sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2])

void crossProduct(float *, float *, float *);

#define crossProduct(vec1, vec2, res) { res[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1]; res[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2]; res[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0]; }

void unitVector(float *);
float cosAngTwoVecs(float *vec1, float *vec2);
float angTwoVecs(float *vec1, float *vec2);

// Quaternions
#define quatMagnitude(quat) sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] * quat[3])
void axisToQuat(float *axis, float ane, float *quat);
void quatToAxis(float *quat, float *axis, float *angle);
void axisToEuler(float *axis, float angle, float *euler);
void quatMult(float *q1, float *q2, float *res);
void unitQuat(float *quat);
void quatInverse(float *quat, float *inv);
void rotatePointAroundAxis(float *point, float *axis, float angle, float *rotPoint);
void rotatePointByQuat(float *point, float *quat, float *rotPoint);

/******************************************************************************/



#endif