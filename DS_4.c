#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Book
{
    char* title_field;
    char* authors_field;
    int year_field;
    char* distributor_field;
    int rate_field;
    float cost_field;
    int id_field;

    struct Book* smaller;
    struct Book* larger;
    struct Book* parent;
}Book;

Book* books = NULL;
int books_counter = 0;
int id_counter = 0;

Book* create_book(char* title_field, char* authors_field, int year_field, char* distributor_field, int rate_field, float cost_field)
{
    Book* new_book = (Book*)malloc(sizeof(Book));

    if (new_book == NULL)
    {
        fprintf(stderr, "Could not allocate memory\n");
        return NULL;
    }

    new_book->title_field = (char*)malloc(sizeof(char) * (strlen(title_field) + 1));
    new_book->authors_field = (char*)malloc(sizeof(char) * (strlen(authors_field) + 1));
    new_book->distributor_field = (char*)malloc(sizeof(char) * (strlen(distributor_field) + 1));

    new_book->title_field = title_field;
    new_book->authors_field = authors_field;
    new_book->year_field = year_field;
    new_book->distributor_field = distributor_field;
    new_book->rate_field = rate_field;
    new_book->cost_field = cost_field;
    new_book->id_field = id_counter++;

    new_book->smaller = NULL;
    new_book->larger = NULL;
    new_book->parent = 0x0;

    return new_book;
}
void edit_id_field();
void show_help();
void init_file(char* fileName);
void travel_tree_lr(Book* book, FILE* stream, void (*func)(Book*, FILE*));
void traver_tree_rl(Book* book, FILE* stream, void (*func)(Book*, FILE*));
void normalist_root();
void edit_book_boosted(char** argv, int i);
int comparsion_name_books(Book* first, Book* second);
int comparsion_name_books(Book* first, Book* second);
int comparsion_authors_books(Book* first, Book* second);
int comparsion_year_books(Book* first, Book* second);
int comparsion_distributor_books(Book* first, Book* second);
int comparsion_rate_books(Book* first, Book* second);
int comparsion_cost_books(Book* first, Book* second);
void books_connect(Book* child, Book* parent, int side);
int search_level(Book* book);
void init_balance_database(Book* book);
void init_balance_recursion(Book* book);
void create_user_book(Book* book, Book** book_pointer);
Book* search_book(Book* book, Book* node);
void erase_book(Book* book, Book* node);
void init_sort_books(char* name_field);
void show_book(Book* book, FILE* stream);
void search_title_book(Book* book, char* data);
void search_authors_book(Book* book, char* data);
void search_authors_book(Book* book, char* data);
void search_year_book(Book* book, int data);
void search_year_book(Book* book, int data);
void add_book_data(Book* book, FILE* outputFile);
void search_distributor_book(Book* book, char* data);
void search_rate_book(Book* book, int data);
void search_cost_book(Book* book, float data);
void find_book();
void find_book_console(char** argv, int i);
Book* searchid_field(Book* book, int id_field);
void deleteid_field();
void erase_book_boosted(char** argv, int i);
void edit_id_field();
void save_to_file(char* fileName);
void init_end(char* pointer);
void parse_book_fields();
void creause_user_book_console(char** argv, int i);
void init_interactive();


int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        init_interactive();
        return 1;
    }
    for (int i = 0; i < argc; i++) {


        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {

            show_help();
            continue;
        }

        if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input"))
        {

            init_file(argv[i + 1]);
            continue;
        }

        if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--add"))
        {
            creause_user_book_console(argv, i);
            continue;
        }

        if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--print")) {

            if (strlen(argv[i + 1]) > 6)
            {
                char name_field[12];
                strncpy_s(name_field, (strlen(argv[i + 1]) - 6) * sizeof(char), argv[i + 1] + 6, strlen(argv[i + 1]) - 6);
                init_sort_books(name_field);
            }
            travel_tree_lr(books, stdout, show_book);
            continue;
        }

        if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--remove")) {

            erase_book_boosted(argv, i);

            continue;
        }

        if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--edit")) {

            edit_book_boosted(argv, i);

            continue;
        }
        if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {

            save_to_file(argv[i + 1]);
            continue;
        }

        if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--find")) {

            find_book_console(argv, i + 1);
            continue;
        }
    }
    return 0;
}


