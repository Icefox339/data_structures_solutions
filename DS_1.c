#include <stdio.h>
#include <math.h>
#include <time.h>
#include <locale.h>

#define be_1 302.4
#define be_2 504.0
#define be_3 180.0
#define be_4 13.5 
#define be_5 0.1

#define ev_1 0.7107
#define ev_2 9.488

unsigned long long m = 4294967296;//2^32 модуль, длина машинного слова
// потенциал  лкп с максимальным периодом определяется как наименьшее целое число s , такое, что 
// b^s == 0 ( по модулю m), хороший - больше 5 
typedef enum { // с вики
    a = 22695477, //множитель b = a -1 //  если m делится на различные простые числа, то только а = 1 обеспечит максимальный период. Если m делится на простое число в степени, то 
    // а можно попробовать выбрать 
    c = 1, // приращение    c = 0 - меньший период, мультипликативный км. с != 0 - смешанный км 
    N = 1000,
    number_period = 10,
}consts;
// лкп имеет период м, только тогда, когда  с и м взаимно простые, b = a -1  кратно p, для каждого простого р, являющегося делителем м, b кратно 4, если m кратно 4.
// если с = 0, то Хn и m - взаимно простые числа, чтобы обеспечить максимально возможный период 
unsigned long long enhance_number(unsigned long long* current_number);
void verify_period(unsigned long long* current_number);
void check_power();
void verify_xi_square(unsigned long long current_number);
void binary_form(unsigned long long int in, int count, int* out);
void verify_bit(unsigned long long current_number);
void classic_poker_combinator(int* numbers, int* categories);
int poker_test(unsigned long long* A, int n);
void verify_poker_combinations(unsigned long long current_number);
void linear_conguarate_generator(unsigned long long current_number);

int main() 
{
    unsigned long long current_number = time(NULL) % m;
    linear_conguarate_generator(current_number);
    return 0;
}

unsigned long long enhance_number(unsigned long long* current_number)
{
    *current_number = ((a * *current_number) + c) % m;// лкг мистер Xn+1 =  (a*Xn + c) mod m 
    return *current_number; // также можно выразить n+k член через Xn+k = (a^k * Xn+k + (a^k - 1)c/b) mod m   
    //a^k mod m - a для к, ((a^k - 1) c/b) mod m  - c для к 
}

void linear_conguarate_generator(unsigned long long current_number)
{//критерии комбинаций последних 5 чисел
    printf("Choosen parameters:\n a = 22695477\n c = 1\n m = 4294967296\n");
    printf("");
    verify_period(&current_number);
    check_power();
    printf("");
    verify_xi_square(current_number);
    printf("");
    verify_bit(current_number);
    printf("");
    verify_poker_combinations(current_number); // выпадение одинаковых цифр в числе

}

void verify_period(unsigned long long* current_number)
{ // проверяем с какой попытки мы генерируем число второй раз
    unsigned long long int count;
    unsigned long long int period = 1;
    count = enhance_number(current_number);
    while (count != enhance_number(current_number)) { // проверяем первое повторение
        period++;
    }
    printf("\nPeriod is: %llu", period);
}

void check_power()
{ // проверяем мощность алгоритма -- какой у нас период 
    unsigned long long  b = a - 1;
    unsigned long long  b_minus_one = a - 1;
    for (int s = 2; s < N; s++) {
        b = b * b_minus_one;
        if (b % m == 0) {
            printf("\nPower: %d \n", s);
            break;
        }
    }
}

