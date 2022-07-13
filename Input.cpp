#include "include/Input.h"

Input::Input()
{
    /* A few examples of information to gather */
    unsigned version;
    unsigned short id[4];                   /* or use struct input_id */
    char name[256] = "N/A";

    fprintf(stderr, "Opening keyboard device: %s\n", deviceName);

    if ((keyboardDesc = open(deviceName, O_RDONLY)) < 0) {
        fprintf(stderr,
            "ERR %d:\n"
            "Unable to open `%s'.  Try running as sudo.\n"
            "%s\n",
            errno, deviceName, strerror(errno)
        );
        attached = false;
        return;
    }
    attached = true;
    /* Error check here as well. */
    ioctl(keyboardDesc, EVIOCGVERSION, &version);
    ioctl(keyboardDesc, EVIOCGID, id);
    ioctl(keyboardDesc, EVIOCGNAME(sizeof(name)), name);

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

    inputHandlerThread = std::thread(&Input::InputHandler, this);
    inputHandlerThread.detach();
    printf("Successfully created input thread.\n");
}

Input::~Input()
{
    if (inputHandlerThread.joinable())
    {
        shouldExit = true;
        inputHandlerThread.join();
    }
    
    if (keyboardDesc)
    {
        close(keyboardDesc);
    }
}

// runs in a separate thread
void Input::InputHandler()
{
    // blocks until windowSetMode is false / space is pressed
    CaptureSetMode();
    // blocks until shouldExit is true
    CaptureMenuToggle();
}

void Input::CaptureSetMode()
{
    unsigned i;
    int sz, leftcount, rightcount ,upcount , downcount, numpluscount, numminuscount, numpad2count, numpad1count, numpad5count, numpad4count, spaceKeyCount;
    struct input_event ev[EV_BUF_SIZE];
    while(windowSetMode)
    {
        sz = read(keyboardDesc, ev, sizeof(struct input_event) * EV_BUF_SIZE);

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
                    windowSetMode = false;
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
                        xpos -= stepSize;  
                    }
                    break;
                case RIGHT_KEY:
                    rightcount += 1;
                    if (rightcount % 2 == 0)
                    {
                        xpos += stepSize;  
                    }
                    break;
                case UP_KEY:
                    upcount += 1;
                    if (upcount % 2 == 0)
                    {
                        ypos -= stepSize;
                    }
                    break;
                case DOWN_KEY:
                    downcount += 1;
                    if (downcount % 2 == 0)
                    {
                        ypos += stepSize;
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
                        width += stepSize;
                    }
                    break;
                case NUMPAD_1:
                    numpad1count += 1;
                    if (numpad1count % 2 == 0)
                    {
                        width -= stepSize;
                    }
                    break;
                case NUMPAD_5:
                    numpad5count += 1;
                    if (numpad5count % 2 == 0)
                    {
                        height += stepSize;
                    }
                    break;
                case NUMPAD_4:
                    numpad4count += 1;
                    if (numpad4count % 2 == 0)
                    {
                        height -= stepSize;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void Input::CaptureMenuToggle()
{
    unsigned i;
    int sz, count;
    struct input_event ev[EV_BUF_SIZE];
    while(!shouldExit)
    {
        sz = read(keyboardDesc, ev, sizeof(struct input_event) * EV_BUF_SIZE);

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
                    printf("Insert pressed, Toggling Menu\n");
                    showMenu = !showMenu;
                }
            }
            
        }
    }
}