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


class Input
{
private:
    int keyboardDesc;
    // could be different on different systems...
    const char* deviceName = "/dev/input/event2";
    std::thread inputHandlerThread;
public:
    int xpos, ypos, width, height;
    int stepSize = 100;
    bool shouldExit, attached, showMenu;
    bool windowSetMode = true;
    Input();
    ~Input();
    void InputHandler();
    void CaptureSetMode();
    void CaptureMenuToggle();
};
