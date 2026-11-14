#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

static void msg(const char *m) {
    fprintf(stderr, "%s\n", m);
}

static void die(const char *msg) {
    perror(msg);
    exit(1);
}

// 1 read() & 1 write()
static void do_something(int connfd) {
    char rbuf[64];
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        rbuf[n] = '\0';
        msg("read() error");
        return;
    }
    printf("muahahahhaha client says %s\n", rbuf);
    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main(void) {
    // init socket w/ ipv4, tcp, default protocol
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }
    int val = 1;
    // set so_reuseaddr to 1 so server can bind to same ip port after restart
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind to 0.0.0.0:1234
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234); // port in big endian
    addr.sin_addr.s_addr = htonl(0); // wildcard ip 0.0.0.0
    int rv = bind(fd, (const struct sockaddr*)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    // accept
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;
        }
        do_something(connfd);
        close(connfd);
    }
}






