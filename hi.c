#include <stdio.h>
#include "utils_rewite.h"

#define ID_LENGTH	24

char *host = "hi.baidu.com";
char *hi_name;
FILE *idfile;

/* some works of init
 * 创建必要的目录结构
 */
void hi_init()
{
	mkdir(host, 0775);
	chdir(host);
    mkdir(hi_name, 0775);
    char *item = strlink(hi_name, "/item", "__Last");
    mkdir(item, 0775);

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

    /* 判断hi是否存在或者可读 */
    if (curl(strlink(host, "/", hi_name, "__Last")))
    {
    	printf("%s/%s isn't exist or something wrong.\n", host, hi_name);
    	printf("\n");
    	return -1;
    }

    hi_init();

	/* 
	 * 文章下载
	 */
	 printf("page downloading...\n");

	int i, pageCount;
	char page_no[10];
	char *tag_page;

	idfile = fopen(strlink(hi_name, ".id", "__Last"), "w+");

	for (i = 1; ; i++)
	{
		/* get */
		sprintf(page_no, "%d\0", i);
		tag_page = strlink(host, "/", hi_name, "/archive?type=tag&page=", page_no, "__Last");
		curl(tag_page);

		/* analy */
		tag_page = strlink(hi_name, "/archive?type=tag&page=", page_no, "__Last");
		pageCount = get_tag_page(tag_page);
		if (!pageCount)
		{
			break;
		}
	}

	fclose(idfile);

	/* 
	 * 评论下载
	 * 遍历item, 或者读取id文件
	 */
	printf("comment downloading...\n");

	idfile = fopen(strlink(hi_name, ".id", "__Last"), "r");
	char id_arr[ID_LENGTH + 1] = {0};
	char *id = id_arr;
	char *qcmt = "hi.baidu.com/qcmt/data/cmtlist?thread_id_enc=";

	while (id = fgets(id, ID_LENGTH + 1, idfile))
	{
		curl(strlink(qcmt, id, "__Last"));
		fgets(id, 2, idfile);	// 读取'\n'
	}

	fclose(idfile);


	/*
	 * 结束 
	 */
	printf("OK.\n");

	return 0;
}
