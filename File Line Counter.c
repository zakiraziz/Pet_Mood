#include <stdio.h>
#include <ctype.h>
#include <string.h>

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [options] [file...]\n", program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -l       Count lines (default)\n");
    fprintf(stderr, "  -w       Count words\n");
    fprintf(stderr, "  -c       Count bytes\n");
    fprintf(stderr, "  -L       Count longest line length\n");
    fprintf(stderr, "  -h       Show this help\n");
    fprintf(stderr, "If no file is specified, reads from stdin\n");
}

void count_file(FILE *f, int *lines, int *words, int *bytes, int *max_line) {
    int c, in_word = 0;
    int line_length = 0;
    
    *lines = *words = *bytes = *max_line = 0;
    
    while ((c = fgetc(f)) != EOF) {
        (*bytes)++;
        
        if (c == '\n') {
            (*lines)++;
            if (line_length > *max_line) *max_line = line_length;
            line_length = 0;
        } else {
            line_length++;
        }
        
        if (isspace(c)) {
            if (in_word) {
                in_word = 0;
                (*words)++;
            }
        } else {
            in_word = 1;
        }
    }
    
    // Handle last line if no trailing newline
    if (line_length > 0) {
        if (line_length > *max_line) *max_line = line_length;
        if (in_word) (*words)++;
    }
}

int main(int argc, char *argv[]) {
    int opt_lines = 0, opt_words = 0, opt_bytes = 0, opt_maxline = 0;
    int file_start = 1;
    
    // Parse options
    if (argc == 1) {
        // No arguments, read from stdin and just count lines
        opt_lines = 1;
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                // Handle options
                if (strcmp(argv[i], "-l") == 0) opt_lines = 1;
                else if (strcmp(argv[i], "-w") == 0) opt_words = 1;
                else if (strcmp(argv[i], "-c") == 0) opt_bytes = 1;
                else if (strcmp(argv[i], "-L") == 0) opt_maxline = 1;
                else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                    print_usage(argv[0]);
                    return 0;
                } else {
                    fprintf(stderr, "Unknown option: %s\n", argv[i]);
                    print_usage(argv[0]);
                    return 1;
                }
                file_start++;
            } else {
                break;
            }
        }
        
        // If no options specified, default to line count
        if (!opt_lines && !opt_words && !opt_bytes && !opt_maxline) {
            opt_lines = 1;
        }
    }
    
    int total_lines = 0, total_words = 0, total_bytes = 0, total_maxline = 0;
    int file_count = 0;
    
    // Process files
    if (file_start >= argc) {
        // Read from stdin
        int lines, words, bytes, max_line;
        count_file(stdin, &lines, &words, &bytes, &max_line);
        
        if (opt_lines) printf("%7d ", lines);
        if (opt_words) printf("%7d ", words);
        if (opt_bytes) printf("%7d ", bytes);
        if (opt_maxline) printf("%7d ", max_line);
        printf("\n");
    } else {
        for (int i = file_start; i < argc; i++) {
            FILE *f = fopen(argv[i], "r");
            if (!f) {
                fprintf(stderr, "Error: Cannot open file '%s'\n", argv[i]);
                continue;
            }
            
            int lines, words, bytes, max_line;
            count_file(f, &lines, &words, &bytes, &max_line);
            fclose(f);
            
            if (opt_lines) printf("%7d ", lines);
            if (opt_words) printf("%7d ", words);
            if (opt_bytes) printf("%7d ", bytes);
            if (opt_maxline) printf("%7d ", max_line);
            printf("%s\n", argv[i]);
            
            total_lines += lines;
            total_words += words;
            total_bytes += bytes;
            if (max_line > total_maxline) total_maxline = max_line;
            file_count++;
        }
        
        // Print total for multiple files
        if (file_count > 1) {
            if (opt_lines) printf("%7d ", total_lines);
            if (opt_words) printf("%7d ", total_words);
            if (opt_bytes) printf("%7d ", total_bytes);
            if (opt_maxline) printf("%7d ", total_maxline);
            printf("total\n");
        }
    }
    
    return 0;
}