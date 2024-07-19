#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MEM_ALLOCATED 536870912
#define MAX_STRING_LEN 2048


typedef enum
{
    OPEN_SUCCESS,
    OPEN_ERROR
} file_status;

typedef struct 
{
    FILE* config_file;
    FILE* input_file;
    FILE* output_file;
} program_files;

void show_help();
void read_source_file(FILE* input_file, char* input);
void replace_fragments(const char* old_fragment, const char* new_fragment, char* input, char* output);
void use_config_file(FILE* config_file, char* input, char* output);
file_status open_file(const char* filePath, const char* mode, FILE** file);
void parse_string(program_files* files, int argc, char** argv);
void free_all(char* input_string, char* output_string, program_files* files);
void print_to_user(FILE* output_file, char* input_string, double time_spent);
void proverka/*ya ne mog uderzhatsya*/(FILE* input_file, char* input_string, char* output_string);
void arg_init(int argc);

int main(int argc, char** argv) 
{
    arg_init(argc);
    program_files program_files;
    memset(&program_files, 0, sizeof(program_files));
    parse_string(&program_files, argc, argv);
    char* input_string = (char*)malloc(MAX_MEM_ALLOCATED);
    char* output_string = (char*)malloc(MAX_MEM_ALLOCATED);
    proverka(program_files.input_file, input_string, output_string);
    double start = (double)clock();

    read_source_file(program_files.input_file, input_string);
    use_config_file(program_files.config_file, input_string, output_string);

    double end = clock();
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;

    print_to_user(program_files.output_file, input_string, time_spent);
    free_all(input_string, output_string, &program_files);

    return 0;
}
void show_help()
{
    printf("-c / --config \t destination to configuration file.\n");
    printf("-i / --input - destination to input file.\n");
    printf("-o / --output - destination to output file.\n");
    printf("-h / --help - show help.\n");
    exit(1);
}

void read_source_file(FILE* input_file, char* input)
{
    int position = 0;
    char c = 0;
    while ((c = fgetc(input_file)) != EOF) {
        input[position] = c;
        position++;
    }
}

void replace_fragments(const char* old_fragment, const char* new_fragment, char* input, char* output)
{
    int position = 0;
    char* start = input;
    char* fragment_pointer = 0;
    for (; (fragment_pointer = strstr(start, old_fragment)) != NULL;) {
        int distance = (int)(fragment_pointer - start);
        strncat_s(output, MAX_MEM_ALLOCATED, start, distance);
        strncat_s(output, MAX_MEM_ALLOCATED, new_fragment, MAX_STRING_LEN);
        start = fragment_pointer + strlen(old_fragment);
        position += (int)strlen(old_fragment);
    }
    strcat_s(output, MAX_MEM_ALLOCATED, start);
    strcpy_s(input, MAX_MEM_ALLOCATED, output);
    memset(output, 0, MAX_MEM_ALLOCATED);
}

void use_config_file(FILE* config_file, char* input, char* output)
{
    char buffer_string[MAX_STRING_LEN];
    memset(buffer_string, 0, sizeof(buffer_string));

    char old_fragment[MAX_STRING_LEN];
    char new_fragment[MAX_STRING_LEN];

    int source_len = 0;
    fseek(config_file, 0, SEEK_END);
    source_len = ftell(config_file);
    fseek(config_file, 0, SEEK_SET);

    int completed_chars = 0;
    int total_completed_chars = 0;
    int progress = 0;

    for (; fgets(buffer_string, sizeof(buffer_string), config_file) != NULL;) {
        completed_chars = (int)strlen(buffer_string);
        total_completed_chars += completed_chars;
        if ((strchr(buffer_string, '^')) != NULL) {
            int temp = sscanf_s(buffer_string, "%[^^\n]%*[\n^]%[^\n]%*[\n]", old_fragment, (unsigned)_countof(old_fragment), new_fragment, (unsigned)_countof(new_fragment));
            replace_fragments(old_fragment, new_fragment, input, output);
        }
        else {
            strcpy_s(old_fragment, MAX_STRING_LEN, buffer_string);
            replace_fragments(old_fragment, "", input, output);
        }

        progress = (int)((double)total_completed_chars / source_len * 100);
        printf("Progress: %d%%\n", progress);
    }
    printf("Progress: %d%%\n", 100);
}

file_status open_file(const char* filePath, const char* mode, FILE** file)
{
    errno_t error;
    if ((error = fopen_s(file, filePath, mode)) != 0) {
        fprintf(stderr, "Unable to open file '%s'\n", filePath);
        return OPEN_ERROR;
    }
    return OPEN_SUCCESS;
}
void parse_string(program_files* files, int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            if (open_file(argv[i + 1], "r", &files->config_file) == OPEN_ERROR) {
                break;
            }
            continue;
        }
        if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input")) {
            if (open_file(argv[i + 1], "r", &files->input_file) == OPEN_ERROR) {
                break;
            }
            continue;
        }
        if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
            if (open_file(argv[i + 1], "w", &files->output_file) == OPEN_ERROR) {
                break;
            }
            continue;
        }
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            show_help();
            continue;
        }
    }
}
void free_all(char* input_string, char* output_string, program_files* files)
{
    free(input_string);
    free(output_string);
    if (files->config_file != NULL) {
        fclose(files->config_file);
    }
    if (files->input_file != NULL) {
        fclose(files->input_file);
    }
    if (files->output_file != NULL) {
        fclose(files->output_file);
    }

}
void print_to_user(FILE* output_file, char* input_string, double time_spent)
{
    printf("Completed in: %.2f seconds\n", time_spent);
    if (output_file == NULL) {
        printf("%s\n", input_string);
    }
    else {
        fprintf(output_file, "%s", input_string);
    }
}
void proverka/*pavapepe gemabody*/(FILE* input_file, char* input_string, char* output_string)
{
    if (input_file == NULL) {
        printf("Invalid input file >:/\n");
        exit(1);
    }
    if (input_string == NULL || output_string == NULL) {
        printf("Failed to allocate memory =(\n");
        exit(1);
    }
    memset(input_string, 0, MAX_MEM_ALLOCATED);
    memset(output_string, 0, MAX_MEM_ALLOCATED);
}
void arg_init/*bubble bath and get my 44 up the dresser now I'm*/(int argc)
{
    if (argc < 2) {
        printf("Use -h or --help to see more information\n");
        show_help();
        exit(1);
    }

}

