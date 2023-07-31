#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void executeCommand(char** tokens) {
    pid_t pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (execvp(tokens[0], tokens) == -1) {
            perror("Execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
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
        executeCommand(Tokens);
        free(Tokens);
        Tokens = NULL;
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        free(input);
    }
    return 0;
}
