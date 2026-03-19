#pragma once
#include <stdio.h>
#include "matrix.hpp"
#include "perspective.hpp"

#define ANIM_TIME "15s"

const char posHead[] = "<animate attributeName=\"points\" values=\"";
const char posTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";
const char opaHead[] = "<animate attributeName=\"opacity\" values=\"";
const char opaTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"discrete\"/>";
const char x2dHead[] = "<animate attributeName=\"x\" values=\"";
const char x2dTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";
const char y2dHead[] = "<animate attributeName=\"y\" values=\"";
const char y2dTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";
const char rotHead[] = "<animateTransform attributeName=\"transform\" type=\"rotate\" values=\"";
const char rotTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";	
const char trsHead[] = "<animateTransform attributeName=\"transform\" type=\"translate\" values=\"";
const char trsTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";
const char scaHead[] = "<animateTransform attributeName=\"transform\" type=\"scale\" values=\"";
const char scaTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";
const char colHead[] = "<animate attributeName=\"fill\" values=\"";
const char colTail[] = "\" dur=\"" ANIM_TIME "\" repeatCount=\"indefinite\" calcMode=\"linear\"/>";

void writePosData(FILE* fp, Matrix output, double vpwidth, double vpheight) {
	for(unsigned int i = 0; i < output.height; i++) {
		fprintf(fp, "%.0lf,%.0lf ", -output[i][0] * vpwidth / 2, -output[i][1] * vpheight / 2);
	}
	fseek(fp, -1, SEEK_CUR);
	fputc(';', fp);
}

void writePosHead(FILE* fp) {
	fprintf(fp, posHead);
}

void writePosTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, posTail);
}

void writeOpaData(FILE* fp, double output) {
	fprintf(fp, "%.2lf;", output);
}

void writeOpaHead(FILE* fp) {
	fprintf(fp, opaHead);
}

void writeOpaTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, opaTail);
}

void writeX2dData(FILE* fp, double output) {
	fprintf(fp, "%.2lf;", output);
}

void writeX2dHead(FILE* fp) {
	fprintf(fp, x2dHead);
}

void writeX2dTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, x2dTail);
}

void writeY2dData(FILE* fp, double output) {
	fprintf(fp, "%.2lf;", output);
}

void writeY2dHead(FILE* fp) {
	fprintf(fp, y2dHead);
}

void writeY2dTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, y2dTail);
}

void writeRotData(FILE* fp, double angle) {
	fprintf(fp, "%.1lf;", -angle);
}

void writeRotHead(FILE* fp) {
	fprintf(fp, rotHead);
}

void writeRotTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, rotTail);
}

void writeTrsData(FILE* fp, double x, double y, double vpwidth, double vpheight) {
	fprintf(fp, "%.0lf,%.0lf;", -x * vpwidth / 2, -y * vpheight / 2);
}

void writeTrsHead(FILE* fp) {
	fprintf(fp, trsHead);
}

void writeTrsTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, trsTail);
}

void writeScaData(FILE* fp, double x, double y) {
	fprintf(fp, "%.2lf %.2lf;", x, y);
}

void writeScaHead(FILE* fp) {
	fprintf(fp, scaHead);
}

void writeScaTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, scaTail);
}

void writeColData(FILE* fp, double r, double g, double b) {
	static const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char buf[9];
	buf[0] = '#';
	buf[7] = ';';
	buf[8] = 0;
	unsigned char r_hex = (unsigned char)(255.0 * clamp(r, 0.0, 1.0));
	unsigned char g_hex = (unsigned char)(255.0 * clamp(g, 0.0, 1.0));
	unsigned char b_hex = (unsigned char)(255.0 * clamp(b, 0.0, 1.0));
	buf[1] = hex[r_hex >> 4];
	buf[2] = hex[r_hex & 0x0F];
	buf[3] = hex[g_hex >> 4];
	buf[4] = hex[g_hex & 0x0F];
	buf[5] = hex[b_hex >> 4];
	buf[6] = hex[b_hex & 0x0F];
	fprintf(fp, buf);
}

void writeColHead(FILE* fp) {
	fprintf(fp, colHead);
}

void writeColTail(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, colTail);
}