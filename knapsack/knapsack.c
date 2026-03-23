#include "knapsack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

ItemList *createItemList(int capacity) {
    ItemList *list = (ItemList *)malloc(sizeof(ItemList));
    if (!list) return NULL;
    list->items = (Item *)malloc(sizeof(Item) * capacity);
    if (!list->items) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;
}

int addItem(ItemList *list, double weight, double value, const char *name) {
    if (!list || list->size >= list->capacity) return 0;
    Item *item = &list->items[list->size];
    item->weight = weight;
    item->value = value;
    strncpy(item->name, name, sizeof(item->name) - 1);
    item->name[sizeof(item->name) - 1] = '\0';
    list->size++;
    return 1;
}

void freeItemList(ItemList *list) {
    if (list) {
        free(list->items);
        free(list);
    }
}

void printItemList(ItemList *list) {
    if (!list) return;
    printf("Item list:\n");
    for (int i = 0; i < list->size; i++) {
        Item *item = &list->items[i];
        printf("  %s: weight = %.2f, value = %.2f\n",
               item->name, item->weight, item->value);
    }
}

double getValuePerWeight(Item *item) {
    if (item->weight == 0) return 0.0;
    return item->value / item->weight;
}

static int compareByValuePerWeight(const void *a, const void *b) {
    Item *ia = (Item *)a;
    Item *ib = (Item *)b;
    double va = getValuePerWeight(ia);
    double vb = getValuePerWeight(ib);
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

void sortByValuePerWeight(ItemList *list) {
    if (!list || list->size <= 1) return;
    qsort(list->items, list->size, sizeof(Item), compareByValuePerWeight);
}

KnapsackResult *fractionalKnapsack(ItemList *items, double capacity) {
    if (!items || capacity <= 0) {
        KnapsackResult *res = (KnapsackResult *)malloc(sizeof(KnapsackResult));
        if (res) {
            res->items = NULL;
            res->size = 0;
            res->totalValue = 0.0;
            res->totalWeight = 0.0;
        }
        return res;
    }

    sortByValuePerWeight(items);

    TakenItem *taken = (TakenItem *)malloc(sizeof(TakenItem) * items->size);
    int takenCount = 0;
    double remaining = capacity;
    double totalValue = 0.0;
    double totalWeight = 0.0;

    for (int i = 0; i < items->size && remaining > 0; i++) {
        Item *item = &items->items[i];
        if (item->weight <= remaining) {
            // Берём предмет целиком
            strcpy(taken[takenCount].name, item->name);
            taken[takenCount].fraction = 1.0;
            taken[takenCount].weight = item->weight;
            taken[takenCount].value = item->value;
            totalValue += item->value;
            totalWeight += item->weight;
            remaining -= item->weight;
            takenCount++;
        } else {
            // Берём часть предмета
            double fraction = remaining / item->weight;
            double partWeight = remaining;
            double partValue = item->value * fraction;
            strcpy(taken[takenCount].name, item->name);
            taken[takenCount].fraction = fraction;
            taken[takenCount].weight = partWeight;
            taken[takenCount].value = partValue;
            totalValue += partValue;
            totalWeight += partWeight;
            remaining = 0.0;
            takenCount++;
        }
    }

    KnapsackResult *result = (KnapsackResult *)malloc(sizeof(KnapsackResult));
    if (!result) {
        free(taken);
        return NULL;
    }
    result->items = (TakenItem *)malloc(sizeof(TakenItem) * takenCount);
    if (!result->items) {
        free(result);
        free(taken);
        return NULL;
    }
    memcpy(result->items, taken, sizeof(TakenItem) * takenCount);
    result->size = takenCount;
    result->totalValue = totalValue;
    result->totalWeight = totalWeight;

    free(taken);
    return result;
}

static double knapsack01Recursive(Item *items, int n, double capacity, int idx) {
    if (idx == n || capacity <= 0) return 0.0;

    double without = knapsack01Recursive(items, n, capacity, idx + 1);

    double with = 0.0;
    if (items[idx].weight <= capacity) {
        with = items[idx].value + knapsack01Recursive(items, n,
                                                      capacity - items[idx].weight,
                                                      idx + 1);
    }

    return (with > without) ? with : without;
}

double knapsack01Naive(ItemList *items, double capacity) {
    if (!items || items->size == 0 || capacity <= 0) return 0.0;
    return knapsack01Recursive(items->items, items->size, capacity, 0);
}

void printKnapsackResult(KnapsackResult *result) {
    if (!result) {
        printf("No result.\n");
        return;
    }
    printf("Knapsack result:\n");
    printf("  Total value: %.2f, Total weight: %.2f\n",
           result->totalValue, result->totalWeight);
    printf("  Taken items:\n");
    for (int i = 0; i < result->size; i++) {
        TakenItem *t = &result->items[i];
        printf("    %s: fraction = %.2f (weight = %.2f, value = %.2f)\n",
               t->name, t->fraction, t->weight, t->value);
    }
}

void freeKnapsackResult(KnapsackResult *result) {
    if (result) {
        free(result->items);
        free(result);
    }
}
