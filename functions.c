#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "functions.h"

#define LINE_CAPACITY	(1024 * 100)
#define ID_LENGTH	(24)

FILE *idfile;
static int count;	// 计数用

extern char *host;
extern char *hi_name;

/* 
 */
int hi_exist()
{
	if (curl(strlink(host, "/", hi_name, "__Last"), NULL))
	{
		return 1;
	}
	return 0;
}

/*
 * pagedown 
 */
void pagedown()
{
	// verbose
	printf("page downloading...\n");

	int i, pageCount;
	char page_no[10];
	char *tag_page;

	idfile = fopen(strlink(hi_name, ".id", "__Last"), "w+");

	count = 0;	// 开始计数
	for (i = 1; ; i++)
	{
		/* get */
		sprintf(page_no, "%d\0", i);
		tag_page = strlink(host, "/", hi_name, "/archive?type=tag&page=", page_no, "__Last");
		curl(tag_page, NULL);

		/* analy */
		tag_page = strlink(hi_name, "/archive?type=tag&page=", page_no, "__Last");
		pageCount = get_tag_page(tag_page);
		if (!pageCount)
		{
			break;
		}
	}

	fclose(idfile);
}

/* 
 * path: 本地文件路径 
 * 返回: 页面pageCount的值
 * <a href="http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4" class="blog-item blog-text" id="17268399" target="_blank" data-timestamp="1316612396"><div class="text-container">忽然间，想要去很远</div></a> 
 */
int get_tag_page(char *path)
{
	char *html;
	char *resource;

	resource = file2str(path);
	html = resource;
	if (html == NULL)
	{
		return 0;
	}

	const char *reg_item = "<a href=\"http://([^>]*?)\" class=\"blog-item blog-text\"";
	char *url;
	int offset;
	int pageCount = 0;
	char *id;

	// char *match_pcre(char *text, const char *pattern, int *rm_eo, int index, int add, int sub);
	while ((url = match_pcre(html, reg_item, &offset, 1, 0, 0)) != NULL)
	{
		/* 下载 item */
		curl(url, NULL);

		/* 将ids写入idfile */
		id = rindex(url, '/');
		id++;
		fprintf(idfile, "%s\n", id);

		// verbose
		printf("%d\t%s\n", ++count, id);

		pageCount++;
		html += offset;
	}

	free(resource);
	return pageCount;
}

/* 
 * 评论下载
 */
void qcmtdown()
{
	printf("comment downloading...\n");

	idfile = fopen(strlink(hi_name, ".id", "__Last"), "r");
	char id_arr[ID_LENGTH + 1] = {0};
	char *id = id_arr;
	char *qcmt = "hi.baidu.com/qcmt/data/cmtlist?thread_id_enc=";

	count = 0;
	while (id = fgets(id, ID_LENGTH + 1, idfile))
	{
		curl(strlink(qcmt, id, "__Last"), NULL);
		printf("%d\t%s\n", ++count, id);
		fgets(id, 2, idfile);	// 读取'\n'
	}

	fclose(idfile);
}

/*
 * 转换 
 */
void htmlconvert()
{
	printf("html converting...\n");

	idfile = fopen(strlink(hi_name, ".id", "__Last"), "r");
	char id_arr[ID_LENGTH + 1] = {0};
	char *id = id_arr;

	count = 0;	// VERBOSE
	while (id = fgets(id, ID_LENGTH + 1, idfile))
	{
		printf("%d\t%s\n", ++count, id);	// VERBOSE
		html2jekyll(strlink(hi_name, "/item/", id, "__Last"), strlink(hi_name, "/_posts", "__Last"), id);
		fgets(id, 2, idfile);	// 读取'\n'
	}

	fclose(idfile);
}

void html2jekyll(char *path, char *mdir, char *id)
{
	/*  */
	char *html;
	char *resource;

	resource = file2str(path);
	html = resource;
	if (html == NULL)
	{
		return;
	}

	char *title;			// title: 傻子才悲伤
	char *time;				// time: 2007-09-30 20:07
	char *content;			// 
	char *tags = NULL;				// tags: github,jekyll,markdown,disqus,simplegray
	char *pv;
	char *comment;
	char *share;

	int offset = 0;

	/* <div class=content-other-info>  <span>2010-06-09 19:24</span>  </div>
	 * <div class=content-other-info>.*?(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}).*?</div>
	 */
	char *reg_time = "<div class=content-other-info>  <span>(.*?)</span>  </div>";
	char *reg_title = "<h2 class=\"title content-title\">(.*?)</h2>";
	char *reg_content = "<div id=content class=\"content mod-cs-content text-content clearfix\">(.*?)</div>";
	char *reg_tags = "<div class=\"mod-tagbox clearfix\">(.*?)</div>";
	/*
	 * <span class=pv>浏览(76)</span>
	 * <span class=comment-nub>(1)</span>
	 * <span id=shareNub class=share-nub>(1)</span>
	 */
	char *reg_pv = "<span class=pv>浏览\\((\\d*)\\)</span>";
	char *reg_comment = "<span class=comment-nub>\\(?(\\d*)\\)?</span>";
	char *reg_share = "<span id=shareNub class=share-nub>\\(?(\\d*)\\)?</span>";

	time = match_pcre(html, reg_time, &offset, 1, 0, 0);
	html += offset;
	offset = 0;

	title = match_pcre(html, reg_title, &offset, 1, 0, 0);
	html += offset;
	offset = 0;

	content = match_pcre(html, reg_content, &offset, 1, 0, 0);
	html += offset;
	offset = 0;
	content = findimgs(content);

	tags = match_pcre(html, reg_tags, &offset, 1, 0, 0);
	html += offset;
	offset = 0;
	tags = findtags(tags);

	pv = match_pcre(html, reg_pv, &offset, 1, 0, 0);
	html += offset;
	offset = 0;

	comment = match_pcre(html, reg_comment, &offset, 1, 0, 0);
	html += offset;
	offset = 0;

	share = match_pcre(html, reg_share, &offset, 1, 0, 0);

	// html解析完毕时
	free(resource);

	FILE *post;
	char *date = substr(time, 0, 10);
	post = fopen(strlink(mdir, "/", date, "-", id, ".md", "__Last"), "w+");
	if (post == NULL) { return; }

	fprintf(post, "---\n");
	fprintf(post, "layout: post\n");
	if (title != NULL) {
		fprintf(post, "title: %s\n", title);
	} else {
		fprintf(post, "title: 无题\n");
	}
	if (tags != NULL) {
		fprintf(post, "tags: %s\n", tags);
	} else {
		fprintf(post, "tags: 无题,\n");
	}
	fprintf(post, "datetime: %s\n", time);
	fprintf(post, "pv: %s\n", pv);
	fprintf(post, "comment: %s\n", comment);
	fprintf(post, "share: %s\n", share);
	fprintf(post, "---\n");

	fprintf(post, "\n");
	fprintf(post, "{{ page.title }}\n");
	fprintf(post, "================\n");
	fprintf(post, "\n");

	fprintf(post, "%s\n", content);
	fprintf(post, "\n");

	fclose(post);
}

