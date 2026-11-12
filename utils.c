#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lms.h"

void read_line(const char *prompt, char *buf, int size)
{
    size_t n;

    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buf, size, stdin) == NULL) {
        printf("\nInput closed. Exiting.\n");
        exit(0);
    }

    n = strcspn(buf, "\r\n");
    if (buf[n] == '\0' && n == (size_t)size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    buf[n] = '\0';

    for (char *p = buf; *p; p++)
        if (*p == '|')
            *p = '/';
}

int read_int(const char *prompt, int min, int max)
{
    char buf[32];
    char *end;
    long v;

    for (;;) {
        read_line(prompt, buf, sizeof buf);
        v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && v >= min && v <= max)
            return (int)v;
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
    }
}

int to_lower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}

int to_upper(int c)
{
    if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');
    return c;
}

int str_contains_ci(const char *text, const char *part)
{
    if (*part == '\0')
        return 1;

    for (; *text; text++) {
        const char *t = text, *p = part;
        while (*t && *p &&
               to_lower((unsigned char)*t) == to_lower((unsigned char)*p)) {
            t++;
            p++;
        }
        if (*p == '\0')
            return 1;
    }
    return 0;
}

int str_equals_ci(const char *a, const char *b)
{
    while (*a && *b) {
        if (to_lower((unsigned char)*a) != to_lower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}
