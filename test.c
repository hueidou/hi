#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern int curl(char *url);
extern char *match(char *text, const char *pattern, int *rm_eo, int add, int sub);

int main(int argc, char *argv[])
{
	char *s;
	char *str = "aaaaa";
	int offset;
	while ((s = match(str, "a", &offset, 0, 0)) != NULL)
	{
		printf("%s\n", s);
		str += offset;
	}

	return 0;
}