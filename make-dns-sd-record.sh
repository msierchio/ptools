#!/bin/sh

#   Copyright (c) 2013 Michael Sierchio
#
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or
#   without modification, are permitted provided that the following
#   conditions are met:
#
#   1.      Redistributions of source code must retain the
#           above copyright notice, this list of conditions and the
#           following disclaimer.
#
#   2.      Redistributions in binary form must reproduce
#           the above copyright notice, this list of conditions and
#           the following disclaimer in the documentation and/or
#           other materials provided with the distribution.
#              
#   THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
#   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#   PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.

PRINTER_NAME=`/bin/echo $1 | sed 's/^\([^.][^.]*\)\..*$/\1/'`
SUBDOMAIN=`/bin/echo $1 | sed 's/^[^.][^.]*\.\(.*\)$/\1/'`

/bin/echo
/bin/echo "^_pdl-datastream._tcp.${SUBDOMAIN}:${PRINTER_NAME}._pdl-datastream._tcp.${SUBDOMAIN}"

/bin/echo -n ":${PRINTER_NAME}._pdl-datastream._tcp.${SUBDOMAIN}:33:\000\000\000\000\043\214"
/bin/echo $1 | awk '{ N=split($1,C,".") ; for (i=1;i<=N; i++) { printf "\\%0.3o%s",  length(C[i]), C[i] } ; printf "\\000" }'
/bin/echo

/bin/echo -n ":${PRINTER_NAME}._pdl-datastream._tcp.${SUBDOMAIN}:16:"
awk '{ printf "\\%0.3o%s", length($0), $0 }' < $1 | sed 's/:/\\072/g'
/bin/echo
