#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  
#include <time.h>


typedef struct Stack 
{
    void* data;
    struct Stack* next_ptr;
}Stack ;

Stack* create_stack(void);
Stack* top(Stack*);
Stack* pop(Stack*);
void push(Stack*, void*);
void free_stack_mem(Stack*);
double factorial(int);
int is_operand(char);
void use_operand(char, Stack*, Stack*, int);
void parse_string(char*);
void print_data(Stack*, unsigned char);


int main(int argc, char* argv[])
{

    long double start, end = 0;
    unsigned char minus_state = 1;
    char* input_string = (char*)malloc(sizeof(char*) * 4076);
    if (argc < 2) {
        printf("Solving problem - invalid expression, type new one: \n");
        input_string = (char*)malloc(sizeof(char*) * 4076);
        int temp = scanf_s("%s", input_string, (unsigned int)strlen(input_string));
    }
    else {
        int string_size = 0;

        for (int i = 1; i < argc; i++) string_size += strlen(argv[i]);

        input_string = (char*)malloc(sizeof(char*) * 4076);
        if (input_string == NULL) return -1;

        for (int i = 1; i < argc; i++) {
            strncpy_s(input_string, strlen(input_string) * sizeof(char), argv[i], strlen(input_string));
        }

    }
    start = clock();
    parse_string(input_string);
    Stack* numbers_array = create_stack();
    Stack* operands = create_stack();

    unsigned char long_value_flag = 0;
    if (input_string != NULL) {
        for (unsigned int i = 0; i < strlen(input_string) + 1; i++) {
            if (isdigit(input_string[i]) || (input_string[i] == '-' && minus_state)) {
                long double* number_double = (long double*)malloc(sizeof(long double));
                unsigned long long int* number_ulli = (unsigned long long int*)malloc(sizeof(unsigned long long int));
                char* number_end;
                if (number_double && number_ulli) {
                    *number_ulli = strtoull(input_string + i, &number_end, 10);
                    if ((*number_ulli >= LONG_MAX || long_value_flag == 1) && strtold(input_string + i, &number_end) > 0) {
                        long_value_flag = 1;
                        i += number_end - (input_string + i) - 1;
                        push(numbers_array, number_ulli);
                        minus_state = 0;
                    }
                    else {
                        *number_double = strtold(input_string + i, &number_end);
                        i += number_end - (input_string + i) - 1;
                        push(numbers_array, number_double);  
                        minus_state = 0;
                    }
                }
            }
            else if (input_string[i] == '\0' || is_operand(input_string[i])) {
                if (input_string[i] == '(') {
                    minus_state = 1;
                }
                if (input_string[i] == '\0') {
                    while (operands->data != NULL)
                        use_operand(input_string[i], operands, numbers_array, long_value_flag);
                }
                else {
                    use_operand(input_string[i], operands, numbers_array, long_value_flag);
                }
            }
        }
    }

    print_data(numbers_array, long_value_flag);

    end = clock();
    printf("Time is %lf seconds", (long double)((end - start) / (CLOCKS_PER_SEC)));

    return 0;
}

void print_data(Stack* numbers_array, unsigned char long_value_flag)
{
    if (long_value_flag == 1) {
        printf("%llu\n", *((unsigned long long int*)(pop(numbers_array)->data)));
    }
    else {
        printf("%f\n", *((long double*)(pop(numbers_array)->data)));
    }
}

Stack* create_stack(void)
{
    Stack* newStack = (Stack*)malloc(sizeof(Stack) * 1);
    if (newStack) {
        newStack->data = NULL;
        newStack->next_ptr = NULL;
    }
    return newStack;
}

Stack* top(Stack* stack) 
{
    while (stack->next_ptr != NULL)
        stack = stack->next_ptr;

    return stack;
}

Stack* pop(Stack* stack)
{
    Stack* next_ptr = stack->next_ptr;
    if (next_ptr == NULL) {
        Stack* shadow_copy = (Stack*)malloc(sizeof(Stack));
        if (shadow_copy)
            memcpy(shadow_copy, stack, sizeof(Stack));
        stack->data = NULL;
        stack->next_ptr = NULL;
        return shadow_copy;
    }
    while (next_ptr->next_ptr != NULL) {
        stack = next_ptr;
        next_ptr = stack->next_ptr;
    }

    stack->next_ptr = NULL;
    return next_ptr;
}

