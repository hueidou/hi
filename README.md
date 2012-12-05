百度博客迁移
==========

0. 从 /archive?type=tag&page=1 获取文章。
0. 从 /qcmt/data/cmtlist 接口获取评论信息。
0. 获取文章内的图片到本地。
0. 从上述提取信息，转为Markdown。(暂时使用html2text项目转换)
0. 完善内容，导入Jekyll博客。

文章
---
/archive?type=tag&page=1
/item/2713d25bccbb60cdd3e10ca4

评论信息
-------
\http://hi.baidu.com/qcmt/data/cmtlist?thread_id_enc=974c6bc2864126b90d0a7bbe

其他
---
* 仅自己可见的文章不可导出，待程序改善或者修改权限。
* 文章内有代码框的会出现错误。待程序改善或者修改权限。
* [Html2text项目(Python)](http://github.com/aaronsw/html2text/)
* [Html2md(C)](https://github.com/hueidou/html2md/)
* [Markdown Online](http://joncom.be/experiments/markdown-editor/edit/)
* [Markdown-Syntax-CN](https://gitcafe.com/riku/Markdown-Syntax-CN/blob/master/syntax.md)