#include "options.h"
#include <getopt.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

static int TempFlag;
static const char ShortOptions[] = "abc:d:f:";
static struct option LongOptions[] =
{
    /* These options set a flag. */
    {"verbose", no_argument, &TempFlag, 1},
    {"brief",   no_argument, &TempFlag, 0},

    /* These options don't set a flag.
       We distinguish them by their indices. */
    {"add",     no_argument,       0, 'a'},
    {"append",  no_argument,       0, 'b'},
    {"delete",  required_argument, 0, 'd'},
    {"create",  required_argument, 0, 'c'},
    {"file",    required_argument, 0, 'f'},
    {0, 0, 0, 0}
};

static void opts_handle_opt();

void opts_parse(int argc, char** argv)
{
    int short_opt = 0;

    while(1)
    {
        int idx = 0;
        short_opt = getopt_long(argc, argv, ShortOptions, LongOptions, &idx);
        if(short_opt == -1) break;
        opts_handle_opt();
        switch (short_opt)
             {
             case 0:
               /* If this option set a flag, do nothing else now. */
               if (LongOptions[idx].flag != 0)
                 break;
               printf ("option %s", LongOptions[idx].name);
               if (optarg)
                 printf (" with arg %s", optarg);
               printf ("\n");
               break;

             case 'a':
               puts ("option -a\n");
               break;

             case 'b':
               puts ("option -b\n");
               break;

             case 'c':
               printf ("option -c with value `%s'\n", optarg);
               break;

             case 'd':
               printf ("option -d with value `%s'\n", optarg);
               break;

             case 'f':
               printf ("option -f with value `%s'\n", optarg);
               break;

             case '?':
               /* getopt_long already printed an error message. */
               break;

             default:
               abort ();
             }
    }

    opts_print_usage();
}

static void opts_handle_opt()
{
}

void opts_print_usage()
{
    int idx = 0;
    while(1)
    {
        const char* name = LongOptions[idx].name;

        // Breakout if we've reached the end of the list
        if( name == 0) break;

        if(strlen(name) > 0)
            printf("--%s\n", name);

        idx++;
    }
}

