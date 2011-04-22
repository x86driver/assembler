#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "assembler.h"

#define VAR_LEN 64 /* Max variable length */

void asm_init(struct asm_file **af, const char *file)
{
    *af = malloc(sizeof(struct asm_file));
    (*af)->fp = fopen(file, "r");
}

void asm_destroy(struct asm_file *af)
{
    fclose(af->fp);
}

long check_file_size(const char *file)
{
    struct stat st;
    stat(file, &st);
    return st.st_size;
}

/* strip the following characteristics:
 * - (space)
 * - (tab)
 * - \r\n
 */
void strip_line(char *dst, char *src)
{
    do {
        switch (*src) {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            break;
        default:
            *dst++ = *src;
        }
    } while (*src++);
}

void parse(char *str)
{
    /* label: opcode, op1, op2, op3
     * (1) strip some chars
     * (2) find colon
     * (3) distinguish opcode and operand
     */

    int index = 0;
    char tokbuf[8][VAR_LEN];
    char *delim = " \t";
    char *ptr, *token, *line;

    line = malloc(strlen(str) + 1);
    strip_line(line, str);

    ptr = strchr(line, ':');
    if (ptr != NULL) {
        int offset = ptr - line;
        strncpy(tokbuf[index], line, offset);
        tokbuf[index++][offset] = '\0';
        line += offset + 1;
    }

    token = strtok(line, delim);
    strcpy(tokbuf[index++], token);
    while (token != NULL) {
        token = strtok(NULL, delim);
        strcpy(tokbuf[index++], token);
    }

    free(line);
}

void asm_start(struct asm_file *af)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, af->fp)) != -1) {
        parse(line);
    }
}

int main(int argc, char **argv)
{
    struct asm_file *af = NULL;

    if (argc < 2) {
        printf("Usage: %s [file]\n", argv[0]);
        exit(1);
    }

    asm_init(&af, argv[1]);
    asm_start(af);
    asm_destroy(af);

    return 0;
}
