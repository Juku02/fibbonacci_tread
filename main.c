#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct shared_data
{
    unsigned int running;
    unsigned int input;
    unsigned int output;
};

unsigned int fibonacci_number(unsigned int n)
{
    switch (n)
    {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return fibonacci_number(n - 1) + fibonacci_number(n - 2);
    }
}

void *fibonacci_thread(void *vargp)
{
    struct shared_data *shared_ptr = (struct shared_data *)vargp;

    shared_ptr->output = fibonacci_number(shared_ptr->input);
    shared_ptr->running = 0;
    return NULL;
}

void *progress_bar(void *vargp)
{
    struct shared_data *shared_ptr = (struct shared_data *)vargp;

    while (shared_ptr->running)
    {
        printf(". ");
        fflush(stdout);
        sleep(0.200);
    }
    return NULL;
}

void input_prompt(const char *prompt, unsigned int* n_ptr)
{
    int n;
    printf("%s", prompt);
    scanf("%d", n_ptr);
}

int main(int argc, char const *argv[])
{
    pthread_t fibonacci_t, progress_t;
    struct shared_data shared_ptr;

    argc == 1 ? input_prompt("Enter n: ", &shared_ptr.input) : sscanf(argv[1], "%d", &shared_ptr.input);

    shared_ptr.running = 1;

    pthread_create(&fibonacci_t, NULL, fibonacci_thread, (void *)&shared_ptr);
    pthread_create(&progress_t, NULL, progress_bar, (void *)&shared_ptr);

    pthread_join(fibonacci_t, NULL);
    pthread_join(progress_t, NULL);

    printf("%d\n", shared_ptr.output);
    fflush(stdout);

    pthread_exit(NULL);
    return 0;
}