void push(Stack* stack, void* data)
{
    if (stack->data == NULL) {
        stack->data = data;
        return;
    }

    if (stack->next_ptr != NULL)
        stack = top(stack);

    Stack* new_stack_elem = create_stack();
    new_stack_elem->data = data;
    stack->next_ptr = new_stack_elem;
    return;
}

void free_stack_mem(Stack* stack) 
{
    if (stack == NULL)
        return;

    if (stack->data != NULL) {
        free(stack->data);
        return;
    }

    free(stack->data);
    return;
}

double factorial(int number)
{
    double total = number;
    while (--number)
        total *= number;
    return total;
}

int is_operand(char character)
{
    return character == '+' || character == '-' || character == '*' || character == '/' || character == '^' || character == '(' || character == ')' ||
        character == '!' || character == '%';
}

int function_comparhison(char previous_char, char next_char) {
    char level[256];
    level['+'] = 1;
    level['-'] = 1;
    level['*'] = 2;
    level['/'] = 2;
    level['!'] = 3;
    level['^'] = 3;
    level['%'] = 3;

    return level[(int)previous_char] - level[(int)next_char];
}

void free_stack_memall(Stack* first_element, Stack* second_element, Stack* previous_element)
{
    free_stack_mem(first_element);
    free_stack_mem(second_element);
    free_stack_mem(previous_element);
}

