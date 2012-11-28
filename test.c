#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <sys/stat.h>
#include "functions.h"

//char *host = "hi.baidu.com";
char *hi_name;

/*
 * 初始化
 */
void init()
{
    // 创建必要的目录结构
    mkdir(hi_name, 0775);
    char *item = strlink(hi_name, "/item", "__Last");
    printf("%d\n", mkdir(item, 0775));
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

void usage(int status)
{
    // EXIT_SUCCESS
    fputs("\
Usage: hi hi_name.\
        ", stdout);
}

int main(int argc, char *argv[])
{
    // 检查参数个数
    if (argc != 2)
    {
        usage(0);
        return 0;
    }

    // 空间名称
    hi_name = argv[1];   // hueidou163

    // 判断空间是否存在
    if (!hi_exist(hi_name))
    {
        fprintf(stderr, "%s\n", "空间不存在");
        return 0;
    }

    init();

    char host[1024];
    char path[1024];
    char file[1024];
    char *url = "http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4";
    cutUrl(url, host, path, file);

    /* code */
    // char * str1 = "asdfghjklzxcvbnmqwertyuiop";
    // char * str3 = "012";

    char *file2 = path;
    file2++;

    curl(host, path, file2);

    // 判断上一篇
    while((url = hasprev(file)) != NULL)
    {
        cutUrl(url, host, path, file);
        file2 = path;
        file2++;
        curl(host, path, file2);
    }

    //curl(host, path, file);
    //printf("%s", strlink(str1, str3, "\n", "__Last"));
    // char * str4;
    // str4 = malloc(0);
    // realloc(str4, 100);

    // printf("%s\n%d\n", str1, strlen(str1));

    // char * str2;
    // str2 = (char *)malloc((strlen(str1) + 1) * sizeof(char));
    // strcpy(str2, str1);

    // printf("%s\n%d\n", str2, strlen(str2));

    //test1(str);
    return 0;
}