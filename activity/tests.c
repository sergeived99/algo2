#include "activity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Создаем список активностей
    ActivityList *list = createActivityList(10);
    if (!list) {
        printf("Failed to create list\n");
        return 1;
    }

    // Добавляем активности из примера
    addActivity(list, 1, 4, "A");
    addActivity(list, 3, 5, "B");
    addActivity(list, 0, 6, "C");
    addActivity(list, 5, 7, "D");
    addActivity(list, 3, 9, "E");
    addActivity(list, 8, 11, "H");
    addActivity(list, 12, 16, "K");

    printf("All activities:\n");
    printActivityList(list);
    printf("\n");

    // Выбор активностей
    ActivityList *selected = selectActivities(list);
    if (!selected) {
        printf("Selection failed\n");
        freeActivityList(list);
        return 1;
    }

    printf("Selected activities:\n");
    printActivityList(selected);
    printf("\n");

    // Визуализация
    printf("Timeline:\n");
    printTimeline(list, selected);

    // Проверка демо-режима
    if (argc > 1 && strcmp(argv[1], "--demo") == 0) {
        printf("\nDemo mode: no extra actions.\n");
    }

    freeActivityList(list);
    freeActivityList(selected);
    return 0;
}
