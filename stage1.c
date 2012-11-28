/*
 * 获取html
 */

#include "utils.h"

// http://hi.baidu.com/hueidou163/archive
// 获取年份
// http://hi.baidu.com/hueidou163/archive?year=2007
// 获取文件名称/链接
// http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4
// 保存

char* htmlPath = "data/html/";

void getHtml()
{
    // path: ./data/html/

}

void getArchive()
{
    // http://hi.baidu.com/hueidou163/archive
    curl("http://hi.baidu.com/hueidou163/archive", htmlPath);

    // META数据
    
}

void getLinks()
{
    // 列表页面获取链接
    // archive?year=2007
    //// archive?year=2011&month=1
    //// archive?type=tag
    //// archive?type=tag&tag=纸飞机

}