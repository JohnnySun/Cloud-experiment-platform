#include <stdio.h>

int main(int argc, char **argv)
{
	int i = 0;
	for(i = 0; i < 5; i++) {
		printf("%s\n", argv[i]);
	}
	return 0;
}
