#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void prova(int signal){
}

int main()
{

	signal(SIGUSR1, prova);
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

	pid_t pid = getpid();
	printf("my pid : %d\n",pid);

	c = send(s, &pid, sizeof(pid_t), 0);
	if(c == -1){
		perror("send");
		return -1;
	}

	close(s);

	while(1){
		memset(&addr.svm, 0, sizeof(struct sockaddr_vm));
		addr.svm.svm_family = AF_VSOCK;
		addr.svm.svm_port = 9999;
		addr.svm.svm_cid = VMADDR_CID_LOCAL;

		int s = socket(AF_VSOCK, SOCK_SEQPACKET, 0);
		connect(s, &addr.sa, sizeof(addr.svm));

		addr.svm.svm_cid = 42;
		connect(s, &addr.sa, sizeof(addr.svm));
		close(s);
	}

	return 0;
}
