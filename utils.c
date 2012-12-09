#include <stdio.h> 
#include <string.h> 
#include <curl/curl.h> 
#include <strings.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <regex.h>
#include <pcre.h>

// size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
// {
//	 int written = fwrite(ptr, size, nmemb, file);
//	 return written;
// }

int curl(char *url, char *path)
{
	FILE *file;
	CURL *curl;
	CURLcode ret;
	struct curl_slist *headers = NULL;

	if (path == NULL)
	{
		path = index(url, '/');
		path++;
	}

	/* First step, init curl */
	curl = curl_easy_init();
	if (!curl)
	{
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	
	/* HTTP HEADER */
	headers = curl_slist_append(headers, "Connection: close");
	headers = curl_slist_append(headers, "Accept-Charset: utf-8");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);

	/* 可以通过 CURLOPT_WRITEDATA属性给默认回调函数传递一个已经打开的文件指针，用于将数据输出到文件里。 */
	file = fopen(path, "w");
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)file);
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 

	/* Allow curl to perform the action */
	ret = curl_easy_perform(curl);

	long response_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	fclose(file);

	/* ret == 0 means everything is OK. */ 
	if (ret != 0 || response_code != 200)
	{
		return -1;
	}

	return 0;
}

char *strlink(char *str1, ...)
{
	char *str = malloc(sizeof(char));
	memset(str, 0, 1);
	// char *str = NULL;

	va_list arg_ptr; // 参数列表指针

	va_start(arg_ptr, str1); // 指向固定参数
	// va_copy(dest, src)

	char *strtemp = str1;

	char *p;
	while(abs(strcmp(strtemp, "__Last")))
	{
		p = realloc(str, strlen(str) + strlen(strtemp) + 1);
		if (p == NULL)
		{
			return NULL;
		}
		str = p;
		strcat(str, strtemp);

		strtemp = va_arg(arg_ptr, char *); // 返回char*参数
	}

	va_end(arg_ptr); // va_start配套

	// printf("%s\n", str);
	return str;
}

char *substr(const char *str, int start, int end)
{
	char *sub, *subtemp;
	int n;

	if ((n = end - start) < 0)
	{
		return NULL;
	}
	sub = malloc((n + 1) * sizeof(char));
	strncpy(sub, str + start, n);
	sub[n] = 0;
	return sub;
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

char *match_pcre(char *text, const char *pattern, int *rm_eo, int index, int add, int sub)
{
	char *str;
	pcre *pc;
	int pe;
	const char *errptr;
	int erroffset;
	int ovector[30];

	if (text == NULL || pattern == NULL) { return NULL; }
	/* PCRE_DOTALL . matches anything including NL */
	pc = pcre_compile(pattern, PCRE_DOTALL, &errptr, &erroffset, NULL);
	pe = pcre_exec(pc, NULL, text, strlen(text), 0, 0, ovector, 30);

	if (pe <= index)
	{
		return NULL;
	}

	*rm_eo = ovector[2 * index + 1];

	str = substr(text, ovector[2 * index] + add, ovector[2 * index + 1] - sub);
	return str;
}

int match_pcre2(char *text, const char *pattern, int *start, int *end)
{
	char *str;
	pcre *pc;
	int pe;
	const char *errptr;
	int erroffset;
	int ovector[30];

	if (text == NULL || pattern == NULL) { return 0; }
	/* PCRE_DOTALL . matches anything including NL */
	pc = pcre_compile(pattern, PCRE_DOTALL, &errptr, &erroffset, NULL);
	pe = pcre_exec(pc, NULL, text, strlen(text), 0, 0, ovector, 30);

	if (pe <= 1)
	{
		return 0;
	}

	*start = ovector[2];
	*end = ovector[3];

	return 1;
}