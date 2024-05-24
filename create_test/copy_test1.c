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

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }

	if (strcmp(input,"minisystem") == 0){
		minisystem();
        }
        
	else if (strcmp(input,"printme") == 0){
		printme();
	}

	else if (strcmp(input,"tryfork") == 0){
		tryfork();
	}

	else if (strcmp(input,"tryipc") == 0){
		tryipc();	
	}

	else if (strcmp(input,"montecarlo") == 0){
		montecarlo();
	}
	else if (strcmp(input,"roundrobin") == 0){
		round_robin();
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
