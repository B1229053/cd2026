#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. 修改 Node 結構：現在要存「單字」和「標籤」
struct Node
{
    char word[100];     // 儲存切出來的字串
    char tokenType[50]; // 儲存對應的 Token 標籤
    int count;          // 出現次數 (保留 HW0 邏輯)
    struct Node *next;
};

// 2. 貼標籤的字典函數 (依照講義 P.90 要求)
void getTokenType(char *word, char *tokenType)
{
    if (strcmp(word, "int") == 0)
        strcpy(tokenType, "TYPE_TOKEN");
    else if (strcmp(word, "main") == 0)
        strcpy(tokenType, "MAIN_TOKEN");
    else if (strcmp(word, "if") == 0)
        strcpy(tokenType, "IF_TOKEN");
    else if (strcmp(word, "else") == 0)
        strcpy(tokenType, "ELSE_TOKEN");
    else if (strcmp(word, "while") == 0)
        strcpy(tokenType, "WHILE_TOKEN");
    else if (strcmp(word, "(") == 0)
        strcpy(tokenType, "LEFTPAREN_TOKEN");
    else if (strcmp(word, ")") == 0)
        strcpy(tokenType, "RIGHTPAREN_TOKEN");
    else if (strcmp(word, "{") == 0)
        strcpy(tokenType, "LEFTBRACE_TOKEN");
    else if (strcmp(word, "}") == 0)
        strcpy(tokenType, "RIGHTBRACE_TOKEN");
    else if (strcmp(word, "==") == 0)
        strcpy(tokenType, "EQUAL_TOKEN");
    else if (strcmp(word, ">=") == 0)
        strcpy(tokenType, "GREATEREQUAL_TOKEN");
    // 以下為測試檔中出現的其他符號與變數，我們也給它基本的標籤
    else if (strcmp(word, "=") == 0)
        strcpy(tokenType, "ASSIGN_TOKEN");
    else if (strcmp(word, "+") == 0)
        strcpy(tokenType, "PLUS_TOKEN");
    else if (strcmp(word, ";") == 0)
        strcpy(tokenType, "SEMICOLON_TOKEN");
    else if (word[0] >= '0' && word[0] <= '9')
        strcpy(tokenType, "NUM_TOKEN");
    else
        strcpy(tokenType, "ID_TOKEN"); // 變數名稱如 cd2026 歸類為 ID
}

// 3. 更新 Link-list (改成比對字串)
struct Node *updateList(struct Node *head, char *w)
{
    struct Node *current = head;

    // 檢查字串是否已存在
    while (current != NULL)
    {
        if (strcmp(current->word, w) == 0)
        {
            current->count++;
            return head;
        }
        current = current->next;
    }

    // 如果不存在，建立新節點
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    strcpy(newNode->word, w);
    getTokenType(w, newNode->tokenType); // 呼叫函數貼標籤
    newNode->count = 1;
    newNode->next = NULL;

    // 為了讓輸出的順序跟檔案出現的順序一樣，我們把新節點接在「最後面」
    if (head == NULL)
    {
        return newNode;
    }
    else
    {
        struct Node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
        return head;
    }
}

// 4. 主程式：Scanner 掃描邏輯
int main()
{
    FILE *fp = fopen("test.c", "r");
    if (fp == NULL)
    {
        printf("Cannot open test.c\n");
        return 1;
    }

    struct Node *head = NULL;
    char c = fgetc(fp);
    char word[100];
    int len;

    while (c != EOF)
    {
        // [規則 A] 遇到空白、換行直接跳過
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
        {
            c = fgetc(fp);
            continue;
        }

        len = 0;
        // [規則 B] 如果是英文字母或底線 (變數或關鍵字)
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        {
            // 一直往下讀，直到遇到不是字母或數字為止
            while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            {
                word[len++] = c;
                c = fgetc(fp);
            }
            word[len] = '\0'; // 字串結尾
            head = updateList(head, word);
        }
        // [規則 C] 如果是數字
        else if (c >= '0' && c <= '9')
        {
            while (c >= '0' && c <= '9')
            {
                word[len++] = c;
                c = fgetc(fp);
            }
            word[len] = '\0';
            head = updateList(head, word);
        }
        // [規則 D] 如果是符號
        else
        {
            word[len++] = c;
            // 特別處理 == 和 >= 這種兩個字元的符號
            if (c == '=' || c == '>')
            {
                char next_c = fgetc(fp);
                if (next_c == '=')
                {
                    word[len++] = next_c;
                    c = fgetc(fp); // 把第二個 '=' 吃掉
                }
                else
                {
                    c = next_c; // 不是 '=' 就保留給下一輪處理
                }
            }
            else
            {
                c = fgetc(fp);
            }
            word[len] = '\0';
            head = updateList(head, word);
        }
    }
    fclose(fp);

    // 5. 印出結果 (依照講義要求的 value: Token 格式)
    printf("--- HW1 Scanner Output ---\n");
    struct Node *current = head;
    while (current != NULL)
    {
        // 只印出我們有明確定義 Token 標籤的內容
        printf("%s: %s\n", current->word, current->tokenType);
        current = current->next;
    }

    return 0;
}