void show_help()
{
    printf("\tadd\t\t\t\t\tType to begin typing data of a new book\n");
    printf("\tdelete [id_field]\t\t\tDelete a book with the given id_field\n");
    printf("\tedit [id_field]\t\t\t\tEdit consequencely an old book\n");
    printf("\tsearch \t\t\t\t\tType to begin typing data field and data itself of an existing book\n");
    printf("\tprint [sort: field_type]\t\tPrint all the books in sorted (or not) order\n");
    printf("\tsave [file]\t\t\t\tSave database in a file\n");
    printf("\tload [file]\t\t\t\tLoad database from file\n");
    return;
}

void normalist_root()
{
    while (books->parent != NULL)
        books = books->parent;
}

int comparsion_name_books(Book* first, Book* second)
{

    if (first == NULL || second == NULL)
        return 0;

    if (strcmp(first->title_field, second->title_field))
        return strcmp(first->title_field, second->title_field);
    if (strcmp(first->authors_field, second->authors_field))
        return strcmp(first->authors_field, second->authors_field);
    if (first->year_field - second->year_field)
        return first->year_field - second->year_field;
    if (strcmp(first->distributor_field, second->distributor_field))
        return strcmp(first->distributor_field, second->distributor_field);
    if (first->rate_field - second->rate_field)
        return first->rate_field - second->rate_field;
    if (first->cost_field - second->cost_field)
        return (first->cost_field - second->cost_field) > 0.001 ? 1 : -1;

    return 0;
}

int comparsion_authors_books(Book* first, Book* second)
{

    if (first == NULL || second == NULL)
        return 0;

    if (strcmp(first->authors_field, second->authors_field))
        return strcmp(first->authors_field, second->authors_field);
    if (strcmp(first->title_field, second->title_field))
        return strcmp(first->title_field, second->title_field);
    if (first->year_field - second->year_field)
        return first->year_field - second->year_field;
    if (strcmp(first->distributor_field, second->distributor_field))
        return strcmp(first->distributor_field, second->distributor_field);
    if (first->rate_field - second->rate_field)
        return first->rate_field - second->rate_field;
    if (first->cost_field - second->cost_field)
        return (first->cost_field - second->cost_field) > 0.001 ? 1 : -1;

    return 0;
}

int comparsion_year_books(Book* first, Book* second)
{

    if (first == NULL || second == NULL)
        return 0;

    if (first->year_field - second->year_field)
        return first->year_field - second->year_field;
    if (strcmp(first->title_field, second->title_field))
        return strcmp(first->title_field, second->title_field);
    if (strcmp(first->authors_field, second->authors_field))
        return strcmp(first->authors_field, second->authors_field);
    if (strcmp(first->distributor_field, second->distributor_field))
        return strcmp(first->distributor_field, second->distributor_field);
    if (first->rate_field - second->rate_field)
        return first->rate_field - second->rate_field;
    if (first->cost_field - second->cost_field) {
        if (first->cost_field - second->cost_field > 0.001) return 1;
        else return -1;
    }

    return 0;
}

int comparsion_distributor_books(Book* first, Book* second)
{

    if (first == NULL || second == NULL)
        return 0;

    if (strcmp(first->distributor_field, second->distributor_field))
        return strcmp(first->distributor_field, second->distributor_field);
    if (strcmp(first->title_field, second->title_field))
        return strcmp(first->title_field, second->title_field);
    if (strcmp(first->authors_field, second->authors_field))
        return strcmp(first->authors_field, second->authors_field);
    if (first->year_field - second->year_field)
        return first->year_field - second->year_field;
    if (first->rate_field - second->rate_field)
        return first->rate_field - second->rate_field;
    if (first->cost_field - second->cost_field) {
        if (first->cost_field - second->cost_field > 0.001) return 1;
        else return -1;
    }
    return 0;
}

