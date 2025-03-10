#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define ERROR_INPUT 100
#define ERROR_LENGTH 101
#define ERROR_MALLOC 200
#define ERROR_REALLOC 201
#define ERROR_REALLOC_MESSAGE "ERROR_BAD_REALLOC"
#define NORMAL_0UT 0
#define SYMBOLS 52
#define START_SIZE -1

// Global Flags
int FLAG_BAD_INPUT = 0;
int FLAG_BAD_LENGTH = 0;
int FLAG_PRP_OPTIONAL = 0;
int FLAG_BAD_MALLOC = 0;

int CIPHER_SIZE = START_SIZE;
int PART_TEXT_SIZE = START_SIZE;

// Function prototypes
char *input(char *text);
int levenshtein(char *cipher, char *part_text, int size_cipher, int size_part_text);
void output(char *new_text, int len);
char *shift(char *cipher, char *part_text, int cipher_size);
void cipher_calculation(char *cipher, char *part_text, int cipher_size, int part_text_size);

// The main program
int main(int argc, char *argv[]) {
    char *cipher = NULL;
    char *part_text = NULL;

    // Memory allocation for cipher
    if ((cipher = (char *)malloc(sizeof(char))) == NULL) {
        return ERROR_MALLOC;
    }

    // Memory allocation for part_text
    if ((part_text = (char *)malloc(sizeof(char))) == NULL) {
        free(cipher);
        cipher = NULL;
        return ERROR_MALLOC;
    }

    // Input for cipher
    cipher = input(cipher);

    // Check for realloc failure
    if (strcmp(cipher, ERROR_REALLOC_MESSAGE) == 0) {
        free(cipher);
        cipher = NULL;
        free(part_text);
        part_text = NULL;
        return ERROR_REALLOC;
    }

    // Input for part_text
    part_text = input(part_text);

    // Check for realloc failure
    if (strcmp(part_text, ERROR_REALLOC_MESSAGE) == 0) {
        free(cipher);
        cipher = NULL;
        free(part_text);
        part_text = NULL;
        return ERROR_REALLOC;
    }

    // Rules
    if (CIPHER_SIZE != PART_TEXT_SIZE) {
        FLAG_BAD_LENGTH = 1;
    }

    if (argc > 1) {
        if (strcmp(argv[1], "-prp-optional") == 0) {
            FLAG_PRP_OPTIONAL = 1;
            FLAG_BAD_LENGTH = 0;
        }
    }

    // Find answer
    if ((FLAG_BAD_INPUT == 0) && (FLAG_BAD_LENGTH == 0)) {
        cipher_calculation(cipher, part_text, CIPHER_SIZE, PART_TEXT_SIZE);
    }

    // Free allocated memory
    free(cipher);
    cipher = NULL;
    free(part_text);
    part_text = NULL;

    // Check for errors and print corresponding messages
    if (FLAG_BAD_MALLOC) {
        fprintf(stderr, "Error: Bad Malloc!\n");
        return ERROR_MALLOC;
    } else if (FLAG_BAD_INPUT) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return ERROR_INPUT;
    } else if (FLAG_BAD_LENGTH) {
        fprintf(stderr, "Error: Chybna delka vstupu!\n");
        return ERROR_LENGTH;
    } else {
        return NORMAL_0UT;
    }
}

// Function to shift characters in the cipher array
char *shift(char *cipher, char *part_text, int cipher_size) {
    for (int i = 0; i < cipher_size; i++) {
        if (islower(cipher[i])) {
            cipher[i] = cipher[i] + 1;
            if ((cipher[i] > 'z')) {
                cipher[i] = cipher[i] + 'A' - 'z' - 1;
            }
        } else {
            cipher[i] = cipher[i] + 1;
            if ((cipher[i] > 'Z')) {
                cipher[i] = cipher[i] - 'Z' + 'a' - 1;
            }
        }
    }
    return cipher;
}

