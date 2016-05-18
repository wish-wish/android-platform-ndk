/*
 * Copyright (c) 2011-2016 CrystaX.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY CrystaX ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL CrystaX OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of CrystaX.
 */

/*
 * Copyright (C) 2013 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <inttypes.h>
#include <ctype.h>
#include <time.h>

static inline int digitval(int ch)
{
    unsigned d;

    d = (unsigned)(ch - '0');
    if (d < 10) return (int)d;

    d = (unsigned)(ch - 'a');
    if (d < 6) return (int)(d+10);

    d = (unsigned)(ch - 'A');
    if (d < 6) return (int)(d+10);

    return -1;
}

uintmax_t strntoumax(const char *nptr, char **endptr, int base, size_t n)
{
    const unsigned char*  p   = (const unsigned char *)nptr;
    const unsigned char*  end = p + n;
    int                   minus = 0;
    uintmax_t             v = 0;
    int                   d;

    while (p < end && isspace(*p)) {
        p++;
    }

    if (p < end) {
        char c = p[0];
        if (c == '-' || c == '+') {
            minus = (c == '-');
            p++;
        }
    }

    if (base == 0) {
        if (p+2 < end && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
            p += 2;
            base = 16;
        } else if (p+1 < end && p[0] == '0') {
            p   += 1;
            base = 8;
        } else {
            base = 10;
        }
    } else if (base == 16) {
        if (p+2 < end && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
            p += 2;
        }
    }

    while (p < end && (d = digitval(*p)) >= 0 && d < base) {
        v = v*base + d;
        p += 1;
    }

    if (endptr) {
        *endptr = (char*) p;
    }

    return minus ? -v : v;
}

intmax_t strntoimax(const char* nptr, char** endptr, int base, size_t n)
{
    return (intmax_t) strntoumax(nptr, endptr, base, n);
}

char* strtotimeval(const char* str, struct timeval* ts)
{
    char* s;
    ts->tv_sec = strtoumax(str, &s, 10);

    long fractional_seconds = 0;
    if (*s == '.') {
        s++;
        int count = 0;

        // Read up to 6 digits (microseconds).
        while (*s && isdigit(*s)) {
            if (++count < 7) {
                fractional_seconds = fractional_seconds*10 + (*s - '0');
            }
            s++;
        }

        for (; count < 6; count++) {
            fractional_seconds *= 10;
        }
    }

    ts->tv_usec = fractional_seconds;
    return s;
}
