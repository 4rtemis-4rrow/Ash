/*this is file is used to test the ReadDirectory function, which would be used to generate a dynamically sized array of directory contents, used on . for path completions, and on /bin for command completions, this will only be used to source the array, comparing a string to contents of the array would also be simple using a for loop and strncmp, my issue would be with the implementation of pressing tabs to start the autocompletion and cycle between them (better explained in the README file), I wrote the function FindMatches to do specifically that, though it is not called in this program*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void FindMatches(const char* Substring, const char** Entries, char*** Results, int* Size) {
    int InitSize = 10;
    int currentSize = 0;
    *Results = (char**)malloc(InitSize * sizeof(char*));

    for (int i = 0; Entries[i] != NULL; i++) {
        if (strstr(Entries[i], Substring) != NULL) {
            if (currentSize >= InitSize) {
                InitSize *= 2;
                *Results = (char**)realloc(*Results, InitSize * sizeof(char*));
            }
            (*Results)[currentSize] = strdup(Entries[i]);
            currentSize++;
        }
    }

    *Size = currentSize;
}

void ReadDirectory(const char* directory, char*** file_names, size_t* num_files) {
    DIR *dir;
    struct dirent *entry;
    size_t capacity = 10;
    size_t count = 0;

    dir = opendir(directory);
    if (!dir) {
        fprintf(stderr, "Error opening directory: %s\n", directory);
        return;
    }

    char** names = (char**)malloc(capacity * sizeof(char*));
    if (!names) {
        fprintf(stderr, "Memory allocation failed\n");
        closedir(dir);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            names[count] = (char*)malloc(strlen(entry->d_name) + 1);
            if (!names[count]) {
                fprintf(stderr, "Memory allocation failed\n");
                for (size_t i = 0; i < count; i++) {
                    free(names[i]);
                }
                free(names);
                closedir(dir);
                return;
            }
            strcpy(names[count], entry->d_name);
            count++;
            if (count == capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(names, capacity * sizeof(char*));
                if (!temp) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    for (size_t i = 0; i < count; i++) {
                        free(names[i]);
                    }
                    free(names);
                    closedir(dir);
                    return;
                }
                names = temp;
            }
        }
    }
    closedir(dir);
    *file_names = names;
    *num_files = count;
}

int main() {
    char** file_names;
    size_t num_files;
    const char* directory = ".";
    ReadDirectory(directory, &file_names, &num_files);
    for (size_t i = 0; i < num_files; i++) {
        printf("%s\n", file_names[i]);
    }
    for (size_t i = 0; i < num_files; i++) {
        free(file_names[i]);
    }
    free(file_names);

    return 0;
}
