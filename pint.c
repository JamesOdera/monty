#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024

typedef struct {
    int stack[STACK_SIZE];
    int top;
} Stack;

void push(Stack *stack, int value, int line_number) {
    if (stack->top == STACK_SIZE) {
        fprintf(stderr, "Error: Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->stack[stack->top++] = value;
}

void pint(Stack *stack, int line_number) {
    if (stack->top == 0) {
        fprintf(stderr, "L%d: can't pint, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }
    printf("%d\n", stack->stack[stack->top - 1]);
}

void pall(Stack *stack) {
    int i;
    for (i = stack->top - 1; i >= 0; i--) {
        printf("%d\n", stack->stack[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error");
        return EXIT_FAILURE;
    }
    
    char line[256];
    Stack stack;
    stack.top = 0;
    
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        char *token = strtok(line, " \n");
        if (token == NULL)
            continue;
        
        if (strcmp(token, "push") == 0) {
            token = strtok(NULL, " \n");
            if (token == NULL || !atoi(token)) {
                fprintf(stderr, "L%d: usage: push integer\n", line_number);
                return EXIT_FAILURE;
            }
            push(&stack, atoi(token), line_number);
        } else if (strcmp(token, "pall") == 0) {
            pall(&stack);
        } else if (strcmp(token, "pint") == 0) {
            pint(&stack, line_number);
        } else {
            fprintf(stderr, "L%d: Unknown opcode: %s\n", line_number, token);
            return EXIT_FAILURE;
        }
    }
    
    fclose(file);
    return EXIT_SUCCESS;
}

