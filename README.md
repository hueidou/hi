百度博客迁移
==========

1. 从 http://hi.baidu.com/{hi_name}/item/{} 获取原始HTML页。
2. 由HTML转换为中间格式，中间格式参考《UNIX编程艺术》5.2中的数据文件元格式DSV, RFC 822, Cookie-Jar, Record-Jar, XML, Windows INI。
3. 参考WordPress eXtended Rss(WXR)，由中间格式转换为xml。
4. 转换为Markdown。


文章内容
-------
/item/2713d25bccbb60cdd3e10ca4  

评论信息
-------
cmtlist /qcmt  
http://hi.bdimg.com/static/qcomment/js/qcmt_import.js?v=6ba41c3a.js

其他
---
确保文章所有人可见，仅自己可见的文章不可导出，待程序改善或者修改权限。