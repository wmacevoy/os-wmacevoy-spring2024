// client.c
#include <zmq.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(requester, "ipc://test.ipc");
    
    char buffer[20];
    zmq_recv(requester, buffer, 20, 0);
    printf("Received message: %s\n", buffer);

    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}
