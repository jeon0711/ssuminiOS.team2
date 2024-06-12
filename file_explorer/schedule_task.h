// schedule_task.h
#ifndef SCHEDULE_TASK_H
#define SCHEDULE_TASK_H

void scheduleTask(void *(*task)(void *), void *arg);

#endif // SCHEDULE_TASK_H
