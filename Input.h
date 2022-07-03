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

//#define DEBUG_PRINTS

#define EV_BUF_SIZE 16

#define INSERT_KEY 458825
#define LEFT_KEY 458832
#define RIGHT_KEY 458831
#define UP_KEY 458834
#define DOWN_KEY 458833
#define SPACE_KEY 458796
#define NUMPAD_PLUS 458839
#define NUMPAD_MINUS 458838
#define NUMPAD_2 458842
#define NUMPAD_1 458841
#define NUMPAD_5 458845
#define NUMPAD_4 458844

extern bool showMenu;
extern bool shouldExit;
extern bool windowSetMode;
extern int g_xpos, g_ypos, g_width, g_height;

void InputHandler(int fd, const char * deviceName)
{
    int stepSize = 100;
    /* Loop. Read event file and parse result. */
    int sz;
    unsigned i;
    int count = 0, leftcount, rightcount ,upcount , downcount, numpluscount, numminuscount, numpad2count, numpad1count, numpad5count, numpad4count;
    int spaceKeyCount = 0;
    struct input_event ev[EV_BUF_SIZE]; /* Read up to N events at a time */
    
    while(windowSetMode)
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
        for (i = 0; i < sz / sizeof(struct input_event); ++i) {
#ifdef DEBUG_PRINTS
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
#endif
            if (ev[i].value == SPACE_KEY && ev[i].code == 4 && ev[i].type == 4)
            {
                // registered twice, press and release
                spaceKeyCount += 1;
                if (spaceKeyCount % 2 == 0)
                {
                    printf("Space key pressed, Setting window's final x,y,width,height\n");
                    windowSetMode = !windowSetMode;
                }
            }
    
            if (ev[i].code == 4 && ev[i].type == 4)
            {
                switch (ev[i].value)
                {             
                case LEFT_KEY:
                    leftcount += 1;
                    if (leftcount % 2 == 0)
                    {
                        g_xpos -= stepSize;  
                    }
                    break;
                case RIGHT_KEY:
                    rightcount += 1;
                    if (rightcount % 2 == 0)
                    {
                        g_xpos += stepSize;  
                    }
                    break;
                case UP_KEY:
                    upcount += 1;
                    if (upcount % 2 == 0)
                    {
                        g_ypos -= stepSize;
                    }
                    break;
                case DOWN_KEY:
                    downcount += 1;
                    if (downcount % 2 == 0)
                    {
                        g_ypos += stepSize;
                    }
                    break;
                case NUMPAD_PLUS:
                    numpluscount += 1;
                    if (numpluscount % 2 == 0)
                    {
                        if (stepSize <= 100)
                        {
                            if (stepSize >= 0 || stepSize <= 10)
                            {
                                stepSize += 1;
                            }
                            else
                                stepSize += 10;
                            printf("Changing stepsize to %i\n", stepSize);
                            
                        }
                    }
                    break;
                case NUMPAD_MINUS:
                    numminuscount += 1;
                    if (numminuscount % 2 == 0)
                    {
                        if (stepSize >= 0)
                        {
                            if (stepSize <= 10)
                            {
                                stepSize -= 1;
                            }
                            else
                                stepSize -= 10;
                            printf("Changing stepsize to %i\n", stepSize);
                        }
                    }
                    break;
                case NUMPAD_2:
                    numpad2count += 1;
                    if (downcount % 2 == 0)
                    {
                        g_width += stepSize;
                    }
                    break;
                case NUMPAD_1:
                    numpad1count += 1;
                    if (numpad1count % 2 == 0)
                    {
                        g_width -= stepSize;
                    }
                    break;
                case NUMPAD_5:
                    numpad5count += 1;
                    if (numpad5count % 2 == 0)
                    {
                        g_height += stepSize;
                    }
                    break;
                case NUMPAD_4:
                    numpad4count += 1;
                    if (numpad4count % 2 == 0)
                    {
                        g_height -= stepSize;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    
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
#ifdef DEBUG_PRINTS
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
#endif
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

class Input
{
private:
    int keyboard_fd;
    // could be different on different systems...
    const char* deviceName = "/dev/input/event2";
public:
    Input();
    ~Input();
};

Input::Input()
{
    /* A few examples of information to gather */
    unsigned version;
    unsigned short id[4];                   /* or use struct input_id */
    char name[256] = "N/A";

    fprintf(stderr, "Opening keyboard device: %s\n", deviceName);

    if ((keyboard_fd = open(deviceName, O_RDONLY)) < 0) {
        fprintf(stderr,
            "ERR %d:\n"
            "Unable to open `%s'\n"
            "%s\n",
            errno, deviceName, strerror(errno)
        );
    }
    /* Error check here as well. */
    ioctl(keyboard_fd, EVIOCGVERSION, &version);
    ioctl(keyboard_fd, EVIOCGID, id); 
    ioctl(keyboard_fd, EVIOCGNAME(sizeof(name)), name);

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


    std::thread t1(InputHandler, keyboard_fd, deviceName);
    t1.detach();
}

Input::~Input()
{
    close(keyboard_fd);
}