#include <stdio.h>
#include "utils_rewite.h"

char *host = "hi.baidu.com";
char *hi_name;

void hi_init()
{
    // 创建必要的目录结构
    mkdir(hi_name, 0775);
    char *item = strlink(hi_name, "/item", "__Last");
    mkdir(item, 0775);
}

int hi_exist(char *page)
{
    /*
        GET /asdfasdfasdfasdfadsf HTTP/1.1
        Host: hi.baidu.com

        HTTP/1.1 302 Found
        Set-Cookie: BAIDUID=02B05E0906C41DE9E936F3D071987140:FG=1; max-age=31536000; expires=Thu, 28-Nov-13 06:23:45 GMT; domain=.baidu.com; path=/; version=1
        P3P: CP=" OTI DSP COR IVA OUR IND COM "
        Location: /com/error/?from=blog
        Content-type: text/html
        Content-Length: 0
        Connection: close
        Date: Wed, 28 Nov 2012 06:23:45 GMT
        Server: apache
    */
    return 1;
}

static void usage()
{
	printf("\n\tUsage: hi [hostname]\n");
	printf("\t   -a: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
	printf("\t   -b: bbbbbbbbbbbbbbbbbbb\n");
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

    if (!hi_exist(hi_name))
    {
    	printf("\n%s/%s isn't exist.\n", host, hi_name);
    	return -1;
    }

    /* some works of init */
    hi_init();

    /* http://hi.baidu.com/hueidou163/archive?type=tag
     * /archive?type=tag
     * /archive?type=tag&page=2
     */
    curl(strlink(host, "/", hi_name, "/archive?type=tag&page=1", "__Last"));
	//curl("hi.baidu.com/hueidou163/archive?type=tag&page=1");
	/*
	 * 		var qPagerInfo = {
			allCount: '142',
			pageSize: '100',
			curPage: '1'
		}; 
	 */
	int allCount, pageSize;
	/* some func */
	allCount = 142;
	pageSize = 100;

	if (allCount == 0)
	{
		printf("not exist item.\n");
		return 0;
	}

	int i;
	char tag_page_num[10] = "1";
	for (i = 2; ; ++i)
	{
		// 分析链接
		 get_tag_page(strlink(hi_name, "/archive?type=tag&page=", tag_page_num, "__Last"));
		//get_tag_page("hueidou163/archive?type=tag&page=1");

		if (i <= ((allCount - 1) / pageSize + 1))
		{
			// 获取下一个
			sprintf(tag_page_num, "%d\0", i);
			curl(strlink(host, "/", hi_name, "/archive?type=tag&page=", tag_page_num, "__Last"));
			//curl("hi.baidu.com/hueidou163/archive?type=tag&page=2");
	
		}
		else
		{
			return 0;
		}
	}

	return 0;
}
