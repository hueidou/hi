#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <sys/socket.h>
#include "utils.h"
#include <netdb.h>  // gethostbyname
#include <errno.h>
#include <unistd.h> // getcwd
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFLEN  128

// static struct sockaddr_in sockaddr_in1;
static int port = 80;

FILE *cin, *cout;

/*
 * 
#define FILEPATH_MAX (80)
int main(){
	char *file_path_getcwd;
	file_path_getcwd=(char *)malloc(FILEPATH_MAX);
	getcwd(file_path_getcwd,FILEPATH_MAX);
	printf("%s",file_path_getcwd);
}
 */

// ssize_t readlink(const char *path, char *buf, size_t bufsiz);

// readlink("/proc/self/exe",);

	   

/*
 * char *getcwd(char *buf, size_t size);
 */
void cutUrl(char *url, char *host, char *path, char *file)
{
	// http://hi.baidu.com/hueidou163/item/2713d25bccbb60cdd3e10ca4
	// hi.baidu.com <--> host
	// /hueidou163/item/2713d25bccbb60cdd3e10ca4 <--> path
	// 2713d25bccbb60cdd3e10ca4 <--> file
	char *str = url;
	/*
	   char *index(const char *s, int c);
	   char *rindex(const char *s, int c);
	*/
	char *str1 = index(str, '/');
	str1 += 2;
	char *host1 = host;
	while (*str1 != '/')
	{
		*host1 = *str1;
		host1++;
		str1++;
	}
	*host1 = '\0';

	char * str2 = index(url, '/');
	str2 += 2;
	str2 = index(str2, '/');
	strcpy(path, str2);

	char * str3 = rindex(url, '/');
	str3++;
	strcpy(file, str3);
}


/*
 * 获取html内容到文件
 * http://hi.baidu.com/hueidou163/archive
 */
