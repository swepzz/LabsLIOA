#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma execution_character_set("utf-8")

struct Node
{
    int data;
    Node* left;
    Node* right;
};

Node* createNode(int data)
{
    Node* node = (Node*)malloc(sizeof(Node));

    if (node == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        exit(1);
    }

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insertNode(Node* root, int data)
{
    if (root == NULL)
    {
        return createNode(data);
    }

    if (data < root->data)
    {
        root->left = insertNode(root->left, data);
    }
    else
    {
        root->right = insertNode(root->right, data);
    }

    return root;
}

void printTree(const Node* root, int level)
{
    if (root == NULL)
    {
        return;
    }

    printTree(root->right, level + 1);

    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    printf("%d\n", root->data);
    printTree(root->left, level + 1);
}

const Node* searchValue(const Node* root, int value)
{
    if (root == NULL || root->data == value)
    {
        return root;
    }

    if (value < root->data)
    {
        return searchValue(root->left, value);
    }

    return searchValue(root->right, value);
}

void deleteTree(Node* root)
{
    if (root == NULL)
    {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Node* root = NULL;
    int value;

    printf("Введите целые числа для построения дерева.\n");
    printf("Для завершения ввода введите -1.\n\n");

    while (true)
    {
        printf("Введите число: ");

        if (scanf_s("%d", &value) != 1)
        {
            printf("Ошибка ввода.\n");
            deleteTree(root);
            return 1;
        }

        if (value == -1)
        {
            break;
        }

        root = insertNode(root, value);
    }

    if (root == NULL)
    {
        printf("Дерево пусто.\n");
        return 0;
    }

    printf("\nПостроенное дерево:\n");
    printTree(root, 0);

    printf("\nВведите значение для поиска: ");

    if (scanf_s("%d", &value) != 1)
    {
        printf("Ошибка ввода.\n");
        deleteTree(root);
        return 1;
    }

    if (searchValue(root, value) != NULL)
    {
        printf("Значение %d найдено в дереве.\n", value);
    }
    else
    {
        printf("Значение %d не найдено в дереве.\n", value);
    }

    deleteTree(root);
    return 0;
}
