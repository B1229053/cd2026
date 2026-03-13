#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// 定義鏈結串列節點
typedef struct Node {
    char ch;                // 儲存字元
    int count;             // 儲存出現次數
    struct Node* next;     // 指向下一節點的指標
} Node;

// 建立新節點的函式
Node* createNode(char ch) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->ch = ch;
    newNode->count = 1;
    newNode->next = NULL;
    return newNode;
}

// 更新或新增節點到鏈結串列
void updateList(Node** head, char ch) {
    Node* current = *head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->ch == ch) {
            current->count++; 
            return;
        }
        prev = current;
        current = current->next;
    }

    Node* newNode = createNode(ch);
    if (*head == NULL) {
        *head = newNode;
    } else {
        prev->next = newNode;
    }
}

// 輸出 1：純文字版本 (符合 Output -> #, i, n, c, u...)
void printSimpleList(Node* head) {
    Node* current = head;
    printf("Output : ");
    while (current != NULL) {
        // 處理特殊字元顯示
        if (current->ch == '\n') printf("\\n");
        else if (current->ch == '\t') printf("\\t");
        else if (current->ch == ' ') printf("' '");
        else printf("%c", current->ch);

        // 如果不是最後一個節點，就印出逗號
        if (current->next != NULL) {
            printf(" , ");
        }
        current = current->next;
    }
    printf("\n");
}

// 輸出 2：帶有計數與結構的版本 (符合老師的 Link-list 圖示)
void printLinkList(Node* head) {
    Node* current = head;
    printf("Scanner link output:\n");
    while (current != NULL) {
        if (current->ch == '\n') printf("[\\n | %d] -> ", current->count);
        else if (current->ch == '\t') printf("[\\t | %d] -> ", current->count);
        else if (current->ch == ' ') printf("[' ' | %d] -> ", current->count);
        else printf("[%c | %d] -> ", current->ch, current->count);
        
        current = current->next;
    }
    printf("NULL\n");
}

// 釋放記憶體
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE *fp;
    char ch;
    Node* head = NULL;

    fp = fopen("main.c", "r");
    if (fp == NULL) {
        printf("ERROR main.c not found\n");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {
        updateList(&head, ch);
    }
    fclose(fp);

    // 呼叫兩種不同的輸出方式
    printSimpleList(head); // 第一種：純字元清單
    printf("\n");          // 換行隔開
    printLinkList(head);   // 第二種：完整鏈結串列結構

    freeList(head);
    return 0;
}