void curl(char * host, char *path, char * file)
{

	// struct hostent *hp = 0; // NULL == 0
	int s;  // socket fd
	int err;
	int c;
	struct addrinfo	 hint;
	struct addrinfo	 *ailist, *aip;

	// s = socket(AF_INET, SOCK_STREAM, 0);
	// if (s < 0) {
	//	 fprintf(stderr, "create socket error\n");
	//	 return;
	// }
	// struct sockaddr_in a;
	// struct sockaddr_in *sin = &a;
	// // printf("%d,%d\n", sizeof(sin), sizeof(*sin));
	// sin->sin_family = AF_INET;
	// sin->sin_port = 80;
	// inet_pton(AF_INET, "127.0.0.1", &sin->sin_addr);
	// printf("%d\n", sin->sin_family);
	// printf("%d\n", sin->sin_port);

	// char str[INET_ADDRSTRLEN];
	// inet_ntop(AF_INET, &sin->sin_addr, str, INET_ADDRSTRLEN);
	// printf("%s\n", str);

	// c = connect(s, (struct sockaddr *)sin, sizeof(*sin));
	// if (c < 0) {
	//	 fprintf(stderr, "connect error: %d\n", errno);
	//	 fprintf(stderr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
	//		 EACCES, EPERM, EADDRINUSE, EAFNOSUPPORT, EAGAIN, EALREADY, EBADF,
	//		 ECONNREFUSED, EFAULT, EINPROGRESS, EINTR, EISCONN, ENETUNREACH, 
	//		 ENOTSOCK, ETIMEDOUT);
	//	 return;
	// }



	// hi.baidu.com
	// /hueidou163/archive
	// 基于域名的的虚拟主机是根据客户端提交的HTTP头中的关于主机名的部分决定的。 使用这种技术，很多虚拟主机可以享用同一个IP地址。

	char * request = strlink(
		"GET ", path, " HTTP/1.1\r\n",
		"Host: ", host, "\r\n",
		"Connection: close\r\n",
		"Accept-Charset: utf-8\r\n",
		"\r\n\r\n",
		"__Last");
	fprintf(stderr, "%s\n", request);
		// goto a;

	// int socket(int domain, int type, int protocol);
	// int shutdown(int ockfd, int how);

	// IPv4 套接字地址结构配置
	// memset(&sockaddr_in1, 0, sizeof(sockaddr_in1));
	// sockaddr_in1.sin_family = AF_INET;
	// sockaddr_in1.sin_port = port;

	// 获取主机信息
	// hp = gethostbyname(host);   //===============================
	// if (hp == NULL) {
	//	 fprintf(stderr, "resolve error\n");
	//	 return;
	// }
	// printf("*h_name: %s\n", hp->h_name);
	// if (hp->h_addr_list[0])
	// {
	//	 printf("**h_addr_list: %s\n", inet_ntop(hp->h_addr_list[0]));
	// }

	// 配置过滤器
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_INET;   // ipv4 internet domain
	hint.ai_socktype = SOCK_STREAM; // TCP
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	fprintf(stderr, "%s\n", "funcitons begin");

	// 获取/检查struct addrinfo链表
	if ((err = getaddrinfo(host, "http", &hint, &ailist)) != 0)
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));

	fprintf(stderr, "%s\n", "getaddrinfo");

	if (ailist == NULL)
	{
		fprintf(stderr, "%s\n", "ailist is empty");
		return;
	}

	// for (aip = ailist; aip != NULL; aip = aip->ai_next) {
	//	 print_flags(aip);
	//	 print_family(aip);
	//	 print_type(aip);
	//	 print_protocol(aip);
	//	 printf("\n\thost %s", aip->ai_canonname?aip->ai_canonname:"-");
	//	 if (aip->ai_family == AF_INET) {
	//		 sinp = (struct sockaddr_in *)aip->ai_addr;
	//		 addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
	//		 printf(" address %s", addr?addr:"unknow");
	//		 printf(" port %d", ntohs(sinp->sin_port));
	//	 }
	//	 printf("\n");
	// }

	// sockaddr_in1.sin_family = hp->h_addrtype;  /* address type */

	// 将地址填充到sockaddr_in的sin_addr
	// IPv4 address
	// if (hp->h_length > (int)sizeof(sockaddr_in1.sin_addr)) {
	//	 hp->h_length = sizeof(sockaddr_in1.sin_addr);
	// }
	// memcpy(&sockaddr_in1.sin_addr, hp->h_addr_list[0], hp->h_length);

	/* 创建socket */
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		fprintf(stderr, "create socket error\n");
		return;
	}
	fprintf(stderr, "%s\n", "create socket success");

	// sockaddr_in1.sin_port = port;  /* port number */


	// memcpy(&sockaddr_in1.sin_addr, ailist->, hp->h_length);
	/* 建立连接 
	 * 逐次尝试hostent->h_addr_list内的network address
	 */
	while (connect(s, ailist->ai_addr, ailist->ai_addrlen) < 0)
	{
		if (ailist->ai_next == NULL)
		{
			fprintf(stderr, "%s\n", "connect error");
			return;
		}
		ailist = ailist->ai_next;
	}

	fprintf(stderr, "%s\n", "connect success");

	//	 if (hp && hp->h_addr_list[1])
	//	 {
	//		 /* inet_neoa, inet_addr 二进制地址格式/点分十进制字符串 */
	//		 fprintf(stderr, "ftp: connect to address %s: ", inet_ntoa(sockaddr_in1.sin_addr));

	//		 hp->h_addr_list++;
	//		 memcpy(&sockaddr_in1.sin_addr, hp->h_addr_list[0], hp->h_length);

	//		 fprintf(stdout, "Trying %s...\n", inet_ntoa(sockaddr_in1.sin_addr));

	//		 /* 重新建立socket */
	//		 (void) close(s);
	//		 s = socket(sockaddr_in1.sin_family, SOCK_STREAM, 0);
	//		 if (s < 0) {
	//			 fprintf(stderr, "create socket error\n");
	//			 return;
	//		 }
	//		 continue;
	//	 }
	//	 fprintf(stderr, "connect error\n");

	//	 // bad
	//	 (void) close(s);
	//	 return;
	// }

	cin = fdopen(s, "r");
	cout = fdopen(s, "w");
	if (cin == NULL || cout == NULL) {
		fprintf(stderr, "fdopen failed.\n");
		if (cin)
			(void) fclose(cin);
		if (cout)
			(void) fclose(cout);
		// bad
		(void) close(s);
		return;
	}
	fprintf(stderr, "%s\n", "set io success");

	// int fputs(const char *s, FILE *stream);

	write(s, request, strlen(request));

	char buf[BUFLEN];
	int n;

	int fd = open(file, O_WRONLY | O_CREAT, 0775);

	while ((n = recv(s, buf, BUFLEN, 0)) > 0)
	{
		write(fd, buf, n);
	}
	if (n < 0)
	{
		fprintf(stderr, "%s\n", "recv error");
	}

	close(fd);

	// char g;
	// while ((g = getc(cin)) != '\0')
	// {
	//	 putchar(g);
	// }

	return;
}

char * getHost(char * url)
{
}

char * getGet(char * url)
{

}

char * strlink(char * str1, ...)
{
	char * str = malloc(sizeof(char));

	va_list arg_ptr; // 参数列表指针

	va_start(arg_ptr, str1); // 指向固定参数
	// va_copy(dest, src)

	char * strtemp = str1;

	while(abs(strcmp(strtemp, "__Last")))
	{
		str = realloc(str, strlen(str) + strlen(strtemp));
		strcat(str, strtemp);

		strtemp = va_arg(arg_ptr, char *); // 返回char*参数
	}

	va_end(arg_ptr); // va_start配套

	return str;
}