int comparsion_rate_books(Book* first, Book* second)
{

    if (first == NULL || second == NULL)
        return 0;

    if (first->rate_field - second->rate_field)
        return first->rate_field - second->rate_field;
    if (strcmp(first->title_field, second->title_field))
        return strcmp(first->title_field, second->title_field);
    if (strcmp(first->authors_field, second->authors_field))
        return strcmp(first->authors_field, second->authors_field);
    if (first->year_field - second->year_field)
        return first->year_field - second->year_field;
    if (strcmp(first->distributor_field, second->distributor_field))
        return strcmp(first->distributor_field, second->distributor_field);
    if (first->cost_field - second->cost_field) {
        if (first->cost_field - second->cost_field > 0.001) return 1;
        else return -1;
    }
    return 0;
}

int comparsion_cost_books(Book* first, Book* second)
{

    if (first == NULL || second == NULL)
        return 0;

    if (first->cost_field - second->cost_field) {
        if (first->cost_field - second->cost_field > 0.001) return 1;
        else return -1;
    }
    if (first->rate_field - second->rate_field)
        return first->rate_field - second->rate_field;
    if (strcmp(first->title_field, second->title_field))
        return strcmp(first->title_field, second->title_field);
    if (strcmp(first->authors_field, second->authors_field))
        return strcmp(first->authors_field, second->authors_field);
    if (first->year_field - second->year_field)
        return first->year_field - second->year_field;
    if (strcmp(first->distributor_field, second->distributor_field))
        return strcmp(first->distributor_field, second->distributor_field);
    return 0;
}
int (*comparsion_func)(Book*, Book*) = comparsion_name_books;

void books_connect(Book* child, Book* parent, int side)
{

    if (parent != NULL)
    {
        if (side)
        {
            parent->larger = child;
        }
        else
        {
            parent->smaller = child;
        }
    }

    if (child != NULL)
        child->parent = parent;
}

int search_level(Book* book)
{

    if (book == NULL)
        return 0;

    if (book->larger == NULL && book->smaller == NULL)
        return 1;
    int bigger_book = book->larger == NULL ? 0 : search_level(book->larger);
    int smaller_book = book->smaller == NULL ? 0 : search_level(book->smaller);
    return bigger_book > smaller_book ? bigger_book + 1 : smaller_book + 1;
}

void init_balance_database(Book* book)
{ // A

    int bigger_book = search_level(book->larger);
    int smaller_book = search_level(book->smaller);

    if (abs(bigger_book - smaller_book) < 2)
        return;

    Book* bigger_tree = bigger_book > smaller_book ? // B
        book->larger
        : book->smaller;

    int left_level = search_level(bigger_tree->smaller);
    int rigth_level = search_level(bigger_tree->larger);

    if (bigger_book > smaller_book)
    {
        if (left_level < rigth_level)
        {
            Book* base_child = bigger_tree->smaller;
            books_connect(bigger_tree, book->parent,
                comparsion_func(book, book->parent) < 0 ? 0 : 1);
            books_connect(base_child, book, 1);
            books_connect(book, bigger_tree, 0);
        }
        else if (left_level > rigth_level)
        {
            // bigger_tree->smaller -- C
            Book* base_left = bigger_tree->smaller->smaller;
            Book* base_right = bigger_tree->smaller->larger;

            books_connect(bigger_tree->smaller, book->parent,
                comparsion_func(book, book->parent) < 0 ? 0 : 1);

            books_connect(bigger_tree, bigger_tree->smaller, 1);
            books_connect(book, bigger_tree->smaller, 0);

            books_connect(base_left, book, 1);
            books_connect(base_right, bigger_tree, 0);
        }
    }
    else
    {
        if (left_level > rigth_level)
        {
            Book* base_child = bigger_tree->larger;
            books_connect(bigger_tree, book->parent,
                comparsion_func(book, book->parent) < 0 ? 1 : 0);
            books_connect(book, bigger_tree, 0);
            books_connect(base_child, book, 1);
        }
        else if (left_level < rigth_level)
        {
            Book* base_left = bigger_tree->larger->smaller;
            Book* base_right = bigger_tree->larger->larger;

            books_connect(bigger_tree->larger, book->parent,
                comparsion_func(book, book->parent) < 0 ? 1 : 0);

            books_connect(bigger_tree, bigger_tree->larger, 0);
            books_connect(book, bigger_tree->larger, 1);

            books_connect(base_left, bigger_tree, 0);
            books_connect(base_right, book, 1);
        }
    }

    normalist_root();
}

