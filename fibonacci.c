#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    enum TypeTag {
        ADD,
        MUL,
        SUB,
        CONST
    } type;
    union {
        struct {
            struct Node *left;
            struct Node *right;
        } op;
        int value;
    } data;
} Node;

Node* makeFunc(enum TypeTag type) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->type = type;
    return node;
}

Node* makeConst(int value) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->type = CONST;
    node->data.value = value;
    return node;
}

int calc(Node *node) {
    if (node == NULL)
        return 0;

    switch (node->type) {
        case ADD:
            return calc(node->data.op.left) + calc(node->data.op.right);
        case MUL:
            return calc(node->data.op.left) * calc(node->data.op.right);
        case SUB:
            return calc(node->data.op.left) - calc(node->data.op.right);
        case CONST:
            return node->data.value;
        default:
            return 0;
    }
}

Node* fibonacci(int n) {
    if (n < 2)
        return makeConst(n);

    Node **fib = (Node**)malloc((n + 1) * sizeof(Node*));

    fib[0] = makeConst(0);
    fib[1] = makeConst(1);

    for (int i = 2; i <= n; i++) {
        fib[i] = makeFunc(ADD);
        fib[i]->data.op.left = fib[i - 1];
        fib[i]->data.op.right = fib[i - 2];
    }

    Node *result = fib[n];
    free(fib);
    return result;
}

void freeNode(Node *node) {
    if (node == NULL)
        return;

    if (node->type == ADD || node->type == MUL || node->type == SUB) {
        freeNode(node->data.op.left);
        freeNode(node->data.op.right);
    }

    free(node);
}

int main() {
    Node *add = makeFunc(ADD);
    add->data.op.left = makeConst(10);
    add->data.op.right = makeConst(6);

    Node *mul = makeFunc(MUL);
    mul->data.op.left = makeConst(5);
    mul->data.op.right = makeConst(4);

    Node *sub = makeFunc(SUB);
    sub->data.op.left = mul;
    sub->data.op.right = add;

    int subResult = calc(sub);
    Node *fibo = fibonacci(abs(subResult) - 1);

    printf("add : %d\n", calc(add));
    printf("mul : %d\n", calc(mul));
    printf("sub : %d\n", -subResult);
    printf("fibo : %d\n", calc(fibo));

    freeNode(add);
    freeNode(mul);
    freeNode(sub);
    freeNode(fibo);

    return 0;
}
