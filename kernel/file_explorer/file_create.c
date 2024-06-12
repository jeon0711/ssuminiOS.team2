#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "schedule_task.h"

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

typedef struct {
    char *folderPath;
    char *folderName;
} FolderArgs;

// 폴더 생성 함수
void *createFolderTask(void *arg) {
    FolderArgs *args = (FolderArgs *)arg;
    char fullPath[1024];

    snprintf(fullPath, sizeof(fullPath), "%s/%s", args->folderPath, args->folderName);

    #ifdef _WIN32
    if (mkdir(fullPath) == 0) {
        printf("폴더 생성 성공: %s\n", fullPath);
        printf(">");
    } else {
        perror("폴더 생성 실패");
        printf(">");
    }
    #else
    if (mkdir(fullPath, 0777) == 0) {
        printf("폴더 생성 성공: %s\n", fullPath);
        printf(">");
    } else {
        perror("폴더 생성 실패");
        printf(">");
    }
    #endif

    free(args->folderPath);
    free(args->folderName);
    free(args);
    return NULL;
}

// createFolder 함수
void createFolder() {
    char folderPath[1024];
    printf("생성하고자 하는 폴더 경로를 입력하세요: ");
    fgets(folderPath, sizeof(folderPath), stdin);
    folderPath[strcspn(folderPath, "\n")] = '\0';  // 개행 문자 제거

    char folderName[256];
    printf("생성하고자 하는 폴더 명을 입력하세요: ");
    fgets(folderName, sizeof(folderName), stdin);
    folderName[strcspn(folderName, "\n")] = '\0';  // 개행 문자 제거

    // FolderArgs 구조체에 정보 저장
    FolderArgs *args = (FolderArgs *)malloc(sizeof(FolderArgs));
    args->folderPath = strdup(folderPath);
    args->folderName = strdup(folderName);

    // scheduleTask 함수를 사용하여 스레드 생성 및 createFolderTask 실행
    scheduleTask(createFolderTask, (void *)args);
    
    // 메시지 출력
    printf("폴더 생성 작업이 백그라운드에서 실행됩니다.\n");
}
// 폴더 삭제 함수
void *deleteFolderTask(void *arg) {
    char *folderPath = (char *)arg;

    #ifdef _WIN32
    if (rmdir(folderPath) == 0) {
        printf("폴더 삭제 성공: %s\n", folderPath);
        printf(">");
    } else {
        perror("폴더 삭제 실패");
        printf(">");
    }
    #else
    if (rmdir(folderPath) == 0) {
        printf("폴더 삭제 성공: %s\n", folderPath);
        printf(">");
    } else {
        perror("폴더 삭제 실패");
        printf(">");
    }
    #endif

    free(folderPath);
    return NULL;
}

void deleteFolder() {
    char folderPath[1024];
    printf("삭제하고자 하는 폴더 경로를 입력하세요: ");
    fgets(folderPath, sizeof(folderPath), stdin);
    folderPath[strcspn(folderPath, "\n")] = '\0';  // 개행 문자 제거

    char *folderPathCopy = strdup(folderPath);
    scheduleTask(deleteFolderTask, (void *)folderPathCopy);
}

// 파일 복사 함수
void *copyFileTask(void *arg) {
    char **fileNames = (char **)arg;
    char *sourceFileName = fileNames[0];
    char *destinationFileName = fileNames[1];
    FILE *sourceFile, *destinationFile;
    char ch;

    sourceFile = fopen(sourceFileName, "r");
    if (sourceFile == NULL) {
        perror("원본 파일을 열 수 없음");
        free(sourceFileName);
        free(destinationFileName);
        free(fileNames);
        return NULL;
    }

    destinationFile = fopen(destinationFileName, "w");
    if (destinationFile == NULL) {
        perror("대상 파일을 생성할 수 없음");
        fclose(sourceFile);
        free(sourceFileName);
        free(destinationFileName);
        free(fileNames);
        return NULL;
    }

    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destinationFile);
    }

    printf("파일 복사 성공: %s -> %s\n", sourceFileName, destinationFileName);

    fclose(sourceFile);             
    fclose(destinationFile);

    free(sourceFileName);
    free(destinationFileName);
    free(fileNames);
    
    return NULL;
}

void copyFile() {
    char sourceFileName[1024];
    printf("복사하고자 하는 파일 명을 전체 경로로 입력하세요: ");
    fgets(sourceFileName, sizeof(sourceFileName), stdin);
    sourceFileName[strcspn(sourceFileName, "\n")] = '\0';  // 개행 문자 제거

    char destinationFileName[1024];
    printf("복사하려는 위치를 전체 경로로 입력하세요: ");
    fgets(destinationFileName, sizeof(destinationFileName), stdin);
    destinationFileName[strcspn(destinationFileName, "\n")] = '\0';  // 개행 문자 제거


    char **fileNames = (char **)malloc(2 * sizeof(char *));
    fileNames[0] = strdup(sourceFileName);
    fileNames[1] = strdup(destinationFileName);

    scheduleTask(copyFileTask, (void *)fileNames);
}

