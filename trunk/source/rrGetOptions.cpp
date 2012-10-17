#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <stdio.h>
#include <string.h>
#include "rrGetOptions.h"
//---------------------------------------------------------------------------
#if defined(__CODEGEAR__)
#pragma package(smart_init)
#endif

namespace rr
{

char    *optarg;        // global argument pointer
int      optind = 0;     // global argv index

int GetOptions(int argc, char *argv[], char *optstring)
{
    static char *next = NULL;
    if (optind == 0)
        next = NULL;

    optarg = NULL;

    if (next == NULL || *next == wchar_t('\0'))
    {
        if (optind == 0)
            optind++;

        if (optind >= argc || argv[optind][0] != wchar_t('-') || argv[optind][1] == wchar_t('\0'))
        {
            optarg = NULL;
            if (optind < argc)
                optarg = argv[optind];
            return EOF;
        }

        if (strcmp(argv[optind], ("--")) == 0)
        {
            optind++;
            optarg = NULL;
            if (optind < argc)
                optarg = argv[optind];
            return EOF;
        }

        next = argv[optind];
        next++;        // skip past -
        optind++;
    }

    char c = *next++;
    char *cp = strchr(optstring, c);

    if (cp == NULL || c == wchar_t(':'))
        return wchar_t('?');

    cp++;
    if (*cp == wchar_t(':'))
    {
        if (*next != wchar_t('\0'))
        {
            optarg = next;
            next = NULL;
        }
        else if (optind < argc)
        {
            optarg = argv[optind];
            optind++;
        }
        else
        {
            return wchar_t('?');
        }
    }

    return c;
}

}//namepsace
