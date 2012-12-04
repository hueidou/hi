#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CAPACITY	(1024 * 1024)


FILE *idfile;

static char *substr(const char *str, int start, int end)
{
	char *sub, *subtemp;
	int n;

	if ((n = end - start) <= 0)
	{
		return NULL;
	}
	sub = malloc((n + 1) * sizeof(char));
	strncpy(sub, str + start, n);
	sub[n] = 0;
	return sub;
}
// regex_t *preg;	// a pointer to a pattern buffer storage area;

/* 
 * path: 本地文件路径 
 */
int get_tag_page(char *path)
{
	/* 
	 * <a href="http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4" class="blog-item blog-text" id="17268399" target="_blank" data-timestamp="1316612396"><div class="text-container">忽然间，想要去很远</div></a> 
	 * /<a href="http:\/\/hi.baidu.com\/[^>]*blog-item blog-text/
	 */
	FILE *fp;
	regex_t preg;
	char *line = malloc(LINE_CAPACITY);
	regmatch_t pmatch;
	int pageCount = 0;
	char *url;
	char *id;

	const char *pattern = "<a href=\"http://hi.baidu.com/[^>]*blog-item blog-text";

	fp = fopen(path, "r");
	if (fp == NULL) { return; }

	/* compile regex */
	if (regcomp(&preg, pattern, 0)) { return; }

	while (fgets(line, LINE_CAPACITY, fp) != NULL)
	{
		while (1)
		{
			if (regexec(&preg, line, 1, &pmatch, 0))
			{
				break;
			}

			pageCount++;
			// printf("%s\n", substr(line, pmatch.rm_so + 16, pmatch.rm_eo - 28));
			url = substr(line, pmatch.rm_so + 16, pmatch.rm_eo - 28);
			curl(url);

			id = rindex(url, '/');
			id++;
			fprintf(idfile, "%s\n", id);
			fflush(idfile);

			line += pmatch.rm_eo;
		}
	}

	fclose(fp);
	regfree(&preg);

	return pageCount;
}

/* 
 * 匹配字符串
 *
 * test: 需要匹配的字符串
 * pattern: 模式
 * rm_eo: 匹配尾偏移量
 * add: 左侧忽略的字符数
 * sub: 右侧忽略的字符数
 */
char *match(char *text, const char *pattern, int *rm_eo, int add, int sub)
{
	char *str;
	regex_t preg;
	regmatch_t pmatch;

	if (text == NULL || pattern == NULL) { return NULL; }
	if (regcomp(&preg, pattern, 0)) { return NULL; }
	if (regexec(&preg, text, 1, &pmatch, 0)) { return NULL;	}

	*rm_eo = pmatch.rm_eo;
	
	str = substr(text, pmatch.rm_so + add, pmatch.rm_eo - sub);
	return str;
}
