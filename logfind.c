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

const char *CONFIG_FILE_NAME = ".logfind";
const unsigned int MAX_FILENAME_LENGTH = 256;
const unsigned int MAX_LOGFILE_LINE_LENGTH = 500;
const unsigned int MAX_FILES = 100;

void trim_trailing(char *string);
void remove_delimiters(char *string);

int main(int argc, char *argv[]) {
    FILE *config_fp = NULL;
    FILE *file_to_search = NULL;
    char config_buffer[MAX_FILENAME_LENGTH];
    char log_buffer[MAX_LOGFILE_LINE_LENGTH];
    char log_files[MAX_FILES][MAX_FILENAME_LENGTH];
    unsigned int num_files = 0;

    // Check for existence of config file.
    if (access(CONFIG_FILE_NAME, F_OK) != 0) {
        log_error("Error reading config file.");
        goto error;
    }

    config_fp = fopen(CONFIG_FILE_NAME, "r");
    check(config_fp, "Could not open config file.");

    // Read the configuration file and get the filenames from it.
    while(fgets(config_buffer, MAX_FILENAME_LENGTH, config_fp) && num_files < MAX_FILES) {
        trim_trailing(config_buffer);
        if (access(config_buffer, F_OK) != 0) {
            log_error("Error reading log file %s.", config_buffer);
            goto error;
        }

        // Put the filenames into an array to iterate over.
        strcpy(log_files[num_files], config_buffer);
        num_files++;
    } 

    for (unsigned int i = 0; i < num_files; i++) {
        file_to_search = fopen(log_files[i], "r");
        check(file_to_search, "Could not open log file.");

        while(fgets(log_buffer, MAX_LOGFILE_LINE_LENGTH, file_to_search)) {
            remove_delimiters(log_buffer);
            printf("%s\n", log_buffer);
        }
    }

    if (config_fp) fclose(config_fp);

    return 0;
error:

    if (config_fp) fclose(config_fp);
    return -1;
}

/**
* Remove trailing newline from string.
*/
void trim_trailing(char *string) {
    size_t length = strlen(string);
    if (string[length - 1] == '\n') string[length - 1] = '\0';
}

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
