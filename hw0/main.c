#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char ch;
    int count;
    struct Node *next;
};

// 建立一個全域的指標陣列作為 Hash Table，大小為 256 對應所有 ASCII 字元
// 初始值全部設為 NULL
struct Node *hashTable[256] = {NULL};

struct Node *updateList(struct Node *head, char c)
{
    // 將字元轉為無號整數，當作陣列的 Index
    unsigned char index = (unsigned char)c;

    // O(1) 搜尋：直接透過陣列查看該字元的節點是否已經建立過
    if (hashTable[index] != NULL)
    {
        // 如果不為空，代表之前已經把節點位址存進來了，直接去那個位址加 1 即可
        hashTable[index]->count++;
        return head; // 串列開頭沒有改變
    }

    // 如果為空，代表是第一次遇到這個字元，建立新節點
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->ch = c;
    newNode->count = 1;

    // 將新節點接到 Link-list 的最前面
    newNode->next = head;

    // 【關鍵步驟】將這個新節點的記憶體位址，存入對應的 Hash Table 格子中
    hashTable[index] = newNode;

    return newNode; // 回傳新的串列開頭
}

int main()
{
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL)
    {
        printf("Cannot open file main.c\n");
        return 1;
    }

    struct Node *head = NULL;
    char c;

    // 逐字讀取直到檔案結束 (EOF)
    while ((c = fgetc(fp)) != EOF)
    {
        head = updateList(head, c);
    }
    fclose(fp);

    // 輸出統計結果 (走訪 Link-list)
    struct Node *current = head;
    printf("--- HW0 Bonus: O(1) Character Statistics ---\n");
    while (current != NULL)
    {
        if (current->ch == '\n')
            printf("'\\n' : %d\n", current->count);
        else if (current->ch == ' ')
            printf("'Space' : %d\n", current->count);
        else if (current->ch == '\r')
            printf("'\\r' : %d\n", current->count);
        else if (current->ch == '\t')
            printf("'\\t' : %d\n", current->count);
        else
            printf("'%c' : %d\n", current->ch, current->count);

        current = current->next;
    }
    return 0;
}