// Function to take input and validate characters
char *input(char *text) {
    char symbol;
    int length = 0;
    int alloc_size = sizeof(char);

    while ((symbol = getchar()) != EOF && symbol != '\n') {
        if (!isalpha(symbol)) {
            FLAG_BAD_INPUT = 1;
            return text;
        }

        // Reallocate memory if needed
        if (length == alloc_size) {
            if ((text = (char *)realloc(text, alloc_size + 1)) == NULL) {
                return ERROR_REALLOC_MESSAGE;
            }
            alloc_size++;
        }

        // Assign the character to the text array
        text[length++] = symbol;
    }

    // Set sizes if not set
    if (CIPHER_SIZE == START_SIZE) {
        CIPHER_SIZE = length;
    } else {
        PART_TEXT_SIZE = length;
    }

    return text;
}
void cipher_calculation(char *cipher, char *part_text, int cipher_size, int part_text_size){
    char *new_text;
    if((new_text = (char *)malloc(sizeof(char) * cipher_size)) == NULL){
        FLAG_BAD_MALLOC = 1;
    }

    int min = cipher_size;
    int max = 0;
    for(int n = 0; n < SYMBOLS; n++){
        cipher = shift(cipher, part_text, cipher_size);

        if(!FLAG_PRP_OPTIONAL){
            int counter = 0;
            for(int i = 0; i < cipher_size; i++){
                counter += (cipher[i] == part_text[i]);
            }
            if(counter > max){
                max = counter;
                for( int i = 0; i < cipher_size; i++){
                    new_text[i] = cipher[i]; // New array with dechipher
                }
            }
        }
        else{
            int leven = levenshtein(cipher, part_text,cipher_size, part_text_size);
            if(leven < min){
                min = leven;
                for(int i = 0; i < cipher_size; i++){
                    new_text[i] = cipher[i]; // New array with dechipher
                }
            }
        }
    }
    output(new_text, cipher_size);
    free(new_text);
}

int levenshtein(char *cipher, char *part_text, int size_cipher, int size_part_text)
{
    // levenshtein matrix
    // int aux_matrix[size_cipher + 1][size_part_text + 1];
    int **aux_matrix;
    if ((aux_matrix = (int **)malloc(sizeof(int *) * (size_cipher + 1))) == NULL)
    {
        FLAG_BAD_MALLOC = 1;
    }
    for (int i = 0; i < size_cipher + 1; i++)
        if ((aux_matrix[i] = (int *)malloc((size_part_text + 1) * sizeof(int))) == NULL)
            FLAG_BAD_MALLOC = 1;

    for (int i = 0; i <= size_cipher; i++)
        for (int j = 0; j <= size_part_text; j++)
            aux_matrix[i][j] = 0;

    // Nummerize the words
    for (int i = 1; i <= size_cipher; i++)
        aux_matrix[i][0] = i;

    for (int j = 1; j <= size_part_text; j++)
        aux_matrix[0][j] = j;

    // Distance calculation
    for (int i = 1; i <= size_cipher; i++)
    {
        for (int j = 1; j <= size_part_text; j++)
        {
            int substitutionCost = 0;
            if (cipher[i - 1] != part_text[j - 1])
                substitutionCost = 1;
            int del = aux_matrix[i - 1][j] + 1;
            int sub = aux_matrix[i][j - 1] + 1;
            int ins = aux_matrix[i - 1][j - 1] + substitutionCost;
            if (del < ins && del < sub)
            {
                aux_matrix[i][j] = del;
            }
            else if (ins < sub)
            {
                aux_matrix[i][j] = ins;
            }
            else
            {
                aux_matrix[i][j] = sub;
            }
        }
    }
    int levenshtein_num = aux_matrix[size_cipher][size_part_text];
    for (int i = 0; i < size_cipher + 1; i++)
        free(aux_matrix[i]);

    free(aux_matrix);
    return levenshtein_num;
}

void output(char *new_text, int len)
{
    for (int i = 0; i < len; i++)
        printf("%c", new_text[i]);
    printf("\n");
}
