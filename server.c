#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void start_server(int s){

	struct sockaddr_vm addr;

	memset(&addr, 0, sizeof(struct sockaddr_vm));
	addr.svm_family = AF_VSOCK;
	addr.svm_port = 9999;
	addr.svm_cid = VMADDR_CID_ANY;

	int c = bind(s, (struct sockaddr *)&addr, sizeof(addr));
	if(c == -1){
		perror("bind");
		return;
	}

	c = listen(s, 0);
	if(c == -1){
		perror("listen");
		return;
	}

}


int main()
{
	int s = socket(AF_VSOCK, SOCK_STREAM, 0);

	start_server(s);

	struct sockaddr_vm peer_addr;
	socklen_t peer_addr_size = sizeof(struct sockaddr_vm);
	int peer_fd = accept(s, (struct sockaddr *)&peer_addr, &peer_addr_size);

	pid_t pid_altro;

	recv(peer_fd, &pid_altro, sizeof(pid_t), 0);

	close(peer_fd);
	close(s);
	printf("pid client: %d\n",pid_altro);

	while(1){
		int s = socket(AF_VSOCK, SOCK_SEQPACKET, 0);
		start_server(s);
		kill(pid_altro, SIGUSR1);
		close(s);
	}

	return 0;
}
