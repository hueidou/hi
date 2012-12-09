#include <stdio.h>
#include "utils.h"
#include "functions.h"

char *host = "hi.baidu.com";
char *hi_name;

/* some works of init
 * 创建必要的目录结构
 */
void hi_init()
{
	/* 锁定hi.baidu.com文件夹
	 * 限制于curl函数，改进curl
	 */
	mkdir(host, 0775);
	chdir(host);
	mkdir(hi_name, 0775);

	chdir(hi_name);
	mkdir("item", 0775);
	mkdir("_posts", 0775);
	chdir("..");

	mkdir("qcmt", 0775);
	mkdir("qcmt/data", 0775);
}

static void usage()
{
	printf("\n\tUsage: hi [hostname]\n");
	printf("\t   -a: nothing to do.\n");
	printf("\t   -b: nothing to do.\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		usage();
		return 0;
	}

	hi_name = argv[1];

	// 判断hi是否存在或者可读
	if (curl(strlink(host, "/", hi_name, "__Last")))
	{
		printf("%s/%s isn't exist or something wrong.\n", host, hi_name);
		return 0;
	}

	// 一些初始化
	hi_init();

	// 文章下载
	pagedown(host, hi_name);

	// 评论下载
	qcmtdown(hi_name);

	// 转换html为jekyll.md
	htmlconvert(hi_name);

	// 结束 
	printf("OK.\n");

	return 0;
}
