#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <linux/wireless.h>

int main(int argc, char ** argv) {
    for(int i = 1; i < argc; i++) {
        auto  fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        iwreq iwr;
        strcpy(iwr.ifr_name, argv[i]);

        char id[IW_ESSID_MAX_SIZE];
        iwr.u.essid.pointer = id;
        iwr.u.essid.length  = IW_ESSID_MAX_SIZE;

        if(ioctl(fd, SIOCGIWESSID, &iwr) == -1) {
            ifreq ifr;
            strcpy(ifr.ifr_name, argv[i]);

            if(ioctl(fd, SIOCGIFADDR, &ifr) == -1) { return 1; }
            sockaddr_in * ipaddr = (sockaddr_in *)&ifr.ifr_addr;

            fprintf(stdout, "%s\n", inet_ntoa(ipaddr->sin_addr));
        } else {
            ifreq ifr;
            strcpy(ifr.ifr_name, argv[i]);

            if(ioctl(fd, SIOCGIFADDR, &ifr) == -1) { return 1; }
            sockaddr_in * ipaddr = (sockaddr_in *)&ifr.ifr_addr;

            ((char *)iwr.u.essid.pointer)[iwr.u.essid.length] = '\0';
            // fprintf(stdout, "length: %ld", iwr.u.essid.length);
            fprintf(stdout, "%s|%s\n", iwr.u.essid.pointer,
                    inet_ntoa(ipaddr->sin_addr));
        }
    }
}
