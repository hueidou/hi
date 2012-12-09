#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "functions.h"

#define LINE_CAPACITY	(1024 * 1024)
#define ID_LENGTH	(24)

FILE *idfile;

/* 
 * path: 本地文件路径 
 */
int get_tag_page(char *path)
{
	/* 
	 * <a href="http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4" class="blog-item blog-text" id="17268399" target="_blank" data-timestamp="1316612396"><div class="text-container">忽然间，想要去很远</div></a> 
	 * /<a href="http:\/\/hi.baidu.com\/[^>]*blog-item blog-text/
	 */
	FILE *fp;
	regex_t preg;
	char *line = malloc(LINE_CAPACITY);
	regmatch_t pmatch;
	int pageCount = 0;
	char *url;
	char *id;

	const char *pattern = "<a href=\"http://hi.baidu.com/[^>]*blog-item blog-text";

	fp = fopen(path, "r");
	if (fp == NULL) { return; }

	/* compile regex */
	if (regcomp(&preg, pattern, 0)) { return; }

	while (fgets(line, LINE_CAPACITY, fp) != NULL)
	{
		while (1)
		{
			if (regexec(&preg, line, 1, &pmatch, 0))
			{
				break;
			}

			pageCount++;
			// printf("%s\n", substr(line, pmatch.rm_so + 16, pmatch.rm_eo - 28));
			url = substr(line, pmatch.rm_so + 16, pmatch.rm_eo - 28);
			curl(url);

			id = rindex(url, '/');
			id++;
			fprintf(idfile, "%s\n", id);
			fflush(idfile);

			line += pmatch.rm_eo;
		}
	}

	fclose(fp);
	regfree(&preg);

	return pageCount;
}

void pagedown(char *host, char *hi_name)
{
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
}

void qcmtdown(char *hi_name)
{
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
}

void htmlconvert(char *hi_name)
{
	FILE *idfile;

	printf("html converting...\n");

	idfile = fopen(strlink(hi_name, ".id", "__Last"), "r");
	char id_arr[ID_LENGTH + 1] = {0};
	char *id = id_arr;
	int count = 1;

	while (id = fgets(id, ID_LENGTH + 1, idfile))
	{
		// hueidou163/item/...
		printf("%d\t", count++);
		html2jekyll(strlink(hi_name, "/item/", id, "__Last"), strlink(hi_name, "/_posts", "__Last"), id);
		fgets(id, 2, idfile);	// 读取'\n'
	}

	fclose(idfile);
}

void html2jekyll(char *htmlpath, char *mdir, char *id)
{
	printf("%s ", id);

	int fileLen;
	int offset = 0;

	FILE *fp = fopen(htmlpath, "r");
	if (fp == NULL)
	{
		return;
	}

	fseek(fp, 0, SEEK_END);
	fileLen = ftell(fp);
	char *res = (char *)malloc(sizeof(char) * fileLen);
	char *html = res;
	// fseek(fp, 0, SEEK_SET);
	rewind(fp);
	fread(html, fileLen, sizeof(char), fp);
	fclose(fp);

	char *title;			// title: 傻子才悲伤
	char *time;				// time: 2007-09-30 20:07
	char *content;			// 
	char *tags = NULL;				// tags: github,jekyll,markdown,disqus,simplegray
	char *pv;
	char *comment;
	char *share;

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

	// printf("%s\n", time);
	// printf("%s\n", title);
	// printf("%s\n", content);
	// printf("%s\n", tags);
	// printf("%s\n", pv);
	// printf("%s\n", comment);
	// printf("%s\n", share);
	free(res);

	FILE *post;
	char *date = substr(time, 0, 10);
	post = fopen(strlink(mdir, "/", date, "-", id, ".md", "__Last"), "w+");
	if (post == NULL) { return; }

	fprintf(post, "---\n");
	fprintf(post, "layout: post\n");
	if (title != NULL) {
		fprintf(post, "title: %s\n", title);
	} else {
		fprintf(post, "title: 无题");
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
	printf("*\n");
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