void init_balance_recursion(Book* book)
{

    if (book == NULL)
        return;

    init_balance_database(book);
    init_balance_recursion(book->smaller);
    init_balance_recursion(book->larger);
}

void create_user_book(Book* book, Book** book_pointer)
{

    if (*book_pointer == NULL)
    {
        *book_pointer = book;
        books_counter += 1;
        return;
    }

    if (book == NULL)
        return;

    int result = comparsion_func(book, *book_pointer);
    if (result > 0)
    {
        if ((*book_pointer)->larger == NULL)
            book->parent = *book_pointer;
        create_user_book(book, &((*book_pointer)->larger));
    }
    else if (result < 0)
    {
        if ((*book_pointer)->smaller == NULL)
            book->parent = *book_pointer;
        create_user_book(book, &((*book_pointer)->smaller));
    }
    else
    {
        printf("[Duplicate located: %s]\n", book->title_field);
    }

    init_balance_database(*book_pointer);
}

Book* search_book(Book* book, Book* node)
{

    if (node == NULL)
        return NULL;

    int result = comparsion_func(book, node);
    if (result > 0)
        return search_book(book, node->larger);
    else if (result < 0)
        return search_book(book, node->smaller);
    else
        return node;
}

void erase_book(Book* book, Book* node)
{

    if (node == NULL)
        return;

    book = search_book(book, node);

    if (book == NULL)
        return;

    if (book->parent != NULL)
    {
        int result = comparsion_func(book, book->parent);
        if (result > 0)
            book->parent->larger = NULL;
        else
            book->parent->smaller = NULL;
    }

    if (book->larger == NULL)
    {
        books_connect(book->smaller, book->parent,
            comparsion_func(book->smaller, book->parent));
        if (book == books)
            books = book->smaller;
        free(book);
        books_counter -= 1;
        return;
    }

    books_connect(book->larger, book->parent,
        comparsion_func(book->larger, book->parent));
    create_user_book(book->larger->smaller, &(book->smaller));
    books_connect(book->smaller, book->larger, 0);

    if (book == books)
        books = book->larger;

    if (node == books)
        books_counter -= 1;
    free(book);

    init_balance_recursion(books);

    normalist_root();
}

void init_sort_books(char* name_field)
{

    if (!strncmp(name_field, "title_field", 5))
    {
        comparsion_func = comparsion_name_books;
    }
    else if (!strncmp(name_field, "authors_field", 7))
    {
        comparsion_func = comparsion_authors_books;
    }
    else if (!strncmp(name_field, "year_field", 4))
    {
        comparsion_func = comparsion_year_books;
    }
    else if (!strncmp(name_field, "distributor_field", 11))
    {
        comparsion_func = comparsion_distributor_books;
    }
    else if (!strncmp(name_field, "rate_field", 4))
    {
        comparsion_func = comparsion_rate_books;
    }
    else if (!strncmp(name_field, "cost_field", 4))
    {
        comparsion_func = comparsion_cost_books;
    }
    else
    {
        return;
    }

    Book* old_base_root = books;
    books = NULL;

    int idCounterSave = id_counter;

    while (old_base_root != NULL)
    {

        Book* book = create_book(old_base_root->title_field, old_base_root->authors_field, old_base_root->year_field,
            old_base_root->distributor_field, old_base_root->rate_field, old_base_root->cost_field);
        create_user_book(book, &books);
        book->id_field = old_base_root->id_field;
        Book* save;
        if (old_base_root->larger == NULL)
            save = old_base_root->smaller;
        else
            save = old_base_root->larger;
        erase_book(old_base_root, old_base_root);
        old_base_root = save;
        if (old_base_root == NULL)
            continue;
        while (old_base_root->parent != NULL)
            old_base_root = old_base_root->parent;
        init_balance_recursion(old_base_root);
    }

    id_counter = idCounterSave;
}

void show_book(Book* book, FILE* stream)
{

    fprintf(stream, "Title: %s\n", book->title_field);
    fprintf(stream, "Authors: %s\n", book->authors_field);
    fprintf(stream, "Year: %d\n", book->year_field);
    fprintf(stream, "Distributor: %s\n", book->distributor_field);
    fprintf(stream, "Rate: %d\n", book->rate_field);
    fprintf(stream, "Cost: %.2f\n", book->cost_field);

    if (stream == stdout)
        fprintf(stream, "id_field: %d\n\n", book->id_field);
}

