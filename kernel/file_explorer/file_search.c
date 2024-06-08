#ifndef BASIC_INCLUDE
#define BASIC_INCLUDE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule_task.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd

#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define GetCurrentDir getcwd
#endif

#define NAMEMAX 255
#define PATHMAX 4096
#define STRMAX 4096
#endif

typedef struct fileLinkedList {
	char path[PATHMAX];
	struct fileLinkedList* prev;
	struct fileLinkedList* next;

} fileLinkedList;
static fileLinkedList* head = NULL;
static fileLinkedList* tail = NULL;

void addLink(char* path) {

	fileLinkedList* rt = (fileLinkedList*)malloc(sizeof(fileLinkedList));
	strcpy(rt->path, path);
	rt->next = NULL;

	if (head == NULL) {
		head = rt;
		tail = rt;
		rt->prev = NULL;
	}
	else {
		rt->prev = tail;
		tail->next = rt;
		tail = rt;
	}
}

void printList() {
	fileLinkedList* temp = head;

	if (temp == NULL) {
		fprintf(stdout, "file not founded \n");
	}

	while (temp != NULL) {
		fprintf(stdout, "%s \n", temp->path);
		temp = temp->next;
	}

	// Clear the linked list after printing
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
	tail = NULL;
}

void *filesearchTask(void *arg) {
    fileLinkedList *args = (fileLinkedList *)arg;
    char *path = args->path;
    char *target = args->next->path; // target을 다음 노드의 path로 설정
    char pathbuf[PATHMAX];
    char pathbuf2[PATHMAX];

    strcpy(pathbuf, path);

#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPattern[PATHMAX];
    sprintf(searchPattern, "%s/*", pathbuf);
    hFind = FindFirstFile(searchPattern, &findFileData);

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }
        sprintf(pathbuf2, "%s/%s", pathbuf, findFileData.cFileName);
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            fileLinkedList *newArgs = (fileLinkedList *)malloc(sizeof(fileLinkedList));
            strcpy(newArgs->path, pathbuf2);
            newArgs->next = (fileLinkedList *)malloc(sizeof(fileLinkedList));
            strcpy(newArgs->next->path, target);
            filesearchTask(newArgs);
        } else {
            if (strcmp(findFileData.cFileName, target) == 0) {
                addLink(pathbuf2);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);

#else
    struct dirent **list;
    struct stat statbuf;
    int cnt = scandir(pathbuf, &list, NULL, alphasort);

    for (int i = 0; i < cnt; i++) {
        if (strcmp(list[i]->d_name, ".") == 0 || strcmp(list[i]->d_name, "..") == 0) {
            free(list[i]);
            continue;
        }
        sprintf(pathbuf2, "%s/%s", pathbuf, list[i]->d_name);
        if (lstat(pathbuf2, &statbuf) != 0) {
            fprintf(stderr, "lstat err \n");
            exit(1);
        }
        if (S_ISREG(statbuf.st_mode)) {
            if (strcmp(list[i]->d_name, target) == 0) {
                addLink(pathbuf2);
            }
        } else if (S_ISDIR(statbuf.st_mode)) {
            fileLinkedList *newArgs = (fileLinkedList *)malloc(sizeof(fileLinkedList));
            strcpy(newArgs->path, pathbuf2);
            newArgs->next = (fileLinkedList *)malloc(sizeof(fileLinkedList));
            strcpy(newArgs->next->path, target);
            filesearchTask(newArgs);
        }
        free(list[i]);
    }
    free(list);
#endif

    if (path == NULL) {
        printList();
    }

    free(args->next); // 여기에서 메모리를 해제합니다.
    free(args); // 여기에서 메모리를 해제합니다.
    return NULL;
}


void filesearch(char *path, char *target) {
    char filename[NAMEMAX];
    char pathbuf[PATHMAX];

    if (path == NULL && target == NULL) {
        fprintf(stdout, "input filename: ");
        fgets(filename, NAMEMAX, stdin);
        filename[strcspn(filename, "\n")] = 0;  // 개행 문자 제거
#ifdef _WIN32
        strcpy(pathbuf, getenv("USERPROFILE"));
#else
        strcpy(pathbuf, getenv("HOME"));
#endif
    } else {
        strcpy(pathbuf, path);
        strcpy(filename, target);
    }

    fileLinkedList *args = (fileLinkedList *)malloc(sizeof(fileLinkedList));
    strcpy(args->path, pathbuf);
    args->next = (fileLinkedList *)malloc(sizeof(fileLinkedList));
    strcpy(args->next->path, filename);
    scheduleTask(filesearchTask, args);
}


#ifdef _WIN32
int search_main(int argc,char* argv)
{
	filesearch(NULL, NULL);
}

#endif
