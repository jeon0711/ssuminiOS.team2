#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
//#include "schedule_task.h"

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

void *createFolderTask(void *arg);
void *deleteFolderTask(void *arg);
void *copyFileTask(void *arg);
void *listDirectoryTask(void *dirPath);
void *executeFileTask(void *filepath);
void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input = (char *)malloc(sizeof(char *));
    while (1) {
        // 여러 줄로 프롬프트 출력
        printf("================================\n");
        printf("사용하고자 하는 명령어를 입력하세요\n");
        printf("입력가능 명령어\n");
        printf("createFolder, deleteFolder, copyFile, fileSearch, listFolder, execFile\n");
        printf("종료: exit\n");
        printf("================================\n");
        //input = readline(">");
        fgets(input, 16*sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input,"exit") == 0) {
            break;
        }
	else if (strcmp(input, "createFolder") == 0) {
            createFolder();
        }
        else if (strcmp(input, "deleteFolder") == 0) {
            deleteFolder();
        }
        else if (strcmp(input, "copyFile") == 0) {
            copyFile();
	}
	else if (strcmp(input, "fileSearch") == 0){
		filesearch(NULL, NULL);	
	}
	else if (strcmp(input, "listFolder") == 0){
		listDirectory();
	}
	else if (strcmp(input, "execFile") == 0){
		executeFile();
	}
	else system(input);
    }

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);
}

void print_minios(char* str) {
        printf("%s\n",str);
}
