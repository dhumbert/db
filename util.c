#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char * strip_spaces(const char * word)
{
    int l;
    int n;
    const char * word_begins;
    const char * word_ends;

    l = strlen (word);
    n = l;
    word_begins = word;
    while (isspace (*word_begins)) {
        word_begins++;
        n--;
    }
    word_ends = word + l - 1;
    while (isspace (*word_ends)) {
        word_ends--;
        n--;
    }
    if (n == l) {
        return strdup (word);
    }
    else {
        char * copy;
        int i;

        copy = malloc (n + sizeof ((char) '\0'));
        if (! copy) {
            fprintf (stderr, "Out of memory.\n");
            exit (EXIT_FAILURE);
        }
        for (i = 0; i < n; i++) {
            copy[i] = word_begins[i];
        }
        copy[n] = '\0';
        return copy;
    }
}