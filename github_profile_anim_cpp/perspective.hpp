#pragma once
#include "matrix.hpp"
#include <math.h>
#ifndef M_PI
	#include <corecrt_math_defines.h>
#endif

const double vpwidth = 1600;
const double vpheight = 900;
const double aspect = vpwidth / vpheight;
const double fov = 120;
const double nearclip = 1e-4;
const double finalscale = 1.6;

double fract(double x) {
	return x - floor(x);
}

double clamp(double x, double min, double max) {
    return fmin(max, fmax(min, x));
}

Matrix makeCamMat(double yaw, double pitch, double roll, double x, double y, double z) {
    yaw *= M_PI / 180.0;
    pitch *= M_PI / 180.0;
    roll *= M_PI / 180.0;
    Matrix Ry(3, 3);
    Ry[0][0] = cos(yaw);  Ry[0][2] = -sin(yaw);
    Ry[1][1] = 1;
    Ry[2][0] = sin(yaw);  Ry[2][2] = cos(yaw);
    Matrix Rx(3, 3);
    Rx.clear();
    Rx[0][0] = 1;
    Rx[1][1] = cos(pitch); Rx[1][2] = sin(pitch);
    Rx[2][1] = -sin(pitch); Rx[2][2] = cos(pitch);
    Matrix Rz(3, 3);
    Rz.clear();
    Rz[0][0] = cos(roll); Rz[0][1] = sin(roll);
    Rz[1][0] = -sin(roll); Rz[1][1] = cos(roll);
    Rz[2][2] = 1;
    Matrix R = Ry * Rx * Rz;
    Matrix ans(4, 4);
    ans.clear();
    ans[0][0] = R[0][0]; ans[1][0] = R[0][1]; ans[2][0] = R[0][2];
    ans[0][1] = R[1][0]; ans[1][1] = R[1][1]; ans[2][1] = R[1][2];
    ans[0][2] = R[2][0]; ans[1][2] = R[2][1]; ans[2][2] = R[2][2];
    ans[3][0] = -(x * ans[0][0] + y * ans[1][0] + z * ans[2][0]);
    ans[3][1] = -(x * ans[0][1] + y * ans[1][1] + z * ans[2][1]);
    ans[3][2] = -(x * ans[0][2] + y * ans[1][2] + z * ans[2][2]);
    ans[3][3] = 1;
    return ans;
}

Matrix makePerspectiveMat(double aspect, double fov, double nearclip) {
    fov *= M_PI / 180.0;
    double f = 1.0 / tan(fov / 2.0);
    Matrix ans(4, 4);
    ans.clear();
    ans[0][0] = f / aspect;
    ans[1][1] = f;
    ans[2][2] = 0.0;
    ans[2][3] = 1.0;
    ans[3][2] = nearclip;
    return ans;
}

void lookAtAngles(double camX, double camY, double camZ, double targetX, double targetY, double targetZ, double& yaw, double& pitch) {
    double dx = targetX - camX;
    double dy = targetY - camY;
    double dz = targetZ - camZ;
    double distXZ = sqrt(dx * dx + dz * dz);
    yaw = atan2(dx, dz) * 180.0 / M_PI;
    pitch = atan2(dy, distXZ) * 180.0 / M_PI;
}

Matrix makeCamMatByTarget(double camX, double camY, double camZ, double targetX, double targetY, double targetZ, double roll) {
    Matrix ans(4, 4);
    ans.clear();
    double fx = targetX - camX;
    double fy = targetY - camY;
    double fz = targetZ - camZ;
    double fLen = sqrt(fx * fx + fy * fy + fz * fz);
    fx /= fLen; fy /= fLen; fz /= fLen;
    double rx = fz;
    double ry = 0;
    double rz = -fx;
    double rLen = sqrt(rx * rx + ry * ry + rz * rz);
    if (rLen < 0.0001) {
        rx = 1; ry = 0; rz = 0;
    }
    else {
        rx /= rLen; rz /= rLen;
    }
    double ux = fy * rz - fz * ry;
    double uy = fz * rx - fx * rz;
    double uz = fx * ry - fy * rx;

    if (fabs(roll) > 0.00001) {
        double s = sin(roll);
        double c = cos(roll);
        double rx_new = rx * c - ux * s;
        double ry_new = ry * c - uy * s;
        double rz_new = rz * c - uz * s;
        double ux_new = rx * s + ux * c;
        double uy_new = ry * s + uy * c;
        double uz_new = rz * s + uz * c;
        rx = rx_new; ry = ry_new; rz = rz_new;
        ux = ux_new; uy = uy_new; uz = uz_new;
    }
    ans[0][0] = rx;  ans[1][0] = ry;  ans[2][0] = rz;
    ans[0][1] = ux;  ans[1][1] = uy;  ans[2][1] = uz;
    ans[0][2] = fx;  ans[1][2] = fy;  ans[2][2] = fz;
    ans[3][0] = -(camX * rx + camY * ry + camZ * rz);
    ans[3][1] = -(camX * ux + camY * uy + camZ * uz);
    ans[3][2] = -(camX * fx + camY * fy + camZ * fz);
    ans[3][3] = 1.0;
    return ans;
}


