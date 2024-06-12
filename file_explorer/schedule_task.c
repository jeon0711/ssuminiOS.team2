//schedule_task.c
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_THREADS 3 // 최대 스레드 수

static int running_threads = 0; // 현재 실행 중인 스레드 수
static pthread_mutex_t running_threads_mutex = PTHREAD_MUTEX_INITIALIZER;

void scheduleTask(void *(*task)(void *), void *arg) {
    pthread_mutex_lock(&running_threads_mutex);
    if (running_threads < MAX_THREADS) {
        running_threads++;
        pthread_mutex_unlock(&running_threads_mutex);

        pthread_t thread;
        pthread_create(&thread, NULL, task, arg);
        pthread_detach(thread);
    } else {
        pthread_mutex_unlock(&running_threads_mutex);
        usleep(1000000); // 0.1초 대기
        printf("thread is waiting..\n");
        scheduleTask(task, arg); // 재귀적으로 호출하여 다시 시도
    }
}

