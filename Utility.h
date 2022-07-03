#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

class Utility
{
private:
    
public:
    Utility();
    ~Utility();
    static int GetProcessPID(const char* procName, bool wait);
    static long GetModuleHandle(const char* moduleName, int procID);
    static long GetFreeSpaceAddr(int procID);
};

Utility::Utility()
{
}

Utility::~Utility()
{
}


int Utility::GetProcessPID(const char* procName, bool wait = false)
{
    char pidLine[1024];
    char commandStr[] = "pidof "; 
    strcat(commandStr, procName);
    FILE * command = popen(commandStr, "r");

    if (command == NULL)
    {
        printf("Unable to open pidof, or invalid argument\n");
        return 0;
    }

    fgets(pidLine, 1024, command);
    __pid_t pid = strtoul(pidLine, NULL, 10);
     pclose(command);
    if (wait && pid == 0)
    {
        printf("Waiting for %s to start...\n", procName);
        while (pid == 0)
        {
            sleep(3);
            command = popen(commandStr, "r");
            fgets(pidLine, 1024, command);
            pid = strtoul(pidLine, NULL, 10);
            pclose(command);
        }
    }
    else if (pid == 0)
    {
        printf("%s is not currently running...\n", procName);
        return pid;
    }
    printf("%s's process id is %i\n",procName, pid);
    return pid;
}

long Utility::GetModuleHandle(const char* moduleName, int procID)
{
    size_t buffSize = 0x1000;
    char* buffer = (char *)malloc(buffSize * sizeof(char));
    sprintf(buffer, "/proc/%d/maps", procID);
    FILE * fp = fopen(buffer, "r");
    long addr;
    //char str[20];
    if (!fp)
    {
        perror("fopen");
        abort();
    }
    while (fgets(buffer, buffSize, fp))
    {
        char* ret = strstr(buffer, moduleName);
        if (ret)
        {
            //sscanf(buffer, "%lx-%*lx %*s %*s %s", &addr, str, str, str, str);
            sscanf(buffer, "%lx-%*x %*s %*s %*s", &addr);
            printf("Found base address of %s: 0x%lx\n", moduleName, addr);
            return addr;
        }
    }
    printf("Failed to find module %s\n", moduleName);
    free(buffer);
    fclose(fp);
    return 0;
}

long Utility::GetFreeSpaceAddr(int procID)
{
    FILE *fp;
    char filename[30];
    char line[85];
    long startAddr;
    char owner[20];
    long endAddr;
    char prot[20];
    sprintf(filename, "/proc/%d/maps", procID);
    fp = fopen(filename, "r");
    if(fp == NULL)
        exit(1);
    while(fgets(line, 85, fp) != NULL) {
        // sscanf(line, "%lx-%*lx %*s %*s %s", &addr,
        //        endAddr, prot, str, str);
        sscanf(line, "%lx-%lx %s %*s %s", &startAddr,
               &endAddr, prot, owner);
        if(strcmp(owner, "00:00") == 0)
        {
            printf("Free space found at: 0x%lx\n", startAddr);
            return startAddr;
        }
            
    }
    fclose(fp);
    printf("Unable to find free space in process...\n");
    return 0;
}