void search_title_book(Book* book, char* data)
{

    if (book == NULL)
        return;

    search_title_book(book->smaller, data);
    if (strstr(book->title_field, data) != NULL) {
        show_book(book, stdout);
    }

    search_title_book(book->larger, data);
}

void search_authors_book(Book* book, char* data)
{

    if (book == NULL)
        return;

    search_authors_book(book->smaller, data);
    if (strstr(book->authors_field, data) != NULL)
        show_book(book, stdout);
    search_authors_book(book->larger, data);
}

void search_year_book(Book* book, int data)
{

    if (book == NULL)
        return;

    search_year_book(book->smaller, data);
    if (book->year_field == data)
        show_book(book, stdout);
    search_year_book(book->larger, data);
}

void search_distributor_book(Book* book, char* data)
{

    if (book == NULL)
        return;

    search_distributor_book(book->smaller, data);
    if (strstr(book->distributor_field, data) != NULL)
        show_book(book, stdout);
    search_distributor_book(book->larger, data);
}

void search_rate_book(Book* book, int data)
{

    if (book == NULL)
        return;

    search_rate_book(book->smaller, data);
    if (book->rate_field == data)
        show_book(book, stdout);
    search_rate_book(book->larger, data);
}

void search_cost_book(Book* book, float data)
{

    if (book == NULL)
        return;

    search_cost_book(book->smaller, data);
    if (book->cost_field == data)
        show_book(book, stdout);
    search_cost_book(book->larger, data);
}

void find_book()
{
    char* name_field = (char*)malloc(sizeof(char) * 13);

    printf("Type field type to search: ");
    fflush(stdout);
    fgets(name_field, 12, stdin);
    name_field[strlen(name_field) - 1] = 0;

    printf("Type data to compare: ");
    fflush(stdout);

    if (!strncmp(name_field, "Title", 6))
    {
        char* data = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
        fgets(data, 64 * 1024 * 1024, stdin);
        data[strlen(data) - 1] = 0;

        search_title_book(books, data);

        free(data);
    }
    else if (!strncmp(name_field, "Authors", 8))
    {
        char* data = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
        fgets(data, 64 * 1024 * 1024, stdin);
        data[strlen(data) - 1] = 0;

        search_authors_book(books, data);

        free(data);
    }
    else if (!strncmp(name_field, "Year", 5))
    {
        int data;
        int tmp = scanf_s("%d", &data);

        search_year_book(books, data);
        /*char c;
        while ((c = fgetc(stdin)) != EOF && (c != '\n'))
            ;*/
    }
    else if (!strncmp(name_field, "Distributor", 12))
    {
        char* data = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
        fgets(data, 10, stdin);
        fgets(data, 64 * 1024 * 1024, stdin);
        data[strlen(data) - 1] = 0;

        search_distributor_book(books, data);

        free(data);
    }
    else if (!strncmp(name_field, "Rate", 5))
    {
        int data;
        int tmp = scanf_s("%d", &data);

        search_rate_book(books, data);
        /* char c;
         while ((c = fgetc(stdin)) != EOF && (c != '\n'))
             ;*/
    }
    else if (!strncmp(name_field, "Cost", 5))
    {
        float data;
        int temp = scanf_s("%f", &data);

        search_cost_book(books, data);
        /*char c;
        while ((c = fgetc(stdin)) != EOF && (c != '\n'))
            ;*/
    }

    free(name_field);
}

