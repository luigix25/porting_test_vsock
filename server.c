#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <string.h>
#include <stdio.h>

int main()
{
	int s = socket(AF_VSOCK, SOCK_STREAM, 0);

	struct sockaddr_vm addr;

	memset(&addr, 0, sizeof(struct sockaddr_vm));
	addr.svm_family = AF_VSOCK;
	addr.svm_port = 9999;
	addr.svm_cid = VMADDR_CID_ANY;

	int c = bind(s, (struct sockaddr *)&addr, sizeof(addr));
	if(c == -1){
		perror("bind");
		return -1;
	}

	listen(s, 0);

	struct sockaddr_vm peer_addr;
	socklen_t peer_addr_size = sizeof(struct sockaddr_vm);
	int peer_fd = accept(s, (struct sockaddr *)&peer_addr, &peer_addr_size);

	char buf[64];
	size_t msg_len;
	while ((msg_len = recv(peer_fd, &buf, 64, 0)) > 0) {
		printf("Received %lu bytes: %.*s\n", msg_len, msg_len, buf);
	}

	return 0;
}
