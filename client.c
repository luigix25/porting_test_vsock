#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int s = socket(AF_VSOCK, SOCK_STREAM, 0);

	union {
		struct sockaddr sa;
		struct sockaddr_vm svm;
	} addr;

	memset(&addr.svm, 0, sizeof(struct sockaddr_vm));
	addr.svm.svm_family = AF_VSOCK;
	addr.svm.svm_port = 9999;
	addr.svm.svm_cid = VMADDR_CID_LOCAL;

	int c = connect(s, &addr.sa, sizeof(addr.svm));
	if(c == -1){
		perror("connect");
		return -1;
	}

	c = send(s, "Hello, world!", 13, 0);
	if(c == -1){
		perror("send");
		return -1;
	}


	close(s);

	return 0;
}
