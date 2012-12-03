#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int curl(char *url);

int main(int argc, char *argv[])
{
    printf("%d\n", curl("hi.baidu.com/avapxia"));
    printf("%d\n", curl("hi.baidu.com/hueidou163"));
    printf("%d\n", curl("hi.baidu.com/hueidou163a"));
}