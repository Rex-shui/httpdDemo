#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define TARGET_URL "https://s.weibo.com/top/summary"
#define TARGET_HOST "s.weibo.com"

void headers(int socketfd)
{
	char buf[1024] = {0};
	
	sprintf(buf, "GET %s HTTP/1.1", TARGET_URL);
	send(socketfd, buf, strlen(buf), 0);
	strcpy(buf, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
	send(socketfd, buf, strlen(buf), 0);
	strcpy(buf, "Accept-Language: zh-CN,zh;q=0.9");
	send(socketfd, buf, strlen(buf), 0);
	strcpy(buf, "Accept-Encoding: gzip, deflate, br");
	send(socketfd, buf, strlen(buf), 0);
	strcpy(buf, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.93 Safari/537.36");
	send(socketfd, buf, strlen(buf), 0);
	strcpy(buf, "Referer: https://www.baidu.com");
	send(socketfd, buf, strlen(buf), 0);
	sprintf(buf, "Host: %s", TARGET_HOST);
	send(socketfd, buf, strlen(buf), 0);
	strcpy(buf, "Connection: close");
	send(socketfd, buf, strlen(buf), 0);
}

char *getHostByDomin(const char *name)
{
	struct hostent *wb = gethostbyname(name);
	return inet_ntoa(*(struct in_addr *)wb->h_addr_list[0]);
}

int main()
{
	int rc = -1;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(getHostByDomin(TARGET_HOST));
	serv_addr.sin_port = htons(80);
	
	rc = connect(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
	if(rc < 0) {
		printf("connect err\n");
		return 0;
	}

	headers(fd);

	char buff[1024];
	rc = recv(fd, buff, 1024, 0);
	if(rc < 0) {
		printf("recv err\n");
		return 0;
	}
	
	printf("%s\n", buff);

	return 0;

}