/*
  <a class="tag" href="/tag/%E5%BF%83%E6%83%85/feeds">#心情</a>    <a class="tag" href="/tag/%E7%BA%A2%E7%8B%B8/feeds">#红狸</a>    <a class="tag" href="/tag/%E6%96%87%E5%AD%97/feeds">#文字</a>    <a class="tag" href="/tag/%E6%83%B3%E5%BF%B5/feeds">#想念</a>  
 */
char *findtags(char *html)
{
	char *reg_tag = "<a [^>]*>#(.*?)</a>";
	char *tag;
	char *tags = (char *)calloc(sizeof(char), 100);

	int offset;

	while ((tag = match_pcre(html, reg_tag, &offset, 1, 0, 0)) != NULL)
	{
		strcat(tags, tag);
		strcat(tags, ",");
		html += offset;
	}

	return tags;
}

/*
 * 发现图片链接，替换并下载 
 * <img src="http://hiphotos.baidu.com/hueidou163/pic/item/e455fd64d7e8cecff63654fb.jpg" small="0" />
 * <img small="0" src="http://hiphotos.baidu.com/hueidou163/pic/item/2663e0fdec64632a09244d93.jpg" />
 * <img width="161" height="199" src="http://hiphotos.baidu.com/hueidou163/abpic/item/cee09abf8b1ad80718d81f05.jpg" small="0" style="width: 188px; height: 206px;" />
 * <img title="点击查看大图" src="http://baike.baidu.com/pic/2/11835527535632421_small.jpg" />
 * <img src="http://photo.southcn.com/upload/F43842003.11.20.10.5.5912.jpg" />
 * <img src="http://img3.pcpop.com/upimg3/2008/1/2/0004625952.jpg" alt="点此在新窗口浏览图片" />
 * <img src="http://imgsrc.baidu.com/baike/pic/item/58af236da2e79bcf431694f6.jpg" small="0" />
 * <img src="http://img.baidu.com/hi/jx2/j_0004.gif" />
 * <img height="132" src="http://tbn0.google.com/images?q=tbn:nl-BatuexLQBHM:http://foto.yculblog.com/jmadmen/2347.JPG" width="93" />
 * <img src=http://asdf.com/a.jpg />
 */
char *findimgs(char *html)
{
	char *content = html;
	char *reg_img = "<img .*?src=(\"?.*?\"?) .*?/>";
	char *src;
	int start, end;

	while (match_pcre2(html, reg_img, &start, &end) != 0)
	{
		down_img(html, start, end);
		html += end;
	}

	return content;
}

void down_img(char *html, int start, int end)
{
	char *url = substr(html, start, end);
	if (*url == '"')
	{
		url++;
		url[strlen(url) - 1] = 0;
	}
	printf("%s\n", url);

	char *filename = rindex(url, '/');
	char *src = strlink("/images", filename, "__Last");
	char *filepath = strlink(hi_name, "/images", filename, "__Last");

	curl(url, filepath);	// 下载图片

	// 更改src
	src = strlink("\"", src, "\"", "__Last");
	strncpy2(html + start, src, end - start);
}

/* 以空格填充dest剩余区域 */
char *strncpy2(char *dest, const char *src, size_t n)
{
   size_t i;

   for (i = 0; i < n && src[i] != '\0'; i++)
       dest[i] = src[i];
   for ( ; i < n; i++)
       dest[i] = ' ';

   return dest;
}

/*
 * 用后务必释放 
 */
char *file2str(char *path)
{
	FILE *fp;
	int fileLen;
	char *resource;

	fp = fopen(path, "r");
	if (fp == NULL)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	fileLen = ftell(fp);
	resource = (char *)malloc(sizeof(char) * fileLen);
	rewind(fp);
	fread(resource, fileLen, sizeof(char), fp);
	fclose(fp);

	return resource;
}