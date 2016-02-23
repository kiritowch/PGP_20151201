#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"

/* buffer长度足够就行 */
#define BUF_LEN        (20240)

unsigned long len;
unsigned long slen;

char* compression(const char* s)
{
	char buf[BUF_LEN];
	len = BUF_LEN;
	slen = strlen(s);
	compress((Bytef*)buf, &len, (Bytef*)s, slen);
	return buf;
	//return (char*)s;
}

char* uncompression(const char* buf)
{
	char* d = (char*)malloc(sizeof(char) * (slen + 1));
	len = strlen(buf);
	uncompress((Bytef*)d, &slen, (Bytef*)buf, len);
	d[slen] = '\0';
	return d;
	//return (char*)buf;
}

/*
int main(int argc, char* argv[])
{
	char* p = "adfga sdgdsdgg sdgki,oweutog;reyweu jfgdjghsdhdljjdgsdlglfjgljgp sudopuo pgjuas";
	char* d;

	printf("%s\n", p);
	compression(p);
	d = uncompression();
	printf("%s\n", buf);
	printf("%s\n", d);

	free(d);
	system("pause");
	return 0;
}*/