/**
* A tool called logfind that lets you search through log files for text.
* This tool is a specialized version of another tool called grep, but designed
* only for log files on a system. The idea is that you can type:
*
* logfind zedshaw
*
* And, it will search all the common places that log files are stored, and print
* out every file that has the word “zedshaw” in it.
* The logfind tool should have these basic features:
*
* 1. This tool takes any sequence of words and assumes I mean “and” for them.
* So logfind zedshaw smart guy will find all files that have zedshaw and smart
* and guy in them.
*
* 2. It takes an optional argument of -o if the parameters are meant to be or
* logic.
*
* 3. It loads the list of allowed log files from ~/.logfind.
*
* 4. The list of file names can be anything that the glob function allows.
* Refer to man 3 glob to see how this works. I suggest starting with just a
* flat list of exact files, and then add glob functionality.
*
* 5. You should output the matching lines as you scan, and try to match them as
* fast as possible.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dbg.h"
#include <string.h>
#include <stdbool.h>

const char *CONFIG_FILENAME = ".logfind";
const int MAX_FILENAME_LENGTH = 256;
const int MAX_LINE_LENGTH = 500; // Max characters to read per logfile line.
const int MAX_SEARCH_FILES = 100; // Max files to search.
const int MAX_WORDS = 10; // Max words to search.
const int MAX_WORD_LEN = 25; // Max search word length.

void trim_trailing(char *string);
void remove_delimiters(char *string);
void search(FILE *file, char search_words[MAX_WORDS][MAX_WORD_LEN], int num_search_words);
void search_or(FILE *file);
void fill_search_words(char search_words[MAX_WORDS][MAX_WORD_LEN], char *argv[], int start_index, int length);

int main(int argc, char *argv[]) {
    if (argc < 2 || (argc == 2 && strcmp(argv[1], "-o") == 0)) {
        printf("USAGE: logfind -o [search words]\n");

        exit(1);
    }

    bool or_search = false;
    char search_words[MAX_WORDS][MAX_WORD_LEN];
    int num_search_words;

    // Add search words from argv to search_words.
    if (strcmp(argv[1], "-o") == 0) {
        or_search = true;
        num_search_words = argc - 2;
        // get search words starting at argv[2] and go until argc - 2.
        fill_search_words(search_words, argv, 2, num_search_words);
    } else {
        num_search_words = argc - 1;
        // get search words starting at argv[1] and go until argc - 1.
        fill_search_words(search_words, argv, 1, num_search_words);
    }

    FILE *config_fp = NULL;
    FILE *file_to_search = NULL;

    char config_buffer[MAX_FILENAME_LENGTH];
    char log_files[MAX_SEARCH_FILES][MAX_FILENAME_LENGTH];

    int num_files = 0;

    // Check for existence of config file.
    if (access(CONFIG_FILENAME, F_OK) != 0) {
        log_error("Error reading config file.");
        goto error;
    }

    config_fp = fopen(CONFIG_FILENAME, "r");
    check(config_fp, "Could not open config file.");

    // Read the configuration file and get the filenames to search from it.
    while(fgets(config_buffer, MAX_FILENAME_LENGTH, config_fp) && num_files < MAX_SEARCH_FILES) {
        trim_trailing(config_buffer);
        if (access(config_buffer, F_OK) != 0) {
            log_error("Error reading log file %s.", config_buffer);
            goto error;
        }

        // Put the filenames into the log_files array to iterate over.
        strcpy(log_files[num_files], config_buffer);
        num_files++;
    } 

    for (int i = 0; i < num_files; i++) {
        file_to_search = fopen(log_files[i], "r");
        check(file_to_search, "Could not open log file.");

        search(file_to_search, search_words, num_search_words);
    }

    if (config_fp) fclose(config_fp);
    if (file_to_search) fclose(file_to_search);

    return 0;
error:

    if (config_fp) fclose(config_fp);
    if (file_to_search) fclose(file_to_search);
    return -1;
}

/**
* Remove trailing newline from string.
*/
void trim_trailing(char *string) {
    size_t length = strlen(string);
    if (string[length - 1] == '\n') string[length - 1] = '\0';
}

/**
* Removes delimiting characters from string.
*/
void remove_delimiters(char *string) {
    size_t length = strlen(string);
    for (int i = 0; i < (int)length; i++) {
        if (string[i] == '-' ||
            string[i] == ':' ||
            string[i] == '.' ||
            string[i] == '<' ||
            string[i] == '>' ||
            string[i] == '/') {
            string[i] = ' ';
        }
    }
}

/**
* Populate the search_words array.
*/
void fill_search_words(char search_words[MAX_WORDS][MAX_WORD_LEN], char *argv[], int start_index, int length) {
    int i = 0;
    int j = start_index;

    for (; i < length; i++, j++) {
        strcpy(search_words[i], argv[j]);
    }
}

/*
* Search file for all search_words.
*/
void search(FILE *file, char search_words[MAX_WORDS][MAX_WORD_LEN], int num_search_words) {
    char log_buffer[MAX_LINE_LENGTH];
    char log_buffer_clean[MAX_LINE_LENGTH];
    char *token_ptr;
    int i;

    while(fgets(log_buffer, MAX_LINE_LENGTH, file)) {
        strcpy(log_buffer_clean, log_buffer);
        remove_delimiters(log_buffer_clean);

        token_ptr = strtok(log_buffer_clean, " ");
        while(token_ptr != NULL) {
            for (i = 0; i < num_search_words; i++) {
                if (strcmp(token_ptr, search_words[i]) == 0) {
                    printf("%s\n", log_buffer);
                }
            }

            token_ptr = strtok(NULL, " ");
        }
    }
}