void find_book_console(char** argv, int i)
{
    char* name_field = (char*)malloc(sizeof(char) * 13);
    if (name_field) {
        name_field = argv[i];




        if (!strncmp(name_field, "Title", 6))
        {
            char* data = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
            if (data) {
                data = argv[i + 1];



                search_title_book(books, data);


            }
        }
        else if (!strncmp(name_field, "Authors", 8))
        {
            char* data = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
            data = argv[i + 1];
            data[strlen(data) - 1] = 0;

            search_authors_book(books, data);

            free(data);
        }
        else if (!strncmp(name_field, "Year", 5))
        {
            int data;
            data = atoi(argv[i + 1]);

            search_year_book(books, data);
            char c;
            while ((c = fgetc(stdin)) != EOF && (c != '\n'))
                ;
        }
        else if (!strncmp(name_field, "Distributor", 12))
        {
            char* data = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
            data = argv[i + 1];
            data[strlen(data) - 1] = 0;

            search_distributor_book(books, data);

            free(data);
        }
        else if (!strncmp(name_field, "Rate", 5))
        {
            int data;
            data = atoi(argv[i + 1]);

            search_rate_book(books, data);
            char c;
            while ((c = fgetc(stdin)) != EOF && (c != '\n'))
                ;
        }
        else if (!strncmp(name_field, "Cost", 5))
        {
            float data;
            data = strtof(argv[i + 1], '\0');

            search_cost_book(books, data);
            char c;
            while ((c = fgetc(stdin)) != EOF && (c != '\n'))
                ;
        }


    }
}

Book* searchid_field(Book* book, int id_field)
{

    if (book == NULL)
        return NULL;

    if (book->id_field == id_field)
        return book;
    Book* smaller_book = searchid_field(book->smaller, id_field);

    if (smaller_book != NULL)
        return smaller_book;

    return searchid_field(book->larger, id_field);
}

void deleteid_field()
{

    printf("Print id_field: ");
    fflush(stdout);

    int id_field;
    int temp = scanf_s("%d", &id_field);


    Book* book = searchid_field(books, id_field);
    if (book == NULL)
        printf("Book doesnt exist error\n");
    else
        erase_book(book, books);
}

void erase_book_boosted(char** argv, int i) 
{
    int id_field;
    id_field = atoi(argv[i + 1]);

    Book* book = searchid_field(books, id_field);
    if (book == NULL)
        printf("Book doesnt exist error\n");
    else
        erase_book(book, books);
}

void edit_id_field()
{

    printf("Print id_field: ");
    fflush(stdout);

    int id_field;
    int tmp = scanf_s("%d", &id_field);
    char c;
    while ((c = fgetc(stdin)) != EOF && (c != '\n'))
        ;

    Book* book = searchid_field(books, id_field);
    if (book == NULL)
        printf("Book doesnt exist error\n");
    else
    {
        char* buffer = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
        for (int i = 0; i < 6; i++)
        {

            switch (i)
            {
            case 0:
                printf("Title: ");
                break;
            case 1:
                printf("Authors: ");
                break;
            case 2:
                printf("Year: ");
                break;
            case 3:
                printf("Distributor: ");
                break;
            case 4:
                printf("Rate: ");
                break;
            case 5:
                printf("Cost: ");
                break;
            }
            fflush(stdout);

            fgets(buffer, 64 * 1024 * 1024, stdin);
            buffer[strlen(buffer) - 1] = 0;

            switch (i)
            {
            case 0:
                if (strlen(buffer) > 0)
                {

                    book->title_field = (char*)malloc(sizeof(char*) * strlen(buffer));
                    if (book->title_field)strncpy_s(book->title_field, strlen(buffer) * sizeof(char*), buffer, strlen(buffer));
                }
                break;
            case 1:
                if (strlen(buffer) > 0)
                {

                    book->authors_field = (char*)malloc(sizeof(char*) * strlen(buffer));
                    if (book->authors_field)strncpy_s(book->authors_field, strlen(buffer) * sizeof(char*), buffer, strlen(buffer));
                }
                break;
            case 2:
                if (atoi(buffer) != 0)
                    book->year_field = atoi(buffer);
                break;
            case 3:
                if (strlen(buffer) > 0)
                {

                    book->distributor_field = (char*)malloc(sizeof(char*) * strlen(buffer));
                    strncpy_s(book->distributor_field, strlen(buffer) * sizeof(char*), buffer, strlen(buffer));
                }
                break;
            case 4:
                if (atoi(buffer) != 0)
                    book->rate_field = atoi(buffer);
                break;
            case 5:
                if (atof(buffer) != 0)
                    book->cost_field = strtof(buffer, '\0');
                break;
            }
        }

    }
}

