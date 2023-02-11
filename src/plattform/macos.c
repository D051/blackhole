#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/kern_event.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/kern_control.h>
#include <ctype.h>
#include <fcntl.h>

#define UTUN_OPT_IFNAME 2
#define UTUN_CONTROL_NAME "com.apple.net.utun_control"

int allocate_socket() {
    // allocate socket
    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    // return socket
    return fd;
}

int configure_socket(int fd, unsigned char *name) {
    // allocate error integer
    int err;
    // allocate and configure the socket info
    struct ctl_info info; bzero(&info, sizeof(info));
    // configure the io control command
    strncpy(info.ctl_name, UTUN_CONTROL_NAME, MAX_KCTL_NAME);
    // execute io control command
    err = ioctl(fd, CTLIOCGINFO, &info);
    if (err != 0) {
        close(fd); return err;
    };
    // allocate and configure the socket address control command
    struct sockaddr_ctl addr;
    addr.sc_len     = sizeof(addr);
    addr.sc_family  = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_id      = info.ctl_id;
    addr.sc_unit    = 0;
    // execute io control command
    err = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (err != 0) {
        printf("error : %d while connect", err);
        close(fd); return err;
    };
    // get the system interface name
    socklen_t len = 32;
    err = getsockopt(fd, SYSPROTO_CONTROL, 2, name, &len);
    if (err != 0) {
        printf("error : %d while getsockopt", err);
        close(fd); return err;
    };
    // return no error
    return 0;
}


// create a tun device
int create_tun_device(unsigned char *name) {
    // allocate error integer
    int err;
    // get socket file descriptor
    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd < 0) {
        return fd;
    };
    // allocate and configure the socket info control
    struct ctl_info info;
    bzero(&info, sizeof(info));
    strncpy(info.ctl_name, "com.apple.net.utun_control", MAX_KCTL_NAME);
    // execute io control command
    err = ioctl(fd, CTLIOCGINFO, &info);
    if (err != 0) {
        close(fd); return err;
    };
    // allocate and configure the socket address control
    struct sockaddr_ctl addr;
    addr.sc_len = sizeof(addr);
    addr.sc_family = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_id = info.ctl_id;
    addr.sc_unit = 0;
    // connect the socket to the control
    err = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (err != 0) {
        printf("error : %d while connect", err);
        close(fd); return err;
    };
    // get the interface name
    unsigned char ifname[32];
    socklen_t ifname_len = sizeof(ifname);
    // socklen_t ifname_len = sizeof(32);
    err = getsockopt(fd, SYSPROTO_CONTROL, 2, name, &ifname_len);
    if (err != 0) {
        printf("error : %d while getsockopt", err);
        close(fd); return err;
    };
    // return file descriptor
    return fd;
}
