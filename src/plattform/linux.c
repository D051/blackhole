#include <linux /if.h>
#include <linux /if_tun.h>


int allocate_socket() {
    // allocate socket
    int fd = open("/dev/net/tun", O_RDWR)
    // return socket
    return fd;
}

int configure_socket(int fd, unsigned char *name) {
    // allocate and configure the socket address control command
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    // set the interface to tun mode and disable packet info
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    // set the interface name
    strncpy(ifr.ifr_name, (char *)name, 31);
    // execute io control command
    int result = ioctl(fd, TUNSETIFF, &ifr);
	if (result < 0) {
		return result;
	}
    // get the system interface name
    strncpy((char*)name, ifr.ifr_name, 31);
    // return no error
    return 0;
}