void edit_book_boosted(char** argv, int i) 
{
    int id_field;
    int j = i + 1;
    id_field = atoi(argv[j]);

    Book* book = searchid_field(books, id_field);
    if (book == NULL)
        printf("Book doesnt exist error\n");
    else
    {

        book->title_field = argv[j + 1];

        book->authors_field = argv[j + 2];

        book->year_field = atoi(argv[j + 3]);


        book->distributor_field = argv[j + 4];

        book->rate_field = atoi(argv[j + 5]);

        book->cost_field = strtof(argv[j + 6], '\0');
    }
}

void init_file(char* fileName)
{

    FILE* inputFile = NULL;
    errno_t err = 0;

    if ((err = fopen_s(&inputFile, fileName, "r")) != 0)
    {
        fprintf(stderr, "Cant reach the file error\n");
        return;
    }

    char* buffer = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));

    while (!feof(inputFile))
    {
        char* title_field = 0;
        char* authors_field = 0;
        char* distributor_field = 0;
        int rate_field = -100, year_field = -3000;
        float cost_field = -10000;
        for (int i = 0; i < 6; i++)
        {
            fgets(buffer, 64 * 1024 * 1024, inputFile);
            buffer[strlen(buffer) - 1] = 0;

            if (feof(inputFile))
                break;

            switch (i)
            {
            case 0:
                title_field = (char*)malloc(sizeof(char*) * (strlen(buffer) + 1));
                if (title_field)strncpy_s(title_field, strlen(buffer) * sizeof(buffer), buffer, strlen(buffer));
                break;
            case 1:
                authors_field = (char*)malloc(sizeof(char*) * (strlen(buffer) + 1));
                if (authors_field)strncpy_s(authors_field, strlen(buffer) * sizeof(buffer), buffer, strlen(buffer));
                break;
            case 2:
                year_field = atoi(buffer);
                break;
            case 3:
                distributor_field = (char*)malloc(sizeof(char*) * (strlen(buffer) + 1));
                if (distributor_field)strncpy_s(distributor_field, strlen(buffer) * sizeof(buffer), buffer, strlen(buffer));
                break;
            case 4:
                rate_field = atoi(buffer);
                break;
            case 5:
                cost_field = strtof(buffer, '\0');
                break;
            }
        }

        if (title_field == NULL || authors_field == NULL || distributor_field == NULL || rate_field == -100 || cost_field == -10000 || year_field == -3000)
            break;

        Book* new_book = create_book(title_field, authors_field, year_field, distributor_field, rate_field, cost_field);

        create_user_book(new_book, &books);
        show_book(new_book, stdout);
    }

    fclose(inputFile);
    free(buffer);

    return;
}

void add_book_data(Book* book, FILE* outputFile)
{

    if (outputFile == NULL || book == NULL)
        return;

    fprintf(outputFile, "%s\n", book->title_field);
    fprintf(outputFile, "%s\n", book->authors_field);
    fprintf(outputFile, "%d\n", book->year_field);
    fprintf(outputFile, "%s\n", book->distributor_field);
    fprintf(outputFile, "%d\n", book->rate_field);
    fprintf(outputFile, "%.2f\n", book->cost_field);
}

void travel_tree_lr(Book* book, FILE* stream, void (*func)(Book*, FILE*))
{

    if (book == NULL)
        return;

    travel_tree_lr(book->smaller, stream, func);
    func(book, stream);
    travel_tree_lr(book->larger, stream, func);
}

void traver_tree_rl(Book* book, FILE* stream, void (*func)(Book*, FILE*))
{

    if (book == NULL)
        return;

    traver_tree_rl(book->smaller, stream, func);
    func(book, stream);
    traver_tree_rl(book->larger, stream, func);
}

void save_to_file(char* fileName)
{

    FILE* outputFile = NULL;
    errno_t err = 0;
    if ((err = fopen_s(&outputFile, fileName, "w")) != 0)
    {
        fprintf(stderr, "Cant reach output file error\n");
        return;
    }

    travel_tree_lr(books, outputFile, add_book_data);

    fclose(outputFile);
}

void init_end(char* pointer)
{
    int position = 0;
    while (pointer[position++] != 'H');
    pointer[position] = '\0';
}

