#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//defined constants to easily change the probabilty as well as the interval and extension amt defined by prompt
#define MUTATION_PROB 0.13
#define EXTEND_INTERVAL 500
#define EXTEND_AMOUNT 10

// Mutate Function: Mutate the input buffer by randomly changing each byte with the probabilty set by MUTATION_PROB
void mutate(unsigned char *input, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if ((double)rand() / RAND_MAX < MUTATION_PROB) {
            input[i] = (unsigned char)rand();
        }
    }
}

//Main Function:
int main(int argc, char **argv)
{
    // Parse command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s prng_seed num_iterations\n", argv[0]);
        return 1;
    }
    unsigned int prng_seed = atoi(argv[1]);
    unsigned int num_iterations = atoi(argv[2]);

    // Pass seed to random number generator
    srand(prng_seed);

    // Read in the initial seed file and save to seed_data and seed size
    FILE *seed_file = fopen("seed", "rb");
    if (!seed_file) {
        perror("Failed to open seed file");
        return 1;
    }
    fseek(seed_file, 0, SEEK_END);
    size_t seed_size = ftell(seed_file);
    fseek(seed_file, 0, SEEK_SET);
    unsigned char *seed_data = malloc(seed_size);
    if (!seed_data) {
        perror("Failed to allocate memory");
        fclose(seed_file);
        return 1;
    }
    fread(seed_data, 1, seed_size, seed_file);
    fclose(seed_file);

    // Mutate the input `num_iterations` times
    for (unsigned int i = 0; i < num_iterations; i++) {
        mutate(seed_data, seed_size);
        // Extend the input by adding 10 random characters every 500 iterations
        if (i % EXTEND_INTERVAL == 0) {
            seed_data = realloc(seed_data, seed_size + EXTEND_AMOUNT);
            for (size_t j = 0; j < EXTEND_AMOUNT; j++) {
                seed_data[seed_size + j] = (unsigned char)rand();
            }
            seed_size += EXTEND_AMOUNT;
        }
    }

    // Write the mutated input to stdout
    fwrite(seed_data, 1, seed_size, stdout);

    // Free the memory allocated for the input buffer
    free(seed_data);

    return 0;
}

