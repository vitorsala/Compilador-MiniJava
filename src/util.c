/*
 * util.c
 *
 *  Created on: 28 de set de 2016
 *      Author: vitorsala@gmail.com
 */
#include <stdlib.h>

int strCmp(const char *str1, int str1len, const char *str2){
	int i;
	for(i = 0; i < str1len || str2[i] != '\0'; i++){
		if(str1[i] != str2[i])	return 0;
	}
	if(i < str1len && str2[i] != '\0')	return 0;
	return 1;
}

char* strtrim(char* s, int len){
	int i = 0;
	char *out = (char*) calloc (len, sizeof(char));
	for(;i < len; i++){
		out[i] = s[i];
	}
	return out;
}
