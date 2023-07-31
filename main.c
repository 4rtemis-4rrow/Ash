#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define INITIAL_SIZE 10
#define TOKEN_DELIMITER " \t\n"

void Tokenize(char* inputString, char*** outputArray, int* numTokens) {
    const char* delimiter = TOKEN_DELIMITER;

    char* token = strtok(inputString, delimiter);
    while (token != NULL) {
        (*numTokens)++;
        *outputArray = realloc(*outputArray, sizeof(char*) * (*numTokens));
        if (*outputArray == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
        (*outputArray)[*numTokens - 1] = strdup(token);

        token = strtok(NULL, delimiter);
    }
}

int main() {
    while (1) {
        char buffer[4096];
        char* input = NULL;
        char** Tokens = NULL;
        int ElemCnt = 0;
        char* cwd = getcwd(buffer, sizeof(buffer));
        printf("%s\n>>>", cwd);
        scanf("%m[^\n]", &input);
        printf("%s\n", input);
        Tokenize(input, &Tokens, &ElemCnt);
        //handle commands here
        free(Tokens);
        Tokens = NULL;
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        free(input);
    }
    return 0;
}
