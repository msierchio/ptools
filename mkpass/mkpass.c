/*
 *    Copyright (c) 2019 Michael Sierchio
 *    
 *    All rights reserved.
 *    
 *    Redistribution and use in source and binary forms, with or
 *    without modification, are permitted provided that the
 *    following conditions are met:
 *    
 *    1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 *    
 *    2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *    
 *    THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS
 *    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 *    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT
 *    SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *    OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 *    THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 *    OF SUCH DAMAGE.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/uio.h>

/*
 *  Since some older Linux implementation of /dev/random block, a
 *  conditional compile for Linux has the program use /dev/urandom.
 *  This may reduce the security of passwords.  On FreeBSD and OS
 *  X /dev/random is of a different design, using a 256-bit variant
 *  of Yarrow when no hardware RNG is present, with support for
 *  hardware RNGs if available.
 */

#ifdef __linux__
#define	RANDDEV	"/dev/urandom"
#else
#define	RANDDEV	"/dev/random"
#endif

/*
 *  Theory of operation – emit characters selected randomly from a
 *  charset that is a power of 2 in size. This makes arithmetic fast
 *  and produces random strings without bias. The ten digits, the
 *  letters of the alphabet in upper and lower case, and two extra
 *  characters are used.
 */

static char alf[] = {
    '-', '+',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};


int main(int argc, char *argv[]) {
    int i, n, fd;
    unsigned char   c, rbits[512];

    n = 16;
    if (argc > 1)
        n = atoi(argv[1]);

    if (n < 8)
        n = 8;

    if (n > 512)
        n = 512;

    fd = open(RANDDEV, O_RDONLY);
    if ((ssize_t) n != read(fd, (void *) rbits, (ssize_t) n)) {
        printf("%s\n", strerror(errno));
        exit(errno);
    }

    for (i=0; i<n; i++)
        putchar(alf[rbits[i] % sizeof(alf)/sizeof(alf[0])]);
    putchar('\n');

    return 0;
}

