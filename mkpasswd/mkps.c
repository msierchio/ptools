#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sz(c) (sizeof(c)/sizeof(char)-1)

static char alfabet[] = "ABCDEFGH$JKLMN%PQRSTUVWXYZabcdefghijk*mnopqrstuvwxyz+?23456789@#";

int main ( int argc, char *argv[] ) {
    unsigned char   buf[1032];
    size_t  n = 0, count = 0;
    int     i;
    FILE    *rd;

    count = (argc > 1) ? atoi(argv[1]) : 16;

    if (count < 8)
        count = 8;
    if (count > 1024)
        count = 1024;

    rd = fopen("/dev/urandom", "r");

    while (n < count)
        n += fread(buf+n, sizeof(unsigned char), count-n, rd);

    while (count)
        putchar(alfabet[buf[--count]%sz(alfabet)]);

    putchar('\n');
}

