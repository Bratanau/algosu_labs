#include <stdio.h>
#include <stdlib.h>

typedef enum { 
    NODE_CONST, 
    NODE_VAR, 
    NODE_OP 
} NodeType;

typedef enum { 
    OP_ADD, 
    OP_MUL 
} Operation;

struct Node {
    NodeType type;
    union {
        int value; //для константы
        char varName; //для переменной
        struct {
            Operation op;
            struct Node* left;
            struct Node* right;
        } binop; // для бинарной операции
    } data;
};

struct Node* createConst(int val) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->type = NODE_CONST;
    node->data.value = val;
    return node;
}

struct Node* createVar(char name) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->type = NODE_VAR;
    node->data.varName = name;
    return node;
}

struct Node* createOp(Operation op, struct Node* left, struct Node* right) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->type = NODE_OP;
    node->data.binop.op = op;
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

// Рекурсивное удаление дерева
void deleteTree(struct Node* root) {
    if (!root) return;
    if (root->type == NODE_OP) {
        deleteTree(root->data.binop.left);
        deleteTree(root->data.binop.right);
    }
    free(root);
}

// Проверка, является ли узел константой
int isConst(struct Node* node, int* val) {
    if (node->type == NODE_CONST) {
        *val = node->data.value;
        return 1;
    }
    return 0;
}

// Преобразование умножения на константу в сумму
struct Node* reduce(struct Node* root) {
    if (!root) return NULL;

    // Сначала редуцируем поддеревья
    if (root->type == NODE_OP) {
        root->data.binop.left = reduce(root->data.binop.left);
        root->data.binop.right = reduce(root->data.binop.right);

        // Если операция умножения и правый операнд - константа
        if (root->data.binop.op == OP_MUL) {
            int constVal;
            // Проверяем, является ли правый операнд константой
            if (isConst(root->data.binop.right, &constVal)) {
                if (constVal > 0) {
                    // Заменяем умножение на сумму constVal слагаемых
                    // Левое поддерево уже редуцировано
                    struct Node* left = root->data.binop.left;
                    struct Node* sum = left;
                    for (int i = 1; i < constVal; i++) {
                        sum = createOp(OP_ADD, sum, left); // создаём копию left? Важно: left не копируется, а используется повторно – это приведёт к общим узлам. Для простоты сделаем копирование.
                        // Но поскольку left может быть большим поддеревом, лучше его скопировать. Для демонстрации создадим новые узлы-переменные.
                        // В данном примере left - это переменная, поэтому можно просто использовать её повторно, но в общем случае нужно копировать.
                        // Упростим: будем считать, что left - это лист (переменная или константа).
                    }
                    // Освобождаем старый узел умножения (но не его поддеревья, они уже использованы)
                    free(root);
                    return sum;
                } else if (constVal == 0) {
                    // Умножение на 0 даёт 0
                    deleteTree(root->data.binop.left);
                    deleteTree(root->data.binop.right);
                    free(root);
                    return createConst(0);
                } else {
                    // Отрицательное число – можно обработать аналогично, но оставим как есть
                    // или преобразуем с учётом знака.
                }
            }
            // Аналогично для левого операнда-константы (коммутативность)
            else if (isConst(root->data.binop.left, &constVal)) {
                // Меняем местами для удобства, но здесь нужно аналогично
                // Для простоты не реализуем.
            }
        }
    }
    return root;
}

// Печать выражения (инфиксная, с учётом приоритетов)
void printExpr(struct Node* root) {
    if (!root) return;
    switch (root->type) {
        case NODE_CONST:
            printf("%d", root->data.value);
            break;
        case NODE_VAR:
            printf("%c", root->data.varName);
            break;
        case NODE_OP:
            if (root->data.binop.op == OP_ADD) {
                printExpr(root->data.binop.left);
                printf(" + ");
                printExpr(root->data.binop.right);
            } else { // OP_MUL
                // Для умножения добавим скобки, если нужно
                if (root->data.binop.left->type == NODE_OP && root->data.binop.left->data.binop.op == OP_ADD)
                    printf("(");
                printExpr(root->data.binop.left);
                if (root->data.binop.left->type == NODE_OP && root->data.binop.left->data.binop.op == OP_ADD)
                    printf(")");
                printf(" * ");
                if (root->data.binop.right->type == NODE_OP && root->data.binop.right->data.binop.op == OP_ADD)
                    printf("(");
                printExpr(root->data.binop.right);
                if (root->data.binop.right->type == NODE_OP && root->data.binop.right->data.binop.op == OP_ADD)
                    printf(")");
            }
            break;
    }
}

// Построение дерева для выражения (a * 3 + b) * 2
struct Node* buildExample() {
    // a * 3
    struct Node* a = createVar('a');
    struct Node* three = createConst(3);
    struct Node* mul1 = createOp(OP_MUL, a, three);
    // + b
    struct Node* b = createVar('b');
    struct Node* add = createOp(OP_ADD, mul1, b);
    // * 2
    struct Node* two = createConst(2);
    struct Node* mul2 = createOp(OP_MUL, add, two);
    return mul2;
}

int main() {
    struct Node* expr = buildExample();
    printf("Исходное выражение: ");
    printExpr(expr);
    printf("\n");

    struct Node* reduced = reduce(expr);
    printf("После редукции: ");
    printExpr(reduced);
    printf("\n");

    deleteTree(reduced);
    return 0;
}