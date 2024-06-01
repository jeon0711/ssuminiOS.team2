#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;

    while (1) {
        // 여러 줄로 프롬프트 출력
        printf("================================\n");
        printf("사용하고자 하는 명령어를 입력하세요\n");
        printf("입력가능 명령어\n");
        printf("createFolder, deleteFolder, copyFile, fileSearch, filePath\n");
        printf("종료: exit\n");
        printf("================================\n");
        input = readline(">");

            if (strcmp(input,"exit") == 0) {
            break;
        }

	if (strcmp(input, "createFolder") == 0){
		createFolder();
        }
        
	else if (strcmp(input, "deleteFolder") == 0){
		deleteFolder();
	}

	else if (strcmp(input, "copyFile") == 0){
		copyFile();
	}

	else if (strcmp(input, "fileSearch") == 0){
		filesearch(NULL, NULL);	
	}
	
	else if (strcmp(input, "filePath") == 0){
		path_main();
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
