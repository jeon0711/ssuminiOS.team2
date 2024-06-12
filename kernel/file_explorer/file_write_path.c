#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/wait.h>
#include "schedule_task.h"

void* list_directory_task(void* arg) {
    const char* path = (const char*)arg;
    DIR* dp = opendir(path);
    if (!dp) {
        perror("opendir");
    } else {
        struct dirent* entry;
        while ((entry = readdir(dp))) {
            printf("%s\n", entry->d_name);
        }
        closedir(dp);
    }

    free(arg);
    return NULL;
}

void* execute_file_task(void* arg) {
    const char* path = (const char*)arg;
    struct stat sb;
    if (stat(path, &sb) != 0) {
        printf("파일이 존재하지 않거나 실행 가능하지 않습니다.\n");
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            char* path_copy = strdup(path);
            char* file_name = basename(path_copy);
            char* extension = strrchr(file_name, '.');
            if (extension && (strcmp(extension, ".txt") == 0 || strcmp(extension, ".c") == 0)) {
                execl("/bin/cat", "cat", path, NULL);
            } else {
                execl(path, file_name, NULL);
            }
            perror("execl");
            free(path_copy);
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
        } else {
            waitpid(pid, NULL, 0);
        }
    }

    free(arg);
    return NULL;
}

void file_explorer() {
    char command[256], path[256];
    while (1) {
        printf("추가 명령어를 입력하세요. (list <directory>, exec <file>, or exit): ");
        if (!fgets(command, sizeof(command), stdin)) break;
        command[strcspn(command, "\n")] = 0;
        if (strcmp(command, "exit") == 0) {
            printf("Exiting file explorer.\n");
            break;
        } else if (sscanf(command, "list %s", path) == 1) {
            char* path_copy = strdup(path);
            scheduleTask(list_directory_task, path_copy);
        } else if (sscanf(command, "exec %s", path) == 1) {
            char* path_copy = strdup(path);
            scheduleTask(execute_file_task, path_copy);
        } else {
            printf("입력 오류가 발생했습니다. Use 'list <directory>', 'exec <file>', or 'exit'.\n");
        }
    }
}

int path_main() {
    file_explorer();
    return 0;
}


