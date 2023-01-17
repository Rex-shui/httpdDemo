#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int ret = 0;
	int clifd = socket(AF_INET, SOCK_STREAM, 0);
	if(clifd < 0)
	{
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(clifd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret < 0)
	{
		return -1;
	}

	char buf[32] = "abcdefihijklmn";
	if(argc >= 2)
	{
		memset(buf, 0, 32);
		strncpy(buf, argv[1], 32);
	}
	ret = write(clifd, buf, strlen(buf));
	printf("send data: %s, sendlen = %d\n", buf, ret);
	close(clifd);
}

