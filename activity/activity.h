#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <stddef.h>

typedef struct {
    int start;
    int end;
    char name[64];
} Activity;

typedef struct {
    Activity *items;
    int size;
    int capacity;
} ActivityList;


ActivityList *createActivityList(int capacity);

int addActivity(ActivityList *list, int start, int end, const char *name);

void freeActivityList(ActivityList *list);

void printActivityList(ActivityList *list);

void sortByEndTime(ActivityList *list);

ActivityList *selectActivities(ActivityList *activities);

void printTimeline(ActivityList *all, ActivityList *selected);

#endif // ACTIVITY_H
