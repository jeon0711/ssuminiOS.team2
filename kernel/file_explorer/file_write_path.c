#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/wait.h>

void list_directory(const char *path) {
    DIR *dp = opendir(path);
    if (!dp) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        printf("%s\n", entry->d_name);
    }

    closedir(dp);
}

void execute_file(const char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0) {
        printf("File does not exist or is not executable\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *path_copy = strdup(path);
        char *file_name = basename(path_copy);
        char *extension = strrchr(file_name, '.');
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

void file_explorer() {
    char command[256], path[256];
    while (1) {
        printf("Enter command (list <directory>, exec <file>, or exit): ");
        if (!fgets(command, sizeof(command), stdin)) break;
        command[strcspn(command, "\n")] = 0;
        if (strcmp(command, "exit") == 0) {
            printf("Exiting file explorer.\n");
            break;
        } else if (sscanf(command, "list %s", path) == 1) {
            list_directory(path);
        } else if (sscanf(command, "exec %s", path) == 1) {
            execute_file(path);
        } else {
            printf("Invalid command. Use 'list <directory>', 'exec <file>', or 'exit'.\n");
        }
    }
}

int path_main() {
    file_explorer();
    return 0;
}