void verify_xi_square(unsigned long long current_number) 
{ // вероятностное отклонение от ожидания 
    float expected_numbers[number_period];
    for (int i = 0; i < number_period; i++) {
        expected_numbers[i] = 100;//(float) 100 / n;
    }
    int ten_periods[number_period] = { 0 };
    unsigned long long value;
    for (int i = 0; i < N; i++) {
        value = enhance_number(&current_number);
        value = value % 1001;
        if ((value < 100))   ten_periods[0]++;
        if ((100 <= value) && (value < 200))   ten_periods[1]++;
        if ((200 <= value) && (value < 300))   ten_periods[2]++;
        if ((300 <= value) && (value < 400))   ten_periods[3]++;
        if ((400 <= value) && (value < 500))   ten_periods[4]++;
        if ((500 <= value) && (value < 600))   ten_periods[5]++;
        if ((600 <= value) && (value < 700))   ten_periods[6]++;
        if ((700 <= value) && (value < 800))   ten_periods[7]++;
        if ((800 <= value) && (value < 900))   ten_periods[8]++;
        if ((900 <= value) && (value <= 1000)) ten_periods[9]++;
    }
    double verification = 0; // плохой выбор = большое верификейшн 
    for (int i = 0; i < number_period; i++) {
        verification += (((double)(pow((double)ten_periods[i] - expected_numbers[i], 2))) / expected_numbers[i]);
    }
    printf("\nverification average = %f \nPercentage point of Xi^2-spread founds in interval from 10%% uo to 90%%, so that proofs the randomness of generated numbers\n", verification);
}

void binary_form(unsigned long long int in, int count, int* out)
{
    unsigned int mask = 1U << (count - 1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}

void verify_bit(unsigned long long current_number)
{ // 
    unsigned long long array_number[32] = { 0 };
    for (int i = 0; i < 32; i++) {
        array_number[i] = enhance_number(&current_number);
    }
    int digits[32][32];
    for (int i = 0; i < 32; i++) {
        binary_form(array_number[i], 32, digits[i]);
    }
    printf("\nLast 4 number's bit of consequence:\n");
    for (int i = 0; i < 32; i++) {
        for (int j = 32 - 4; j < 32; j++) {
            printf("%d", digits[i][j]);
        }
        printf("\n");
    }
    printf("Bytes create consequence with 16 period\n");
}

void classic_poker_combinator(int* numbers, int* categories)
{
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (numbers[j] > numbers[j + 1])
            {
                int tmp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = tmp;
            }
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (numbers[i] == numbers[i + 1])
            count++;
    }
    categories[count] += 1;
}

int poker_test(unsigned long long* A, int n)
{
    unsigned long long numbers_count = 0;
    int categories[5] = { 0 };
    int numbers[5] = { 0 };
    for (int i = 0; i < N; i++)
    {
        unsigned long long temp = 0;
        temp = A[i];
        numbers_count++;
        for (int i = 0; i < 5; i++)
        {
            numbers[i] = temp % 10;
            temp /= 10;
        }
        classic_poker_combinator(numbers, categories);
    }
    double verification = 0;
    double probability_expected[5];
    probability_expected[0] = be_1 * pow(10, -3);
    probability_expected[1] = be_2 * pow(10, -3);
    probability_expected[2] = be_3 * pow(10, -3);
    probability_expected[3] = be_4 * pow(10, -3);
    probability_expected[4] = be_5 * pow(10, -3);
    for (int i = 0; i < 5; i++)
    {
        verification += (double)categories[i] / numbers_count * categories[i] / probability_expected[i];
    }
    verification -= numbers_count;
    if (verification > ev_1 && verification < ev_2)
    {
        printf("verification = %f, this value falls within the range [0.7107-9.488]\n", verification);
        n++;
    }
    return n;
}

void verify_poker_combinations(unsigned long long current_number)
{ // упрощенный критерий по группам лучшая группа - нет совпадений цифр
    printf("\nResults of poker test:\n");
    int tests_number = 10;
    int tests_done = 0;
    unsigned long long array[N] = { 0 };
    while (tests_number)
    {
        unsigned long long int i = N;
        while (i)
        {
            current_number = enhance_number(&current_number);
            array[i - 1] = current_number;
            i--;
        }
        if (poker_test(array, 0) == 1)
            tests_done++;
        tests_number--;
    }
    printf("Finally,  %d out of 10 tests passed \n\n\n", tests_done);
}




