#include <stdio.h>
#include <string.h>

int main(){

	char *str, buf[23]="end";
	str=malloc(256);
	gets(str);
	printf("\n%s\n", str);

}
