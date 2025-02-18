from threading import *
from socket import *
from signal import *

def listener(tid):
	while True:
		s = socket(AF_VSOCK, SOCK_SEQPACKET)
		s.bind((1, 1234))
		s.listen()
		pthread_kill(tid, SIGUSR1)

signal(SIGUSR1, lambda *args: None)
Thread(target=listener, args=[get_ident()]).start()

while True:
	c = socket(AF_VSOCK, SOCK_SEQPACKET)
	c.connect_ex((1, 1234))
	c.connect_ex((42, 1234))
