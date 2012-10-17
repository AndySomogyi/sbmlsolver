#pragma hdrstop
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>

int addThemAll( int numargs, ... )
{
    // RULES you must know in order to be able to use "..." in one of your
    // own functions:
    //
    // 3)  THERE MUST BE AT LEAST ONE MANDATORY, NON-OPTIONAL ARGUMENT,
    //     THAT COMES BEFORE THE ...

    // We'll be using these macros here:
/*
     va_list    va_start    va_end    va_arg
*/

    // All of the above va_* things are actually special MACROS,
    // exclusively defined for us to use when working with
    // _V_ariable _A_rgument lists.

    // FIRST, we create a POINTER that will be used
    // to point to the first element of the VARIABLE
    // ARGUMENT LIST.
    va_list listPointer;

    // Currently, listPointer is UNINITIALIZED, however,
    // SO, now we make listPointer point to
    // the first argument in the list
    va_start( listPointer, numargs );

    // Notice that numargs is the LAST MANDATORY ARGUMENT
    // that the addThemAll() function takes.
    // By "LAST MANDATORY ARGUMENT", I mean 'numargs'
    // is the last argument to the addThemAll() function
    // JUST BEFORE the "..."

    // NEXT, we're going to start to actually retrieve
    // the values from the va_list itself.
    // THERE IS A CATCH HERE.  YOU MUST KNOW THE
    // DATA TYPE OF THE DATA YOU ARE RETRIEVING
    // FROM THE va_list.  In this example, I'm assuming
    // they're all ints, but you could always pass a format
    // string that lets you know the types.

    int sum = 0;
    for( int i = 0 ; i < numargs; i++ )
    {
        // GET an arg.  YOU MUST KNOW
        // THE TYPE OF THE ARG TO RETRIEVE
        // IT FROM THE va_list.
        int arg = va_arg( listPointer, int );

        printf( "    The %dth arg is %d\n", i, arg );

        sum += arg;
    }

    printf("--");
    printf("END OF ARGUMENT LIST\n\n");

    // FINALLY, we clean up by saying
    // va_end().  Don't forget to do this
    // BEFORE the addThemAll() function returns!
    va_end( listPointer );

    printf("The total sum was %d\n\n", sum);
    return sum;
}

int main()
{
    // Try it out.

    printf("Calling 'addThemAll( 3, 104, 29, 46 );' . . .\n");
    addThemAll( 3, 104, 29, 46 );

    printf("Calling 'addThemAll( 8,   1, 2, 3, 4, 5, 6, 7, 8 );' . . .\n");
    addThemAll( 8,   1, 2, 3, 4, 5, 6, 7, 8 );
    return 0;
}

