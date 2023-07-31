#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define INITIAL_SIZE 10
#define TOKEN_DELIMITER " \t\n"

void Tokenize(char* inputString, char*** outputArray, int* numTokens) {
    const char* delimiter = TOKEN_DELIMITER;
    char* token = strtok(inputString, delimiter);
    while (token != NULL) {
        if (strcmp(token, "|") == 0) {
            (*numTokens)++;
            *outputArray = realloc(*outputArray, sizeof(char*) * (*numTokens));
            if (*outputArray == NULL) {
                fprintf(stderr, "Memory allocation failed!\n");
                exit(EXIT_FAILURE);
            }
            (*outputArray)[*numTokens - 1] = strdup(token);
        } else if (token[0] == '"' && token[strlen(token) - 1] == '"') {
            int len = strlen(token);
            token[len - 1] = '\0'; // Remove the last double quote
            (*numTokens)++;
            *outputArray = realloc(*outputArray, sizeof(char*) * (*numTokens));
            if (*outputArray == NULL) {
                fprintf(stderr, "Memory allocation failed!\n");
                exit(EXIT_FAILURE);
            }
            (*outputArray)[*numTokens - 1] = strdup(&token[1]); // Skip the first double quote
        } else {
            // Split other tokens using the regular delimiter
            (*numTokens)++;
            *outputArray = realloc(*outputArray, sizeof(char*) * (*numTokens));
            if (*outputArray == NULL) {
                fprintf(stderr, "Memory allocation failed!\n");
                exit(EXIT_FAILURE);
            }
            (*outputArray)[*numTokens - 1] = strdup(token);
        }
        token = strtok(NULL, delimiter);
    }
    *outputArray = realloc(*outputArray, sizeof(char*) * (*numTokens + 1));
    if (*outputArray == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    (*outputArray)[*numTokens] = NULL;
}

void executeCommand(char** tokens, int numTokens) {
    int hasPipe = 0;
    int pipeIndex = -1;
    int hasOutputRedirection = 0;
    char* outputFileName = NULL;
    for (int i = 0; i < numTokens; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            hasPipe = 1;
            pipeIndex = i;
            break;
        }
        if (strcmp(tokens[i], ">") == 0) {
            hasOutputRedirection = 1;
            outputFileName = tokens[i + 1];
            break;
        }
    }

    if (hasPipe) {
        // Handle commands with pipe
        char** firstCommand = tokens;
        char** secondCommand = &tokens[pipeIndex + 1];
        tokens[pipeIndex] = NULL;
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }

        pid_t pid1 = fork();
        if (pid1 == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            if (execvp(firstCommand[0], firstCommand) == -1) {
                perror("Execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid1 > 0) {
            // Parent process
            pid_t pid2 = fork();
            if (pid2 == 0) {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                if (execvp(secondCommand[0], secondCommand) == -1) {
                    perror("Execvp failed");
                    exit(EXIT_FAILURE);
                }
            } else if (pid2 > 0) {
                // Parent process
                close(pipefd[0]);
                close(pipefd[1]);
                // Wait for both child processes to finish
                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            } else {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    } else if (hasOutputRedirection) {
        int outputFd = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outputFd == -1) {
            perror("Output file open failed");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == 0) {
            dup2(outputFd, STDOUT_FILENO);
            close(outputFd);
            if (execvp(tokens[0], tokens) == -1) {
                perror("Execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) {
            // Parent process
            close(outputFd);
            waitpid(pid, NULL, 0);
        } else {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(tokens[0], tokens) == -1) {
                perror("Execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        } else {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    while (1) {
        char buffer[4096];
        char* input = NULL;
        char** Tokens = NULL;
        int ElemCnt = 0;
        char* cwd = getcwd(buffer, sizeof(buffer));
        printf("%s@Ash:%s$ ", getlogin(), cwd);
        scanf("%m[^\n]", &input);
        Tokenize(input, &Tokens, &ElemCnt);
        if (ElemCnt > 0) {
            if (strcmp(Tokens[0], "exit") == 0 || strcmp(Tokens[0], "quit") == 0) {
                printf("Exiting Ash\n");
                exit(EXIT_SUCCESS);
            } else {
                executeCommand(Tokens, ElemCnt);
            }
        }
        free(Tokens);
        Tokens = NULL;
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        free(input);
    }
    return 0;
}
