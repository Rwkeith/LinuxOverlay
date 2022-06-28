#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <string.h>     /* strerror() */
#include <errno.h>      /* errno */

#include <fcntl.h>      /* open() */
#include <unistd.h>     /* close() */
#include <sys/ioctl.h>  /* ioctl() */

#include <linux/input.h>    /* EVIOCGVERSION ++ */

#include <thread>

#define EV_BUF_SIZE 16

#define INSERT_KEY 458825

extern bool showMenu;
extern bool shouldExit;

void InputHandler(int fd, char * deviceName)
{

    /* Loop. Read event file and parse result. */
    int sz;
    unsigned i;
    int count = 0;
    struct input_event ev[EV_BUF_SIZE]; /* Read up to N events at a time */
    while(!shouldExit)
    {
        sz = read(fd, ev, sizeof(struct input_event) * EV_BUF_SIZE);

        if (sz < (int) sizeof(struct input_event)) {
            fprintf(stderr,
                "ERR %d:\n"
                "Reading of `%s' failed\n"
                "%s\n",
                errno, deviceName, strerror(errno)
            );
            return;
        }

        /* Implement code to translate type, code and value */
        for (i = 0; i < sz / sizeof(struct input_event); ++i) {
            fprintf(stderr,
                "%ld.%06ld: "
                "type=%02x "
                "code=%02x "
                "value=%02x\n",
                ev[i].time.tv_sec,
                ev[i].time.tv_usec,
                ev[i].type,
                ev[i].code,
                ev[i].value
            );
            printf("ev[i].value as signed int: %i\n", ev[i].value);
            if (ev[i].value == INSERT_KEY && ev[i].code == 4 && ev[i].type == 4)
            {
                // registered twice, press and release
                count += 1;
                if (count % 2 == 0)
                {
                    printf("INSERT KEY IS PRESSED, Toggling Menu\n");
                    showMenu = !showMenu;
                }
            }
            
        }
    }
}

class input
{
private:
    int fd;
    // could be different on different systems...
    char* deviceName = "/dev/input/event2";
public:
    input();
    ~input();
};

input::input()
{
    /* A few examples of information to gather */
    unsigned version;
    unsigned short id[4];                   /* or use struct input_id */
    char name[256] = "N/A";

    fprintf(stderr, "Opening keyboard device: %s\n", deviceName);

    if ((fd = open(deviceName, O_RDONLY)) < 0) {
        fprintf(stderr,
            "ERR %d:\n"
            "Unable to open `%s'\n"
            "%s\n",
            errno, deviceName, strerror(errno)
        );
    }
    /* Error check here as well. */
    ioctl(fd, EVIOCGVERSION, &version);
    ioctl(fd, EVIOCGID, id); 
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);

    fprintf(stderr,
        "Name      : %s\n"
        "Version   : %d.%d.%d\n"
        "ID        : Bus=%04x Vendor=%04x Product=%04x Version=%04x\n"
        "----------\n"
        ,
        name,

        version >> 16,
        (version >> 8) & 0xff,
        version & 0xff,

        id[ID_BUS],
        id[ID_VENDOR],
        id[ID_PRODUCT],
        id[ID_VERSION]
    );


    std::thread t1(InputHandler, fd, deviceName);
    t1.detach();
}

input::~input()
{
    close(fd);
}

/*
for insert:
1656442620.536780: type=04 code=04 value=70049
1656442620.536780: type=01 code=6e value=01
1656442620.536780: type=00 code=00 value=00
*/