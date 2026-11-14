#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    // init socket w/ ipv4, tcp, dummy data
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    // set so_reuseaddr to 1 so server can bind to same ip port after restart
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind to 0.0.0.0:1234
    struct sockadrr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234); // port
    addr.sin_addr.s_addr = htonl(0); // wildcard ip 0.0.0.0
    int rv = bind(fd, (const struct sockaddr*)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    

};

struct sockaddr_in {
    uint16_t sin_family; 
    uint16_t sin_port;
    struct in_addr sin_addr;
};
        
struct in_addr {
    uint32_t s_addr;    
};