void use_operand(char current_character, Stack* operands, Stack* numbers_array, int long_value_flag)
{
    char* new_operand = malloc(sizeof(char) * 1);
    if (new_operand) {
        new_operand[0] = current_character;
    }
    if (current_character == '(') {
        push(operands, new_operand);
        return;
    }
    Stack* previous_element = top(operands);
    if (previous_element->data == NULL) {
        push(operands, new_operand);
        return;
    }
    char previous_operand = *((char*)(previous_element->data));
    if (previous_operand == '(') {
        if (current_character == ')') {
            previous_element = pop(operands);
            free_stack_mem(previous_element);
            return;
        }
        push(operands, new_operand);
        return;
    }
    if (current_character != ')' && current_character != '\0' && function_comparhison(current_character, previous_operand) > 0) {
        push(operands, new_operand);
        return;
    }
    previous_element = pop(operands);
    Stack* first_elem;
    Stack* second_elem;
    long double first_num = 0;
    long double second_num = 0;
    long double* total = (long double*)malloc(sizeof(long double));
    unsigned long long int first_temp = 0;
    unsigned long long int second_temp = 0;
    switch (previous_operand) {
    case '+':
        first_elem = pop(numbers_array);
        second_elem = pop(numbers_array);
        if (long_value_flag == 1) {
            first_temp = *((unsigned long long int*)(first_elem->data));
            second_temp = *((unsigned long long int*)(second_elem->data));
        }
        else {
            first_num = *((long double*)(first_elem->data));
            second_num = *((long double*)(second_elem->data));
        }

        if (total && long_value_flag == 1) {
            unsigned long long int* total_temp = (unsigned long long int*)malloc(sizeof(unsigned long long int));
            if (total_temp)
                *total_temp = (second_temp + first_temp);
            push(numbers_array, total_temp);
        }
        else {
            if (total)
                *total = second_num + first_num;
            push(numbers_array, total);
        }
        free_stack_memall(first_elem, second_elem, previous_element);

        break;
    case '-':
        first_elem = pop(numbers_array);
        second_elem = pop(numbers_array);

        if (long_value_flag == 1) {
            first_temp = *((unsigned long long int*)(first_elem->data));
            second_temp = *((unsigned long long int*)(second_elem->data));
            long_value_flag = 1;
        }
        else {
            first_num = *((long double*)(first_elem->data));
            second_num = *((long double*)(second_elem->data));
        }

        if (total && long_value_flag == 1) {
            unsigned long long int* total_temp = (unsigned long long int*)malloc(sizeof(unsigned long long int*));
            if (total_temp)
                *total_temp = (second_temp - first_temp);
            push(numbers_array, total_temp);
        }
        else {
            if (total)
                *total = second_num - first_num;
            push(numbers_array, total);
        }
        free_stack_memall(first_elem, second_elem, previous_element);

        break;
    case '*':
        first_elem = pop(numbers_array);
        second_elem = pop(numbers_array);
        first_num = *((long double*)(first_elem->data));
        second_num = *((long double*)(second_elem->data));
        if (total)
            *total = second_num * first_num;
        push(numbers_array, total);

        free_stack_memall(first_elem, second_elem, previous_element);

        break;
    case '/':
        first_elem = pop(numbers_array);
        second_elem = pop(numbers_array);
        if (long_value_flag == 1) {
            first_temp = *((unsigned long long int*)(first_elem->data));
            second_temp = *((unsigned long long int*)(second_elem->data));
            long_value_flag = 1;
        }
        else {
            first_num = *((long double*)(first_elem->data));
            second_num = *((long double*)(second_elem->data));
        }

        
        if (total && long_value_flag == 1) {
            unsigned long long int* total_temp = (unsigned long long int*)malloc(sizeof(unsigned long long int*));

            if (total_temp) {
                if (second_num == 0 && second_temp == 0) {
                    fprintf(stderr, "Divide by zero error\n");
                    exit(1);
                }
                *total_temp = (unsigned long long int)(second_temp / first_temp);
                push(numbers_array, total_temp);
            }
        }
        else {
            if (total) {
                if (second_num == 0) {
                    fprintf(stderr, "Divide by zero error\n");
                    exit(1);
                }
                *total = (long double)(second_num / first_num);
                push(numbers_array, total);
            }
        }

        free_stack_memall(first_elem, second_elem, previous_element);

        break;
    case '^':
        first_elem = pop(numbers_array);
        second_elem = pop(numbers_array);
        first_num = *((long double*)(first_elem->data));
        second_num = *((long double*)(second_elem->data));
        if (first_num / 1 != first_num) {
            fprintf(stderr, "Power integer error\n");
            exit(1);
        }
        if (total)
            *total = pow(second_num, first_num);
        push(numbers_array, total);

        free_stack_memall(first_elem, second_elem, previous_element);

        break;
    case '!':
        first_elem = pop(numbers_array);
        first_num = *((long double*)(first_elem->data));

        if (first_num / 1 != first_num) {
            fprintf(stderr, "Float number is given to modulus operand.\n");
            exit(1);
        }
        if (total)
            *total = factorial((int)first_num);
        push(numbers_array, total);

        free_stack_mem(first_elem);
        free_stack_mem(previous_element);

        break;
    case '%':
        first_elem = pop(numbers_array);
        Stack* second_elem = pop(numbers_array);
        first_num = *((long double*)(first_elem->data));
        second_num = *((long double*)(second_elem->data));

        if (first_num / 1 != first_num || second_num / 1 != second_num) {
            fprintf(stderr, "Float number is given to modulus operand.\n");
            exit(1);
        }
        if (first_num == 0) {
            fprintf(stderr, "You can't get a remainder of the division by 0.\n");
            exit(1);
        }

        if (total)
            *total = (long double)((int)second_num % (int)first_num);

        push(numbers_array, total);

        free_stack_memall(first_elem, second_elem, previous_element);

        break;
    default:
        fprintf(stderr, "Unknown operand error: %c\n", current_character);
        exit(1);
    }

    if (current_character != '\0') use_operand(current_character, operands, numbers_array, long_value_flag);
}


void parse_string(char* input) 
{
    char* input_string = (char*)malloc(strlen(input) * 2 + 1);
    char* solver;
    for (; (solver = strstr(input, " ")) != NULL || (solver = strstr(input, "\n")) != NULL;) {
        if (input_string) {
            memcpy(input_string, input, (solver - input));
            memcpy(input_string + (solver - input), solver + 1, strlen(solver + 1) + 1);
            memcpy(input, input_string, strlen(input_string) + 1);
        }
    }
    for (; (solver = strstr(input, "-(")) != NULL;) {
        if (input_string) {
            memcpy(input_string, input, (solver - input) + 1);
            memcpy(input_string + (solver - input) + 3, solver + 1, strlen(solver + 1) + 1);
            input_string[solver - input + 1] = '1';
            input_string[solver - input + 2] = '*';
            memcpy(input, input_string, strlen(input_string) + 1);
        }
    }
}

