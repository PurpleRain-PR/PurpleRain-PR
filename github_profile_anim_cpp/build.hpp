#pragma once
#include <stdio.h>
#include <string.h>

FILE* newPolyFile(const char *str) {
	char fnameBuf[256] = "./build/";
	strcat(fnameBuf, str);
	strcat(fnameBuf, ".poly");
	printf("create [%s]\n", str);
	return fopen(fnameBuf, "w");
}

void build() {
	FILE *in = fopen("template.svg", "r");
	FILE *out = fopen("build.svg", "w");
	char fnameBuf[256] = "./build/";
	char* fname = sizeof("./build/") - 1 + fnameBuf;
	while(1) {
		char c = fgetc(in);
		if(c == EOF) break;
		if(c == '$') {
			fscanf(in, "[%[^]]", fname); fgetc(in);
			printf("build [%s]\n", fname);
			strcat(fnameBuf, ".poly");
			FILE *src = fopen(fnameBuf, "r");
			if(src == NULL) {
				printf("open [%s] failed\n", fnameBuf);
				continue;
			}
			while(1) {
				char ch = fgetc(src);
				if(ch == EOF) break;
				fputc(ch, out);
			}
			fclose(src);
			continue;
		}
		fputc(c, out);
	}
	fclose(in);
	fclose(out);
}

#include "write.hpp"