void parse_book_fields()
{

    char* buffer = (char*)malloc(sizeof(char) * (64 * 1024 * 1024 + 1));
    char* title_field = 0;
    char* authors_field = 0;
    char* distributor_field = 0;
    int rate_field = 0;
    float cost_field = 0;
    int year_field = 0;

    for (int i = 0; i < 6; i++)
    {

        switch (i)
        {
        case 0:
            printf("Title: ");
            break;
        case 1:
            printf("Authors: ");
            break;
        case 2:

            printf("Year: ");
            break;
        case 3:

            printf("Distributor: ");
            break;
        case 4:

            printf("Rate: ");
            break;
        case 5:

            printf("Cost: ");
            break;
        }
        fflush(stdout);
        memset(buffer, 0, 64 * 1024 * 1024);
        fgets(buffer, 64 * 1024 * 1024, stdin);
        buffer[strlen(buffer) - 1] = 0;




        switch (i)
        {
        case 0:
            title_field = (char*)malloc(sizeof(char*) * strlen(buffer) + 1);
            if (title_field)strncpy_s(title_field, strlen(buffer) * sizeof(buffer), buffer, strlen(buffer));



            break;
        case 1:
            authors_field = (char*)malloc(sizeof(char*) * strlen(buffer) + 1);
            if (authors_field)strncpy_s(authors_field, sizeof(buffer) * strlen(buffer), buffer, strlen(buffer));
            break;
        case 2:
            year_field = atoi(buffer);

            break;
        case 3:
            distributor_field = (char*)malloc(sizeof(char*) * strlen(buffer) + 1);
            if (distributor_field)strncpy_s(distributor_field, sizeof(buffer) * strlen(buffer), buffer, strlen(buffer));

            break;
        case 4:
            rate_field = atoi(buffer);

            break;
        case 5:
            cost_field = strtof(buffer, '\0');

            break;
        }
    }

    if (title_field == NULL || authors_field == NULL || distributor_field == NULL || rate_field == -100 || cost_field == -10000 || year_field == -5000)
        return;

    Book* new_book = create_book(title_field, authors_field, year_field, distributor_field, rate_field, cost_field);

    create_user_book(new_book, &books);

    printf("\nid_field ==> %d\n", new_book->id_field);
}

void creause_user_book_console(char** argv, int i) 
{
    int j = i + 1;
    char* title_field = argv[j];
    char* authors_field = argv[j + 1];
    int year_field = atoi(argv[j + 2]);
    char* distributor_field = argv[j + 3];
    int rate_field = atoi(argv[j + 4]);
    float cost_field = strtof(argv[j + 5], '\0');

    Book* new_book = create_book(title_field, authors_field, year_field, distributor_field, rate_field, cost_field);
    create_user_book(new_book, &books);
}

void init_interactive()
{

    show_help();

    char command[129];

    while ("Pedri Negorasi")
    {

        printf("\n> ");
        fflush(stdout);
        fgets(command, 129, stdin);

        command[strlen(command) - 1] = 0;

        if (!strncmp(command, "load", 4))
        {
            if (strlen(command) < 6)
            {
                show_help();
                continue;
            }
            char fileName[1024];
            fileName[strlen(command)] = 0;
            strncpy_s(fileName, (strlen(fileName) - 4) * sizeof(char), command + 5, strlen(fileName) - 4);
            init_file(fileName);
        }
        else if (!strncmp(command, "add", 3))
        {
            parse_book_fields();
        }
        else if (!strncmp(command, "print", 5))
        {
            if (strlen(command) > 6)
            {
                char name_field[32];
                name_field[strlen(command) + 1] = 0;
                strncpy_s(name_field, (strlen(name_field) - 6) * sizeof(char), command + 6, strlen(name_field) - 6);
                init_sort_books(name_field);
            }
            travel_tree_lr(books, stdout, show_book);
        }
        else if (!strncmp(command, "save", 4))
        {
            if (strlen(command) < 6)
            {
                show_help();
                continue;
            }
            char fileName[257];
            fileName[strlen(command)] = 0;
            strncpy_s(fileName, (strlen(fileName)) * sizeof(char), command + 5, strlen(fileName));
            save_to_file(fileName);
        }
        else if (!strncmp(command, "search", 6))
        {
            find_book();
        }
        else if (!strncmp(command, "delete", 6))
        {
            deleteid_field();
        }
        else if (!strncmp(command, "edit", 4))
        {
            edit_id_field();
        }
        else
        {
            show_help();
        }
    }
}


