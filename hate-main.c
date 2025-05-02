
#include <stdio.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    int64_t input = 0xabcd;
    printf("lol: %" PRIu64 "\n", input);
    printf("lol: %" PRIX64 "\n", input);
}

