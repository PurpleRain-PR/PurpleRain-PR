#include <math.h>
#ifndef M_PI
	#include <corecrt_math_defines.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include "lumi.hpp"
#include "matrix.hpp"
#include "build.hpp"
#include "perspective.hpp"
#include "write.hpp"
#include "logos.cpp"

double anim_squareRot_0_util_h(double x, double k, double b) {
	return(fabs(floor(x / k) - x / k + b) + x / k + (1 - 2 * b) * floor(x / k) - b);
}

double anim_squareRot_0_util_f(double x, double k, double b, double s) {
	return((anim_squareRot_0_util_h(k * x, s, b) * k * M_PI_2) / (anim_squareRot_0_util_h(2 * k * M_PI, s, b)));
}

double anim_squareRot_0_util_p(double x, double k, double b, double s) {
	return -M_SQRT2 / 2 + sin(anim_squareRot_0_util_f(x - M_PI_2 * floor(x * M_2_PI), k, b, s) + 3 * M_PI_4) - M_SQRT2 * floor(x * M_2_PI);
}

double anim_squareRot_0_util_q1(double x, double r, double s, double k) {
	return s / k * (1 - pow(-k * x / s - floor(-k * x / s), r) + floor(k * x / s));
}

Matrix anim_squareRot_0(double w) {
	//0~2pi
	w += 1e-6;
	const double s = 2 * M_PI;
	const double b = 0.8;
	const double k = 4.0;
	const double r = 4.0;

	const double a = anim_squareRot_0_util_q1(w, r, s, k);
	Matrix v(4, 4);
	v.clear();

	for(int i = 0; i < 4; i++) {
		const double l = anim_squareRot_0_util_f(a, k, b, s) + M_PI_4 + M_PI_2 * (double)i;
		v[i][0] = cos(l);
		v[i][1] = sin(l);
	}

	Matrix center(1, 4);
	center.clear();
	center[0][0] = M_2_PI * M_SQRT2 * w + anim_squareRot_0_util_p(a, k, b, s);
	center[0][1] = fmax(fmax(v[0][0], v[1][0]), fmax(v[2][0], v[3][0]));

	Matrix S(4, 4);
	S.clear();
	Matrix::inPlaceBroadcastRowAdd(v, center, S);
	Matrix::inPlaceScale(S, M_SQRT2, S);
	for(int i = 0; i < 4; i++) {
		S[i][3] = 1;
		S[i][0] = -S[i][0];
	}
	return S;
}

Matrix anim_cam_0(double s) {
	return makeCamMatByTarget(
		0.5 * (6.0 - 13.0 * cos(0.35 * sin(s) + 0.01 * sin(2.0 * s))),
		1.3 + 0.7 * sin(2.0 * s) + (0.03 * sin(15.0 * s + 3.0 * sin(s)) + 0.005 * sin(50.0 * s)) * (pow(fabs(6.0 * sin(0.35 * sin(s))), 2.2) - 1.0),
		10.0 * sin(0.31 * sin(s)),
		(-1.0 + exp(-3.0 + 6.0 * cos(-0.64 * sin(s) + 0.01 * sin(2.0 * s)))),
		-0.3 * (1.0 + 0.7 * sin(2.0 * s + M_PI) + (0.05 * sin(15.0 * s + 3.0 * sin(s + M_PI)) + 0.005 * sin(50.0 * s + M_PI_2)) * (pow(fabs(6.0 * sin(0.35 * sin(s - M_PI_2))), 2.2) - 1.0)) + 1,
		-5.0 * sin(0.33 * sin(s)),
		0.1 * sin(s)
	);
}

double anim_blkShadow_0(double s) {
	const double t = s - floor(s * M_2_PI) * M_PI_2;
	return 1 - t * 4 * M_1_PI;
}

const int totalFrame = 360 * (12 / 4);

