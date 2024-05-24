#ifndef BASIC_INCLUDE
#define BASIC_INCLUDE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define GetCurrentDir getcwd
#endif

#define NAMEMAX 255
#define PATHMAX 4096
#define STRMAX 4096

#endif
