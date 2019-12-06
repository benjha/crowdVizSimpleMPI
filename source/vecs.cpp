//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#include <math.h>
#include "vecs.h"

void unitVector(float *vector){
	float absVal = vecMagnitude(vector);
	if(absVal > 0){
		vector[0] /= absVal;
		vector[1] /= absVal;
		vector[2] /= absVal;
	}
}

float cosAngTwoVecs(float *vec1, float *vec2){
	float dp, sqs1, sqs2;
	dp = dotProduct(vec1, vec2);
	sqs1 = vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2];
	sqs2 = vec2[0] * vec2[0] + vec2[1] * vec2[1] + vec2[2] * vec2[2];
	return sqrt(dp * dp / (sqs1 * sqs2));
}

float angTwoVecs(float *vec1, float *vec2){
	return acos(cosAngTwoVecs(vec1, vec2));
}

void axisToQuat(float *axis, float angle, float *quat){
	float cosAng, sinAng;
	cosAng = cos(angle);
	sinAng = sin(angle);
	quat[0] = cosAng;
	quat[1] = axis[0] * sinAng;
	quat[2] = axis[1] * sinAng;
	quat[3] = axis[2] * sinAng;
}

void quatToAxis(float *quat, float *axis, float *angle){
	float sinAng;
	sinAng = sqrt(1 - quat[0] * quat[0]);
	*angle = acos(quat[0]) * 360.0f / PI;
	if(sinAng != 0){
		axis[0] = quat[1] / sinAng;
		axis[1] = quat[2] / sinAng;
		axis[2] = quat[3] / sinAng;
	} else {
		axis[0] = 1;
		axis[0] = 0;
		axis[0] = 0;
	}
}

void axisToEuler(float *axis, float angle, float *euler){
	float q[4];
	axisToQuat(axis, angle, q);
	euler[0] = atan2(2*q[2]*q[3], q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3]);
	euler[1] = asin(-2*(q[1]*q[3] - q[0]*q[2]));
	euler[2] = atan2(2*(q[1]*q[2]+q[0]*q[3]), q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3]);
}

void rotatePointAroundAxis(float *point, float *axis, float angle, float *rotPoint){
	float quat[4];
	axisToQuat(axis, angle, quat);
	rotatePointByQuat(point, quat, rotPoint);
}

void rotatePointByQuat(float *point, float *quat, float *rotPoint){
	float quatInv[4], quatPoint[4], quatAux1[4], quatAux2[4];
	unitQuat(quat);
	quatPoint[0] = 0;
	quatPoint[1] = point[0];
	quatPoint[2] = point[1];
	quatPoint[3] = point[2];
	unitQuat(quatPoint);
	quatInverse(quat, quatInv);
	quatMult(quat, quatPoint, quatAux1);
	quatMult(quatAux1, quatInv, quatAux2);
	rotPoint[0] = quatAux1[1];
	rotPoint[1] = quatAux1[2];
	rotPoint[2] = quatAux1[3];

}

void quatMult(float *q1, float *q2, float *res){

	float A, B, C, D, E, F, G, H;

	A = (q1[0] + q1[1])*(q2[0] + q2[1]);
	B = (q1[3] - q1[2])*(q2[2] - q2[3]);
	C = (q1[0] - q1[1])*(q2[2] + q2[3]); 
	D = (q1[2] + q1[3])*(q2[0] - q2[1]);
	E = (q1[1] + q1[3])*(q2[1] + q2[2]);
	F = (q1[1] - q1[3])*(q2[1] - q2[2]);
	G = (q1[0] + q1[2])*(q2[0] - q2[3]);
	H = (q1[0] - q1[2])*(q2[0] + q2[3]);


	res[0] = B + (-E - F + G + H) /2;
	res[1] = A - (E + F + G + H)/2; 
	res[2] = C + (E - F + G - H)/2; 
	res[3] = D + (E - F - G + H)/2;
}

void quatInverse(float *quat, float *inv){
	inv[0] =  quat[0];
	inv[1] = -quat[1];
	inv[2] = -quat[2];
	inv[3] = -quat[3];
}

void unitQuat(float *quat){
	float absVal = quatMagnitude(quat);
	if(absVal > 0){
		quat[0] /= absVal;
		quat[1] /= absVal;
		quat[2] /= absVal;
		quat[3] /= absVal;
	}
}

/******************************************************************************/