void calculateClip(double *p1, double *p2, double *p) {
	double x1, y1, z1, w1, x2, y2, z2, w2;
	x1 = p1[0]; y1 = p1[1]; z1 = p1[2]; w1 = p1[3];
	x2 = p2[0]; y2 = p2[1]; z2 = p2[2]; w2 = p2[3];
    p[3] = -8.0; 
    p[7] = -8.0;
    double t_near = 0.0;
    double t_far = 1.0;
    static const int axis_idx[] = {0, 0, 1, 1, 2, 2};
    static const double signs[] = {1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
    double c1[] = {x1, y1, z1, w1};
    double c2[] = {x2, y2, z2, w2};
    for (int i = 0; i < 6; i++) {
        int a = axis_idx[i];
        double s = signs[i];
        double d1 = c1[3] + s * c1[a];
        double d2 = c2[3] + s * c2[a];
        if (d1 < 0 && d2 < 0) {
            return;
        } else if (d1 < 0) {
            double t = d1 / (d1 - d2);
            if (t > t_near) t_near = t;
        } else if (d2 < 0) {
            double t = d1 / (d1 - d2);
            if (t < t_far) t_far = t;
        }
    }
    if (t_near > t_far) return;
    int count = 0;
    if (t_near > 1e-9) {
        p[0] = x1 + t_near * (x2 - x1);
        p[1] = y1 + t_near * (y2 - y1);
        p[2] = z1 + t_near * (z2 - z1);
        p[3] = w1 + t_near * (w2 - w1);
        count++;
		p[7] = -16.0; 
    }
    if (t_far < (1.0 - 1e-9)) {
        if (count == 0 || (t_far - t_near) > 1e-9) {
            double *out = (count == 0) ? p : (p + 4);
            out[0] = x1 + t_far * (x2 - x1);
            out[1] = y1 + t_far * (y2 - y1);
            out[2] = z1 + t_far * (z2 - z1);
            out[3] = w1 + t_far * (w2 - w1);
        }
    }
}

Matrix applyPerspective(Matrix &points, bool clipTail) {
	double clipPoints[8];
	int len = clipTail ? points.height * 3 - 2 : points.height * 3;
	Matrix newPoints(len, 4);
	newPoints.clear();
	unsigned int newPointIdx = 0;
	for(unsigned int i = 0; i < points.height; i++) {
		const int nextIndex = i == points.height - 1 ? 0 : i + 1;
		calculateClip(points[i], points[nextIndex], clipPoints);
		memcpy(newPoints[newPointIdx], points[i], sizeof(double) * 4);
		newPointIdx++;
		if(newPointIdx >= len) break;
		if(clipPoints[7] > -7.0) {
			memcpy(newPoints[newPointIdx], clipPoints, sizeof(double) * 8);
			newPointIdx += 2;
		}
		else if(clipPoints[3] > -7.0) {
			if(clipPoints[7] > -15.0) {
				for(unsigned int p = 0; p < 4; p++) {
					newPoints[newPointIdx][p] = points[i][p];
				}
				newPointIdx++;
				memcpy(newPoints[newPointIdx], clipPoints, sizeof(double) * 4);
				newPointIdx++;
			}
			else {
				memcpy(newPoints[newPointIdx], clipPoints, sizeof(double) * 4);
				newPointIdx++;
				for(unsigned int p = 0; p < 4; p++) {
					newPoints[newPointIdx][p] = points[nextIndex][p];
				}
				newPointIdx++;
			}
		}
		else {
			for(unsigned int p = 0; p < 4; p++) {
				newPoints[newPointIdx][p] = points[i][p];
			}
			newPointIdx++;
			for(unsigned int p = 0; p < 4; p++) {
				newPoints[newPointIdx][p] = points[nextIndex][p];
			}
			newPointIdx++;
		}
	}
	for(unsigned int i = 0; i < newPoints.height; i++) {
		const double w = newPoints[i][3] < nearclip ? nearclip : newPoints[i][3];
		if (newPoints[i][0] > w) newPoints[i][0] = w;
		else if (newPoints[i][0] < -w) newPoints[i][0] = -w;
        if (newPoints[i][1] > w) newPoints[i][1] = w;
        else if (newPoints[i][1] < -w) newPoints[i][1] = -w;
		const double factor = finalscale / w;
		newPoints[i][0] *= factor;
		newPoints[i][1] *= factor;
		newPoints[i][2] *= factor;
	}
	return newPoints;
}

void swap(double &a, double &b) {
	double temp = a;
	a = b;
	b = temp;
}

Matrix cross(double *vec1, double *vec2) {//vec3 x vec3
	Matrix ans(1, 3);
	ans[0][0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	ans[0][1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	ans[0][2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	return ans;
}

double dot(double *vec1, double *vec2) {//vec3 * vec3
	return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

Matrix faceNormal(Matrix points, int center, int branch1, int branch2) {
	double tangent[3];
	double bitangent[3];
	for(unsigned int i = 0; i < 3; i++) {
		tangent[i] = points[branch1][i] - points[center][i];
		bitangent[i] = points[branch2][i] - points[center][i];
	}
	return cross(tangent, bitangent);
}

void normalize3(Matrix &vec) {
	double lenFactor = 1.0 / sqrt(dot(vec[0], vec[0]));
	vec[0][0] *= lenFactor;
	vec[0][1] *= lenFactor;
	vec[0][2] *= lenFactor;
}

Matrix mirror3(Matrix &points, Matrix normal, Matrix planePoint) {
    normalize3(normal);
    Matrix newPoints = points;
    Matrix::inPlaceBroadcastRowSub(newPoints, planePoint, newPoints);
    for(unsigned int i = 0; i < points.height; i++) {
        double d = dot(newPoints[i], normal[0]);
        for(unsigned int j = 0; j < 3; j++) {
            newPoints[i][j] = points[i][j] - 2.0 * d * normal[0][j];
        }
        newPoints[i][3] = 1.0;
    }
	return newPoints;
}
