#include <stdio.h> 
#include <string.h> 
#include <curl/curl.h> 
#include <strings.h>
#include <stdlib.h>
#include <stdarg.h>

// size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
// {
//	 int written = fwrite(ptr, size, nmemb, file);
//	 return written;
// }

int curl(char *url)
{
	FILE *file;
	CURL *curl;
	CURLcode ret;
	char *path;
	struct curl_slist *headers = NULL;

	path = index(url, '/');
	path++;

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
