#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("argc must be 5. Your argc: %d\n", argc);
        return EXIT_FAILURE;
    }
    
    int i;

    int num_arr[4];

    for (i = 1; i < 5; i++)
    {
        num_arr[i - 1] = atoi(argv[i]);
    }

    for (int i = 0; i < argc; i++) {
    printf("Argument %d: %s\n", i, argv[i]);
    }


    int fibo_result = fibonacci(num_arr[0]);
    int max_val_result = max_of_four_int(num_arr[0], num_arr[1], num_arr[2], num_arr[3]);

    // show values
    printf("%d %d\n", fibo_result, max_val_result);
    return EXIT_SUCCESS;
}
