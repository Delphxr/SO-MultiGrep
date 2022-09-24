#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

pthread_mutex_t mutex;

struct num_list {
    int *list;
    int index;
};

struct thread_list {
    struct num_list *list;
    int thread_id;
};

void *PrintHello(void *threadarg) {
    int status;
    struct thread_list *my_data;
    my_data = (struct thread_list *)threadarg;

    int index = my_data->thread_id;

    while (my_data->list->index < 50) {
        

        status = pthread_mutex_lock(&mutex);
        if (my_data->list->index < 50){
        printf("Soy el thread #: %d\tMi numero es: #%d y el index es: %d\n", index, my_data->list->list[my_data->list->index], my_data->list->index);
        my_data->list->index += 1;}
        status = pthread_mutex_unlock(&mutex);
  
    }
    printf("saliendo del thread: %d\n", index);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int return_code;
    int i;
    int status;

    struct num_list list;
    list.index = 0;
    int a[50] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
    list.list = a;

    status = pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < NUM_THREADS; i++) {
        printf("In main: creating thread %d\n", i);
        struct thread_list tl;

        tl.list = &list;
        tl.thread_id = i;
        return_code = pthread_create(&threads[i], NULL, PrintHello, (void *)&tl);
        if (return_code) {
            printf("ERROR; return code from pthread_create() is %d\n", return_code);
            exit(-1);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++)
        status = pthread_join(threads[i], NULL);

    status = pthread_mutex_destroy(&mutex);
    printf("saliendo del programa\n");
    pthread_exit(NULL);
}
