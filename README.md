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

详细参数：
http://hi.baidu.com/qcmt/data/cmtlist
?qing_request_source=new_request
&thread_id_enc=6e688f6ce9e8300ba1cf0fa4
&start=0
&count=20
&orderby_type=1
&favor=2
&type=smblog
&b1354505795610=1

一个参数：
http://hi.baidu.com/qcmt/data/cmtlist?thread_id_enc=974c6bc2864126b90d0a7bbe

其他
---
确保文章所有人可见，仅自己可见的文章不可导出，待程序改善或者修改权限。
文章内有代码框的会出现错误。<div>