int main() {
	Matrix perspective = makePerspectiveMat(aspect, fov, nearclip);
	Matrix camRot;

	FILE *fp_blkleft = newPolyFile("blkleft");
	FILE *fp_blkright = newPolyFile("blkright");
	FILE *fp_blkup = newPolyFile("blkup");
	FILE *fp_blkdown = newPolyFile("blkdown");
	FILE *fp_blkfront = newPolyFile("blkfront");
	FILE *fp_blkback = newPolyFile("blkback");
	FILE *fp_axis = newPolyFile("axis");
	FILE *fp_light = newPolyFile("light");
	FILE *fp_wall = newPolyFile("wall");
	FILE *fp_blkshadow = newPolyFile("blkshadow");
	FILE *fp_blkbtmshadow = newPolyFile("blkbtmshadow");
	FILE *fp_blkshumir = newPolyFile("blkshumir");
	FILE *fp_blkshdmir = newPolyFile("blkshdmir");
	FILE *fp_blkshfmir = newPolyFile("blkshfmir");
	FILE *fp_blkshbmir = newPolyFile("blkshbmir");

	writePosHead(fp_blkleft);
	writePosHead(fp_blkright);
	writePosHead(fp_blkup);
	writePosHead(fp_blkdown);
	writePosHead(fp_blkfront);
	writePosHead(fp_blkback);
	writePosHead(fp_axis);
	writePosHead(fp_light);
	writePosHead(fp_wall);
	writePosHead(fp_blkshadow);
	writePosHead(fp_blkbtmshadow);
	writePosHead(fp_blkshumir);
	writePosHead(fp_blkshdmir);
	writePosHead(fp_blkshfmir);
	writePosHead(fp_blkshbmir);

	FILE *fp_blkl_opa = newPolyFile("blklopa");
	FILE *fp_blkr_opa = newPolyFile("blkropa");
	FILE *fp_blku_opa = newPolyFile("blkuopa");
	FILE *fp_blkd_opa = newPolyFile("blkdopa");
	FILE *fp_blkf_opa = newPolyFile("blkfopa");
	FILE *fp_blkb_opa = newPolyFile("blkbopa");
	FILE *fp_blkbtmsh_opa = newPolyFile("blkbtmshopa");

	writeOpaHead(fp_blkl_opa);
	writeOpaHead(fp_blkr_opa);
	writeOpaHead(fp_blku_opa);
	writeOpaHead(fp_blkd_opa);
	writeOpaHead(fp_blkf_opa);
	writeOpaHead(fp_blkb_opa);
	writeOpaHead(fp_blkbtmsh_opa);

	FILE *fp_gnddiff_rot = newPolyFile("gnddiffrot");

	writeRotHead(fp_gnddiff_rot);

	FILE *fp_gnddiff_pos = newPolyFile("gnddiffpos");

	writeTrsHead(fp_gnddiff_pos);

	FILE *fp_gnddiff_sca = newPolyFile("gnddiffsca");

	writeScaHead(fp_gnddiff_sca);

	FILE *fp_blkl_col = newPolyFile("blklcol");
	FILE *fp_blkr_col = newPolyFile("blkrcol");
	FILE *fp_blku_col = newPolyFile("blkucol");
	FILE *fp_blkd_col = newPolyFile("blkdcol");
	FILE *fp_blkf_col = newPolyFile("blkfcol");
	FILE *fp_blkb_col = newPolyFile("blkbcol");

	writeColHead(fp_blkl_col);
	writeColHead(fp_blkr_col);
	writeColHead(fp_blku_col);
	writeColHead(fp_blkd_col);
	writeColHead(fp_blkf_col);
	writeColHead(fp_blkb_col);

	char logoNameSuffixes[5][8] = {"", "blkfmir", "blkbmir", "blkumir", "blkdmir"};
	for(int i = 0; i < 5; i++) {
		openShapeGroupFile(logo_c, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_cpp, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_js, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_html, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_css, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_rust, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_go, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_octave, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_opengl, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_vulkan, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_desmos, logoNameSuffixes[i], i);
		openShapeGroupFile(logo_mc, logoNameSuffixes[i], i);
	}

	const double axis[] = {
		1,		0,		0,		1,
		.9,		0,		.1,		1,
		1.1,	0,		-.1,	1,
		1,		0,		0,		1,
		.9,	0,		-.1,	1,
		1.1,	0,		.1,	1,
		1,		0,		0,		1,
		-1,	0,		0,		1,
		0,		0,		0,		1,
		0,		1,		0,		1,
		0,		-1,	0,		1,
		0,		0,		0,		1,
		0,		0,		-1,	1,
		0,		0,		.9,	1,
		-.1,	0,		.9,	1,
		.1,	0,		.9,	1,
		-.1,	0,		1.1,	1,
		.1,	0,		1.1,	1
	};
	Matrix axis_model(18, 4);
	memcpy(axis_model.data, axis, axis_model.size * sizeof(double));

	const double light[] = {
		100,	4,		1,		1,
		100,	4,		-1,		1,
		100,	-0.1,	-1,	1,
		100,	-0.1,	1,		1
	};
	Matrix light_model(4, 4);
	memcpy(light_model.data, light, light_model.size * sizeof(double));

	const double wall[] = {
		100,	2000,	5000,	1,
		100,	2000,	-5000,	1,
		100,	0,		-5000,	1,
		100,	0,		5000,	1
	};
	Matrix wall_model(4, 4);
	memcpy(wall_model.data, wall, wall_model.size * sizeof(double));

	const double gndDiffuseLocator[] = {
		100,	0,	0,	1,
		100,	0,	10,	1,
		0,		0,	0,	1
	};
	Matrix gndDiffuseLocator_pnt(3, 4);
	memcpy(gndDiffuseLocator_pnt.data, gndDiffuseLocator, gndDiffuseLocator_pnt.size * sizeof(double));

	const double blkshadow[] = {
		0,		0,	-1,		1,
		0,		0,	1,		1,
		-10,	0,	1,		1,
		-10,	0,	-1,	1
	};
	Matrix blkshadow_model(4, 4);
	memcpy(blkshadow_model.data, blkshadow, blkshadow_model.size * sizeof(double));

	PointLight lights[] = {
		//logo light
		{{0,0,0}, {117, 129, 197}, 1.0},//c
		{{0,0,0}, {78, 140, 186}, 1.0},//cpp
		{{0,0,0}, {239, 214, 42}, 1.0},//js
		{{0,0,0}, {230, 123, 88}, 1.0},//html
		{{0,0,0}, {69, 153, 204}, 1.0},//css
		{{0,0,0}, {202, 61, 0}, 1.0},//rust
		{{0,0,0}, {38, 186, 223}, 1.0},//go
		{{0,0,0}, {189, 211, 220}, 1.0},//octave
		{{0,0,0}, {208, 218, 224}, 1.0},//opengl
		{{0,0,0}, {229, 210, 211}, 1.0},//vulkan
		{{0,0,0}, {58, 149, 96}, 1.0},//desmos
		{{0,0,0}, {116, 100, 28}, 1.0},//mc
		//scene light
		{{35, 0, 5}, {1, 1, 1}, 300},
		{{35, 0, -5}, {1, 1, 1}, 300},
		{{30, 30, 0}, {1, 1, 1}, 400},
		{{-40, 1, 0}, {1, 1, 1}, 60},
	};

	int frameCounter = 0;
	for(int lcounter = 0; lcounter < totalFrame; lcounter += 2) {//main render cycle
		double counter = (double)lcounter;
		Matrix normal;

		Matrix blkleft_raw = anim_squareRot_0((double)counter / 180 * M_PI);
		Matrix blkright_raw = (Matrix&)blkleft_raw;
		blkleft_raw[0][2] = blkleft_raw[1][2] = blkleft_raw[2][2] = blkleft_raw[3][2] = -1;
		blkright_raw[0][2] = blkright_raw[1][2] = blkright_raw[2][2] = blkright_raw[3][2] = 1;
		for(int i = 0; i < 4; i++) swap(blkright_raw[0][i], blkleft_raw[3][i]);
		for(int i = 0; i < 4; i++) swap(blkright_raw[1][i], blkleft_raw[2][i]);
		Matrix blkup_raw = (Matrix&)blkleft_raw;
		Matrix blkdown_raw = (Matrix&)blkright_raw;
		for(int i = 0; i < 4; i++) swap(blkright_raw[0][i], blkleft_raw[1][i]);
		for(int i = 0; i < 4; i++) swap(blkright_raw[3][i], blkleft_raw[2][i]);
		Matrix blkfront_raw = (Matrix&)blkleft_raw;
		Matrix blkback_raw = (Matrix&)blkright_raw;
		for(int i = 0; i < 4; i++) swap(blkright_raw[0][i], blkleft_raw[1][i]);
		for(int i = 0; i < 4; i++) swap(blkright_raw[3][i], blkleft_raw[2][i]);
		for(int i = 0; i < 4; i++) swap(blkright_raw[0][i], blkleft_raw[3][i]);
		for(int i = 0; i < 4; i++) swap(blkright_raw[1][i], blkleft_raw[2][i]);

		camRot = anim_cam_0((double)counter / 180 * M_PI);
		// camRot = anim_cam_0((double)90.0 / 180 * M_PI);
		double lowestCenterY = 10.0;
		double frontmostEdgeX = -10.0;
		for(int i = 0; i < 4; i++) frontmostEdgeX = fmax(frontmostEdgeX, blkleft_raw[i][0]);

		Matrix blklnorw = faceNormal(blkleft_raw, 0, 1, 2);
		Matrix blklcenter = center(blkleft_raw);
		lowestCenterY = fmin(lowestCenterY, blklcenter[0][1]);
		Matrix blkleft_trans = blkleft_raw * camRot;
		normal = faceNormal(blkleft_trans, 0, 1, 2);
		normalize3(normal);
		// writeColData(fp_blkl_col, (normal[0][0] + 1.0) * 0.5, (normal[0][1] + 1.0) * 0.5, (normal[0][2] + 1.0) * 0.5);
		blkleft_trans = blkleft_trans * perspective;
		Matrix blkLeft_final = applyPerspective(blkleft_trans, false);
		normal = faceNormal(blkLeft_final, 0, 3, 6);
		writeOpaData(fp_blkl_opa, normal[0][2] < 0 ? 1 : 0);
		writePosData(fp_blkleft, blkLeft_final, vpwidth, vpheight);
		
		Matrix blkrnorw = faceNormal(blkright_raw, 1, 0, 2);
		Matrix blkrcenter = center(blkright_raw);
		lowestCenterY = fmin(lowestCenterY, blkrcenter[0][1]);
		Matrix blkright_trans = blkright_raw * camRot;
		normal = faceNormal(blkright_trans, 1, 0, 2);
		normalize3(normal);
		// writeColData(fp_blkr_col, (normal[0][0] + 1.0) * 0.5, (normal[0][1] + 1.0) * 0.5, (normal[0][2] + 1.0) * 0.5);
		blkright_trans = blkright_trans * perspective;
		Matrix blkRight_final = applyPerspective(blkright_trans, false);
		normal = faceNormal(blkRight_final, 3, 0, 6);
		writeOpaData(fp_blkr_opa, normal[0][2] < 0 ? 1 : 0);
		writePosData(fp_blkright, blkRight_final, vpwidth, vpheight);

		Matrix blkunorw = faceNormal(blkup_raw, 1, 0, 2);
		Matrix blkucenter = center(blkup_raw);
		lowestCenterY = fmin(lowestCenterY, blkucenter[0][1]);
		Matrix blkup_trans = blkup_raw * camRot;
		normal = faceNormal(blkup_trans, 1, 0, 2);
		normalize3(normal);
		// writeColData(fp_blku_col, (normal[0][0] + 1.0) * 0.5, (normal[0][1] + 1.0) * 0.5, (normal[0][2] + 1.0) * 0.5);
		blkup_trans = blkup_trans * perspective;
		Matrix blkup_final = applyPerspective(blkup_trans, false);
		writePosData(fp_blkup, blkup_final, vpwidth, vpheight);
		normal = faceNormal(blkup_final, 3, 0, 6);
		writeOpaData(fp_blku_opa, normal[0][2] < 0 ? 1 : 0);
		
		Matrix blkdnorw = faceNormal(blkdown_raw, 0, 1, 2);
		Matrix blkdcenter = center(blkdown_raw);
		lowestCenterY = fmin(lowestCenterY, blkdcenter[0][1]);
		Matrix blkdown_trans = blkdown_raw * camRot;
		normal = faceNormal(blkdown_trans, 0, 1, 2);
		normalize3(normal);
		// writeColData(fp_blkd_col, (normal[0][0] + 1.0) * 0.5, (normal[0][1] + 1.0) * 0.5, (normal[0][2] + 1.0) * 0.5);
		blkdown_trans = blkdown_trans * perspective;
		Matrix blkdown_final = applyPerspective(blkdown_trans, false);
		writePosData(fp_blkdown, blkdown_final, vpwidth, vpheight);
		normal = faceNormal(blkdown_final, 0, 3, 6);
		writeOpaData(fp_blkd_opa, normal[0][2] < 0 ? 1 : 0);
		
		Matrix blkfnorw = faceNormal(blkfront_raw, 0, 1, 2);
		Matrix blkfcenter = center(blkfront_raw);
		lowestCenterY = fmin(lowestCenterY, blkfcenter[0][1]);
		Matrix blkfront_trans = blkfront_raw * camRot;
		normal = faceNormal(blkfront_trans, 0, 1, 2);
		normalize3(normal);
		// writeColData(fp_blkf_col, (normal[0][0] + 1.0) * 0.5, (normal[0][1] + 1.0) * 0.5, (normal[0][2] + 1.0) * 0.5);
		blkfront_trans = blkfront_trans * perspective;
		Matrix blkfront_final = applyPerspective(blkfront_trans, false);
		writePosData(fp_blkfront, blkfront_final, vpwidth, vpheight);
		normal = faceNormal(blkfront_final, 0, 3, 6);
		writeOpaData(fp_blkf_opa, normal[0][2] < 0 ? 1 : 0);
		
		Matrix blkbnorw = faceNormal(blkback_raw, 1, 0, 2);
		Matrix blkbcenter = center(blkback_raw);
		lowestCenterY = fmin(lowestCenterY, blkbcenter[0][1]);
		Matrix blkback_trans = blkback_raw * camRot;
		normal = faceNormal(blkback_trans, 1, 0, 2);
		normalize3(normal);
		// writeColData(fp_blkb_col, (normal[0][0] + 1.0) * 0.5, (normal[0][1] + 1.0) * 0.5, (normal[0][2] + 1.0) * 0.5);
		blkback_trans = blkback_trans * perspective;
		Matrix blkback_final = applyPerspective(blkback_trans, false);
		writePosData(fp_blkback, blkback_final, vpwidth, vpheight);
		normal = faceNormal(blkback_final, 3, 0, 6);
		writeOpaData(fp_blkb_opa, normal[0][2] < 0 ? 1 : 0);

		memcpy(axis_model.data, axis, axis_model.size * sizeof(double));
		axis_model = axis_model * camRot;
		axis_model = axis_model * perspective;
		Matrix axis_final = applyPerspective(axis_model, true);
		writePosData(fp_axis, axis_final, vpwidth, vpheight);

		memcpy(light_model.data, light, light_model.size * sizeof(double));
		light_model = light_model * camRot;
		light_model = light_model * perspective;
		Matrix light_final = applyPerspective(light_model, false);
		writePosData(fp_light, light_final, vpwidth, vpheight);

		memcpy(wall_model.data, wall, wall_model.size * sizeof(double));
		wall_model = wall_model * camRot;
		wall_model = wall_model * perspective;
		Matrix wall_final = applyPerspective(wall_model, false);
		writePosData(fp_wall, wall_final, vpwidth, vpheight);

		memcpy(gndDiffuseLocator_pnt.data, gndDiffuseLocator, gndDiffuseLocator_pnt.size * sizeof(double));
		gndDiffuseLocator_pnt[2][2] = 5.0 * sin(0.31 * sin((double)counter / 180 * M_PI));
		gndDiffuseLocator_pnt = gndDiffuseLocator_pnt * camRot;
		gndDiffuseLocator_pnt = gndDiffuseLocator_pnt * perspective;
		Matrix gndDiffuseLocator_final = applyPerspective(gndDiffuseLocator_pnt, true);
		double gndDiffuseRot = atan2(gndDiffuseLocator_final[3][1] - gndDiffuseLocator_final[0][1], aspect * (gndDiffuseLocator_final[0][0] - gndDiffuseLocator_final[3][0]));
		writeRotData(fp_gnddiff_rot, gndDiffuseRot * 180.0 / M_PI);
		writeTrsData(fp_gnddiff_pos, gndDiffuseLocator_final[0][0], gndDiffuseLocator_final[0][1], vpwidth, vpheight);
		double gndDiffuseScale = aspect
			* fabs((gndDiffuseLocator_final[6][0] - gndDiffuseLocator_final[3][0])
				* (gndDiffuseLocator_final[3][1] - gndDiffuseLocator_final[0][1])
				- (gndDiffuseLocator_final[3][0] - gndDiffuseLocator_final[0][0])
				* (gndDiffuseLocator_final[6][1] - gndDiffuseLocator_final[3][1]))
			/ sqrt(pow(aspect * (gndDiffuseLocator_final[6][0] - gndDiffuseLocator_final[3][0]), 2)
				+ pow(gndDiffuseLocator_final[6][1] - gndDiffuseLocator_final[3][1], 2));
		writeScaData(fp_gnddiff_sca, 1, gndDiffuseScale * 10);

		memcpy(blkshadow_model.data, blkshadow, blkshadow_model.size * sizeof(double));
		const double blkshadow_edge = anim_blkShadow_0((double)counter / 180 * M_PI);
		blkshadow_model[0][0] = blkshadow_model[1][0] = blkshadow_edge;
		blkshadow_model = blkshadow_model * camRot;
		blkshadow_model = blkshadow_model * perspective;
		Matrix blkshadow_final = applyPerspective(blkshadow_model, false);
		writePosData(fp_blkshadow, blkshadow_final, vpwidth, vpheight);

		memcpy(blkshadow_model.data, blkshadow, blkshadow_model.size * sizeof(double));
		double bottomShadowBaseOpacity = 1.0 - clamp(1.0 * M_SQRT2 * lowestCenterY, 0.0, 1.0);
		if(frontmostEdgeX - blkshadow_edge < M_SQRT2) bottomShadowBaseOpacity = 0.0;
		blkshadow_model[0][0] = blkshadow_model[1][0] = frontmostEdgeX;
		blkshadow_model[2][0] = blkshadow_model[3][0] = blkshadow_edge;
		blkshadow_model[0][2] += 0.6 * (1.0 - bottomShadowBaseOpacity);
		blkshadow_model[1][2] -= 0.6 * (1.0 - bottomShadowBaseOpacity);
		blkshadow_model = blkshadow_model * camRot;
		blkshadow_model = blkshadow_model * perspective;
		blkshadow_final = applyPerspective(blkshadow_model, false);
		writePosData(fp_blkbtmshadow, blkshadow_final, vpwidth, vpheight);
		writeOpaData(fp_blkbtmsh_opa, bottomShadowBaseOpacity );

		memcpy(blkshadow_model.data, blkshadow, blkshadow_model.size * sizeof(double));
		blkshadow_model[0][0] = blkshadow_model[1][0] = blkshadow_edge;
		blkshadow_model[2][0] = blkshadow_model[3][0] = -40;
		Matrix blkshadow_up_mirror_model = mirror3(blkshadow_model, blkunorw, blkup_raw);
		Matrix blkshadow_down_mirror_model = mirror3(blkshadow_model, blkdnorw, blkdown_raw);
		Matrix blkshadow_front_mirror_model = mirror3(blkshadow_model, blkfnorw, blkfront_raw);
		Matrix blkshadow_back_mirror_model = mirror3(blkshadow_model, blkbnorw, blkback_raw);

		blkshadow_up_mirror_model = blkshadow_up_mirror_model * camRot;
		blkshadow_up_mirror_model = blkshadow_up_mirror_model * perspective;
		Matrix blkshadow_up_mirror_final = applyPerspective(blkshadow_up_mirror_model, false);
		writePosData(fp_blkshumir, blkshadow_up_mirror_final, vpwidth, vpheight);

		blkshadow_down_mirror_model = blkshadow_down_mirror_model * camRot;
		blkshadow_down_mirror_model = blkshadow_down_mirror_model * perspective;
		Matrix blkshadow_down_mirror_final = applyPerspective(blkshadow_down_mirror_model, false);
		writePosData(fp_blkshdmir, blkshadow_down_mirror_final, vpwidth, vpheight);

		blkshadow_front_mirror_model = blkshadow_front_mirror_model * camRot;
		blkshadow_front_mirror_model = blkshadow_front_mirror_model * perspective;
		Matrix blkshadow_front_mirror_final = applyPerspective(blkshadow_front_mirror_model, false);
		writePosData(fp_blkshfmir, blkshadow_front_mirror_final, vpwidth, vpheight);

		blkshadow_back_mirror_model = blkshadow_back_mirror_model * camRot;
		blkshadow_back_mirror_model = blkshadow_back_mirror_model * perspective;
		Matrix blkshadow_back_mirror_final = applyPerspective(blkshadow_back_mirror_model, false);
		writePosData(fp_blkshbmir, blkshadow_back_mirror_final, vpwidth, vpheight);

		for(unsigned int p = 0; p < ALL_LOGO_COUNT; p++) {
			const double offset = -(double)((lcounter + (ALL_LOGO_COUNT - p) * 90) % totalFrame) / 180 * 4;
			Matrix offsetVec(1,4);
			offsetVec.clear();
			offsetVec[0][0] = offset;
			const double logoOpa = clamp(pow(fabs(offset + /*24.0*/ 24.0) * 0.05, 3), 0.0, 1.0) * clamp(-0.5 - offset, 0.0, 1.0);
			writeOpaData(all_logos[p]->opaFile[0], logoOpa);
			lights[p].intensity = logoOpa * 0.002;
			lights[p].pos[0] = offset;
			for(unsigned int i = 0; i < all_logos[p]->len; i++) {
				Matrix model = logoRawToMatrix(all_logos[p]->shapes[i], all_logos[p]->pointCount[i]);
				Matrix::inPlaceBroadcastRowAdd(model, offsetVec, model);

				Matrix logo_up_mirror_model = mirror3(model, blkunorw, blkup_raw);
				Matrix logo_down_mirror_model = mirror3(model, blkdnorw, blkdown_raw);
				Matrix logo_front_mirror_model = mirror3(model, blkfnorw, blkfront_raw);
				Matrix logo_back_mirror_model = mirror3(model, blkbnorw, blkback_raw);

				model = model * camRot * perspective;
				Matrix model_final = applyPerspective(model, false);
				writePosData(all_logos[p]->posFile[0][i], model_final, vpwidth, vpheight);

				logo_front_mirror_model = logo_front_mirror_model * camRot * perspective;
				Matrix logo_front_mirror_final = applyPerspective(logo_front_mirror_model, false);
				writePosData(all_logos[p]->posFile[1][i], logo_front_mirror_final, vpwidth, vpheight);

				logo_back_mirror_model = logo_back_mirror_model * camRot * perspective;
				Matrix logo_back_mirror_final = applyPerspective(logo_back_mirror_model, false);
				writePosData(all_logos[p]->posFile[2][i], logo_back_mirror_final, vpwidth, vpheight);

				logo_up_mirror_model = logo_up_mirror_model * camRot * perspective;
				Matrix logo_up_mirror_final = applyPerspective(logo_up_mirror_model, false);
				writePosData(all_logos[p]->posFile[3][i], logo_up_mirror_final, vpwidth, vpheight);

				logo_down_mirror_model = logo_down_mirror_model * camRot * perspective;
				Matrix logo_down_mirror_final = applyPerspective(logo_down_mirror_model, false);
				writePosData(all_logos[p]->posFile[4][i], logo_down_mirror_final, vpwidth, vpheight);
			}
		}

		Matrix blkllight = calcPointLighting(blklcenter, blklnorw, lights, LIGHT_COUNT(lights));
		writeColData(fp_blkl_col, blkllight[0][0], blkllight[0][1], blkllight[0][2]);
		Matrix blkrlight = calcPointLighting(blkrcenter, blkrnorw, lights, LIGHT_COUNT(lights));
		writeColData(fp_blkr_col, blkrlight[0][0], blkrlight[0][1], blkrlight[0][2]);
		Matrix blkulight = calcPointLighting(blkucenter, blkunorw, lights, LIGHT_COUNT(lights));
		writeColData(fp_blku_col, blkulight[0][0], blkulight[0][1], blkulight[0][2]);
		Matrix blkdlight = calcPointLighting(blkdcenter, blkdnorw, lights, LIGHT_COUNT(lights));
		writeColData(fp_blkd_col, blkdlight[0][0], blkdlight[0][1], blkdlight[0][2]);
		Matrix blkflight = calcPointLighting(blkfcenter, blkfnorw, lights, LIGHT_COUNT(lights));
		writeColData(fp_blkf_col, blkflight[0][0], blkflight[0][1], blkflight[0][2]);
		Matrix blkblight = calcPointLighting(blkbcenter, blkbnorw, lights, LIGHT_COUNT(lights));
		writeColData(fp_blkb_col, blkblight[0][0], blkblight[0][1], blkblight[0][2]);

		printf("render frame [%d]\n", frameCounter++);
	}

	writePosTail(fp_blkleft);
	writePosTail(fp_blkright);
	writePosTail(fp_blkup);
	writePosTail(fp_blkdown);
	writePosTail(fp_blkfront);
	writePosTail(fp_blkback);
	writePosTail(fp_axis);
	writePosTail(fp_light);
	writePosTail(fp_wall);
	writePosTail(fp_blkshadow);
	writePosTail(fp_blkbtmshadow);
	writePosTail(fp_blkshumir);
	writePosTail(fp_blkshdmir);
	writePosTail(fp_blkshfmir);
	writePosTail(fp_blkshbmir);
		
	fclose(fp_blkleft);
	fclose(fp_blkright);
	fclose(fp_blkup);
	fclose(fp_blkdown);
	fclose(fp_blkfront);
	fclose(fp_blkback);
	fclose(fp_axis);
	fclose(fp_light);
	fclose(fp_wall);
	fclose(fp_blkshadow);
	fclose(fp_blkbtmshadow);
	fclose(fp_blkshumir);
	fclose(fp_blkshdmir);
	fclose(fp_blkshfmir);
	fclose(fp_blkshbmir);

	writeOpaTail(fp_blkl_opa);
	writeOpaTail(fp_blkr_opa);
	writeOpaTail(fp_blku_opa);
	writeOpaTail(fp_blkd_opa);
	writeOpaTail(fp_blkf_opa);
	writeOpaTail(fp_blkb_opa);
	writeOpaTail(fp_blkbtmsh_opa);

	fclose(fp_blkl_opa);
	fclose(fp_blkr_opa);
	fclose(fp_blku_opa);
	fclose(fp_blkd_opa);
	fclose(fp_blkf_opa);
	fclose(fp_blkb_opa);
	fclose(fp_blkbtmsh_opa);

	writeRotTail(fp_gnddiff_rot);

	fclose(fp_gnddiff_rot);

	writeTrsTail(fp_gnddiff_pos);

	fclose(fp_gnddiff_pos);

	writeScaTail(fp_gnddiff_sca);

	fclose(fp_gnddiff_sca);

	writeColTail(fp_blkl_col);
	writeColTail(fp_blkr_col);
	writeColTail(fp_blku_col);
	writeColTail(fp_blkd_col);
	writeColTail(fp_blkf_col);
	writeColTail(fp_blkb_col);

	fclose(fp_blkl_col);
	fclose(fp_blkr_col);
	fclose(fp_blku_col);
	fclose(fp_blkd_col);
	fclose(fp_blkf_col);
	fclose(fp_blkb_col);

	for(int i = 0; i < 5; i++) {
		closeShapeGroupFile(logo_c, i);
		closeShapeGroupFile(logo_cpp, i);
		closeShapeGroupFile(logo_js, i);
		closeShapeGroupFile(logo_html, i);
		closeShapeGroupFile(logo_css, i);
		closeShapeGroupFile(logo_rust, i);
		closeShapeGroupFile(logo_go, i);
		closeShapeGroupFile(logo_octave, i);
		closeShapeGroupFile(logo_opengl, i);
		closeShapeGroupFile(logo_vulkan, i);
		closeShapeGroupFile(logo_desmos, i);
		closeShapeGroupFile(logo_mc, i);
	}

	build();
	puts("finish");
	return 0;
}