#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

// 폴더 생성 함수
void createFolder() {
    char *create_input;
    create_input  = readline("생성하고자 하는 폴더 명을 입력하세요: ");

    #ifdef _WIN32
    if (mkdir(create_input) == 0) {
        printf("폴더 생성 성공: %s\n", create_input);
    } else {
        printf("폴더 생성 실패: %s\n", create_input);
    }
    #else
    if (mkdir(create_input, 0777) == 0) {
        printf("폴더 생성 성공: %s\n", create_input);
    } else {
        printf("폴더 생성 실패: %s\n", create_input);
    }
    #endif

}

// 폴더 삭제 함수
void deleteFolder() {
    char *delete_input;
    delete_input  = readline("삭제하고자 하는 폴더 명을 입력하세요: ");
    
    #ifdef _WIN32
    if (rmdir(delete_input) == 0) {
        printf("폴더 삭제 성공: %s\n", delete_input);
    } else {
        printf("폴더 삭제 실패: %s\n", delete_input);
    }
    #else
    if (rmdir(delete_input) == 0) {
        printf("폴더 삭제 성공: %s\n", delete_input);
    } else {
        printf("폴더 삭제 실패: %s\n", delete_input);
    }
    #endif
}

// 파일 복사 함수
void copyFile() {
    char *sourceFileName;
    char *destinationFileName;
    FILE *sourceFile, *destinationFile;
    char ch;

    sourceFileName = readline("복사하고자 하는 파일 명을 전체 경로로 입력하세요: ");
    destinationFileName = readline("복사하려는 위치를 전체 경로로 입력하세요: ");
    
    sourceFile = fopen(sourceFileName, "r");
    if (sourceFile == NULL) {
        printf("원본 파일을 열 수 없음: %s\n", sourceFileName);
        free(sourceFileName);
        free(destinationFileName);
        return;
    }
    destinationFile = fopen(destinationFileName, "w");
    if (destinationFile == NULL) {
        printf("대상 파일을 생성할 수 없음: %s\n", destinationFileName);
        fclose(sourceFile);
        free(sourceFileName);
        free(destinationFileName);
        return;
    }
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destinationFile);
    }
    printf("파일 복사 성공: %s -> %s\n", sourceFileName, destinationFileName);
    fclose(sourceFile);             
    fclose(destinationFile);

    // 동적 할당된 메모리 해제
    free(sourceFileName);
    free(destinationFileName);
}   
