// schedule_task.h
#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

void scheduleTask(void *(*task)(void *), void *arg);

void *listDirectoryTask(void* dirPath);
void listDirectory();
void *executeFileTask(void *filepath);
void executeFile();

void *createFolderTask(void *arg);
void createFolder();
void *deleteFolderTask(void *arg);
void deleteFolder();
void *copyFileTask(void *arg);
void copyFile();
#endif // SCHEDULE_TASK_H
