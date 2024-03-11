// server.c
#include <zmq.h>
#include <stdio.h>
// #include <unistd.h>
#include <string.h>

int main(void) {
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_PAIR);
    int rc = zmq_bind(responder, "ipc://test.ipc");
    if (rc != 0) {
        printf("Server failed to bind: %s\n", zmq_strerror(errno));
        return -1;
    }

    printf("Server waiting for client connection...\n");
    zmq_send(responder, "Hello from Server!", 18, 0);
    zmq_close(responder);
    zmq_ctx_destroy(context);
    return 0;
}
