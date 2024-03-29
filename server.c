#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log/log.h"

int main()
{
	log_start();

	int ret = 0;
	int servfd = socket(AF_INET, SOCK_STREAM, 0);
	if(servfd < 0)
	{
		DEBUG_ERR("create socket failed\n");
		return -1;
	}

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	ret = bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		DEBUG_ERR("bind port failed\n");
		return -1;
	}

	ret = listen(servfd, 1024);
	if(ret < 0)
	{
		DEBUG_ERR("listen failed\n");
		return -1;
	}

	char buf[1024] = {0};
	while(1)
	{
		memset(buf, 0, 1024);
		int cli = accept(servfd, NULL, NULL);
		if(cli > 0)
		{
			ret = read(cli, buf, 1024);
			DEBUG_INFO("get data: %s, datalen = %d\n", buf, ret);
			if(strncmp(buf, "exit", 4) == 0)
			{
				close(cli);
				break;
			}
		}
		close(cli);
	}
	close(servfd);

	log_end();
}

