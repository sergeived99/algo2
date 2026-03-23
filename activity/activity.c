#include "activity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ActivityList *createActivityList(int capacity)
{
    ActivityList *list = (ActivityList*)malloc(sizeof(ActivityList));
    if (!list) return NULL;
    list->items = (Activity*)malloc(capacity * sizeof(Activity));
    if (!list->items) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;
}

int addActivity(ActivityList *list, int start, int end, const char *name)
{
    if (!list || list->size >= list->capacity) return 0;
    Activity *act = &list->items[list->size];
    act->start = start;
    act->end = end;
    strncpy(act->name, name, sizeof(act->name) - 1);
    act->name[sizeof(act->name)-1] = '\0';
    list->size++;
    return 1;
}

void freeActivityList(ActivityList *list)
{
    if (!list) return;
    free(list->items);
    free(list);
}

void printActivityList(ActivityList *list)
{
    if (!list) return;
    for (int i = 0; i < list->size; i++) {
        Activity *a = &list->items[i];
        printf("%s: [%d, %d]\n", a->name, a->start, a->end);
    }
}

static int compareByEndTime(const void *a, const void *b)
{
    const Activity *aa = (const Activity*)a;
    const Activity *bb = (const Activity*)b;
    return aa->end - bb->end;
}

void sortByEndTime(ActivityList *list)
{
    if (!list || list->size <= 1) return;
    qsort(list->items, list->size, sizeof(Activity), compareByEndTime);
}

ActivityList *selectActivities(ActivityList *activities)
{
    if (!activities || activities->size == 0) return NULL;

    ActivityList *sorted = createActivityList(activities->size);
    if (!sorted) return NULL;
    for (int i = 0; i < activities->size; i++) {
        Activity *src = &activities->items[i];
        if (!addActivity(sorted, src->start, src->end, src->name)) {
            freeActivityList(sorted);
            return NULL;
        }
    }
    sortByEndTime(sorted);

    ActivityList *selected = createActivityList(activities->size);
    if (!selected) {
        freeActivityList(sorted);
        return NULL;
    }

    int last_end = -1;
    for (int i = 0; i < sorted->size; i++) {
        Activity *a = &sorted->items[i];
        if (a->start >= last_end) {
            addActivity(selected, a->start, a->end, a->name);
            last_end = a->end;
        }
    }

    freeActivityList(sorted);
    return selected;
}

void printTimeline(ActivityList *all, ActivityList *selected)
{
    if (!all || all->size == 0) return;

    int min_time = all->items[0].start;
    int max_time = all->items[0].end;
    for (int i = 1; i < all->size; i++) {
        if (all->items[i].start < min_time) min_time = all->items[i].start;
        if (all->items[i].end > max_time) max_time = all->items[i].end;
    }

    for (int i = 0; i < all->size; i++) {
        Activity *a = &all->items[i];
        int is_selected = 0;
        if (selected) {
            for (int j = 0; j < selected->size; j++) {
                if (selected->items[j].start == a->start && selected->items[j].end == a->end &&
                    strcmp(selected->items[j].name, a->name) == 0) {
                    is_selected = 1;
                    break;
                }
            }
        }

        if (is_selected) printf("[*] ");
        else printf("    ");

        printf("%s ", a->name);

        int offset = a->start - min_time;
        for (int k = 0; k < offset; k++) printf(" ");

        int len = a->end - a->start;
        for (int k = 0; k < len; k++) printf("#");
        printf("\n");
    }
}
