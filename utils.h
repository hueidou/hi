int curl(char *url);
char *strlink(char *str1, ...);
char *substr(const char *str, int start, int end);
char *match(char *text, const char *pattern, int *rm_eo, int add, int sub);
char *match_pcre(char *text, const char *pattern, int *rm_eo, int index, int add, int sub);