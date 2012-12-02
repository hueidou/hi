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
     *
	 *
	 * 		var qPagerInfo = {
			allCount: '142',
			pageSize: '100',
			curPage: '1'
		}; 
	 */

	int i, pageCount;
	char page_no[10];
	char *tag_page;

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

	return 0;
}
