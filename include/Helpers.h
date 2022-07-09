#pragma once

#include <filesystem>
#include <iostream>

std::filesystem::path GetExecutablePath()
{
    return std::filesystem::canonical("/proc/self/exe");
}

std::string GetExecutableDir()
{
    std::filesystem::path exePath = GetExecutablePath();
    std::string aux(exePath.c_str());
    int pos = aux.rfind('/');

    // Get the path and the name
    std::string path = aux.substr(0,pos+1);
    std::string name = aux.substr(pos+1);
    // show results
    std::cout << "Path: " << path << std::endl;
    std::cout << "Name: " << name << std::endl;
    return path;
}