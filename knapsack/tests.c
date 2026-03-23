#include "knapsack.h"
#include <stdio.h>

int main() {
    printf("=== Дробный рюкзак (пример из условия) ===\n");
    ItemList *items1 = createItemList(3);
    addItem(items1, 10.0, 600.0, "Золото");
    addItem(items1, 20.0, 400.0, "Серебро");
    addItem(items1, 30.0, 300.0, "Бронза");

    printItemList(items1);

    double capacity = 50.0;
    KnapsackResult *res1 = fractionalKnapsack(items1, capacity);
    printKnapsackResult(res1);
    freeKnapsackResult(res1);
    freeItemList(items1);

    printf("\n=== Сравнение с 0/1 рюкзаком (жадный НЕ оптимален) ===\n");
    ItemList *items2 = createItemList(3);
    addItem(items2, 10.0, 60.0, "A");
    addItem(items2, 20.0, 100.0, "B");
    addItem(items2, 30.0, 120.0, "C");

    printItemList(items2);
    capacity = 50.0;

    // Дробный рюкзак (жадный)
    KnapsackResult *res2 = fractionalKnapsack(items2, capacity);
    printf("\n--- Жадный алгоритм (дробный) ---\n");
    printKnapsackResult(res2);

    // 0/1 рюкзак (наивный перебор)
    double optimal01 = knapsack01Naive(items2, capacity);
    printf("\n--- Оптимальное решение для 0/1 рюкзака ---\n");
    printf("  Максимальная стоимость: %.2f\n", optimal01);

    // (жадный взял бы A и B: 60+100=160, а оптимальный B+C=220)
    printf("\nЖадный алгоритм для 0/1 взял бы A и B (30 кг, стоимость 160),\n");
    printf("а оптимальное решение — B и C (50 кг, стоимость 220).\n");
    printf("Таким образом, жадный НЕ работает для 0/1 варианта.\n");

    freeKnapsackResult(res2);
    freeItemList(items2);

    return 0;
}
