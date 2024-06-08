//schedule_task.c
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_THREADS 3 // 최대 스레드 수

static int running_threads = 0; // 현재 실행 중인 스레드 수

void scheduleTask(void *(*task)(void *), void *arg) {
    while (1) {
        // 현재 실행 중인 스레드 수가 최대 스레드 수에 도달할 때까지 대기
        while (running_threads > MAX_THREADS) {
            usleep(1000000); // 0.1초 대기
            printf("thread is waiting..\n");
        }
        
        // 스레드 생성
        pthread_t thread;
        pthread_create(&thread, NULL, task, arg);
        pthread_detach(thread);
        
        // 현재 실행 중인 스레드 수 증가
        running_threads++;

        // 스레드 실행 후 반복문 탈출
        break;
    }
}
