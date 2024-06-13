#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include "schedule_task.h"

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

typedef char* Path;

void *listDirectoryTask(void* dirPath) {
    DIR *dp = opendir(dirPath);
    if (!dp) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        printf("%s\n", entry->d_name);
    }

    closedir(dp);
    return NULL;
}

void listDirectory() {
    Path folderPath = (char *)malloc(sizeof(char *));
    printf("enter the path to list: ");
    fgets(folderPath, sizeof(folderPath), stdin);
    folderPath[strcspn(folderPath, "\n")] = '\0';  // 개행 문자 제거
    //getchar();  // 버퍼 비우기

    scheduleTask(listDirectoryTask, (void *)folderPath);
}

void *executeFileTask(void *filepath) {
    struct stat sb;
    if (stat(filepath, &sb) != 0) {
        printf("파일이 존재하지 않거나 실행 가능하지 않습니다.\n");
        return NULL;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *path_copy = strdup(filepath);
        char *file_name = basename(path_copy);
        char *extension = strrchr(file_name, '.');
        if (extension && (strcmp(extension, ".txt") == 0 || strcmp(extension, ".c") == 0)) {
            execlp("nano", "nano", filepath, NULL);
        } else {
            execl(filepath, file_name, NULL);
        }

        perror("execl");
        free(path_copy);
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        waitpid(pid, NULL, 0);
    }
    return NULL;
}

void executeFile() {
    Path filePath = (char *)malloc(256* sizeof(char *));
    printf("enter the full path of file to execute: ");
    fgets(filePath, 256 * sizeof(filePath), stdin);
    filePath[strcspn(filePath, "\n")] = '\0';  // 개행 문자 제거
    //getchar();  // 버퍼 비우기
    printf(filePath);
    printf("\n");
    scheduleTask(executeFileTask, (void *)filePath);
}
