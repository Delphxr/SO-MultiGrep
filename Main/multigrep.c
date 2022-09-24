/*
- Proyecto 1
- Oswaldo Ramírez - 2020044182
*/
#include <pthread.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define REGEXSZ 128    // tamanno maximo de un regex
#define LINESZ 1024    // tamanno maximo de una linea en un archivo
#define NUM_THREADS 3  // numero de hilos en los que va a correr el programa

pthread_mutex_t mutex;

struct thread_list {
    char **files;  // array con nombres  archivos por analizar
    int size;      // tamaño del array de archivos
    int index;     // para navegar la lista
    int thread_id;
    regex_t regex; 
};

void menu() {
    printf("+---------------------------------------------+\n");
    printf("|  __  __       _ _   _  ____                 |\n");
    printf("| |  \\/  |_   _| | |_(_)/ ___|_ __ ___ _ __   |\n");
    printf("| | |\\/| | | | | | __| | |  _| '__/ _ \\ '_ \\  |\n");
    printf("| | |  | | |_| | | |_| | |_| | | |  __/ |_) | |\n");
    printf("| |_|  |_|\\__,_|_|\\__|_|\\____|_|  \\___| .__/  |\n");
    printf("|                                     |_|     |\n");
    printf("+---------------------------------------------+\n");
}

// recibe el nombre de un archivo y un regex para ejecutarlo
int regex_file(char filename[], regex_t regex) {
    FILE *file_input;  // variable para archivo
    char line[LINESZ];
    int current_line = 1;

    file_input = fopen(filename, "r");  // abrimos el archivo

    if (file_input == NULL) {
        printf("! No se pudo abrir el archivo: %s correctamente, compruebe que exista o que hayan los permisos necesarios\n", filename);
        return -1;
    }

    while (fgets(line, LINESZ, file_input) != NULL) {
        if (!regexec(&regex, line, 0, NULL, 0)) {  // se da el caso del regex

            // por cuestiones de estetica recortamos el string si es muy largo
            if (strlen(line) > 50)
                printf("%s\t%d\t%.50s(...)\n", filename, current_line, line);
            else
                printf("%s\t%d\t%s", filename, current_line, line);
        }

        current_line++;
    }

    fclose(file_input);
    //printf("\n");
    return 0;
}

void *thread_func(void *threadarg) {
    int status;   // status para el mutex
    int current;  // el index del que vamos a analizar
    struct thread_list *my_data;
    my_data = (struct thread_list *)threadarg;

    int thread_id = my_data->thread_id;  // guardamos nuestro id

    while (my_data->index < my_data->size) {
        status = pthread_mutex_lock(&mutex);
        if (my_data->index < my_data->size) {  // hacemos una segunda verificación en caso de que una condicion de carrera aumente el index en medio ciclo
            current = my_data->index;
            my_data->index += 1;
        }
        status = pthread_mutex_unlock(&mutex);

        //printf("Soy el thread #: %d\tMi archivo es: #%s\n", thread_id, my_data->files[current]);
        int return_value = regex_file(my_data->files[current], my_data->regex);  // le hacemoss el regex al archivo

    }
    //printf("Saliendo del thread: %d\n", thread_id);
    pthread_exit(NULL);
}

// recibe el nombre de un archivo y hace un backup del mismo
int log_time(double time) {
    FILE *logs;  // variables para los archivos


    logs = fopen("logs.txt", "a");

    fprintf(logs,"%f\n",time);

    fclose(logs);
    return 0;
}



int main(int argc, char *argv[]) {

    pthread_t threads[NUM_THREADS];
    int i;
    int return_code;
    regex_t regex;
    char regex_s[REGEXSZ];  // aqui vamos a guardar el regex
    clock_t start, end;     // hora de inicio y finalizacion deel programa
    double execution_time;  // para tener el tiempo de ejecucion del programa
    int status;             // para el mutex

    menu();
    start = clock();

    // revisamos si hay mas de 2 argumentos (un regex y un archivo como minimo), el argumento 0 es el nombre del programa
    if (argc >= 3) {
        strcpy(regex_s, argv[1]);           // guardamos el regex en un string
        struct thread_list lista_archivos;  // struct que vamos a usar de argumento
        lista_archivos.size = argc - 2;     // tamaño de la lista de archivos, restamos 2 por el 0 y el regex
        lista_archivos.index = 0;
        lista_archivos.files = malloc(sizeof(lista_archivos.files) * lista_archivos.size);

        for (int i = 2; i < argc; i++) {  // metemos los archivos a la lista
            lista_archivos.files[i - 2] = argv[i];
        }

        // compilamos el regex y verificamos errores
        if (regcomp(&regex, regex_s, REG_EXTENDED)) {
            printf("No se pudo compilar el regex, verifique los datos ingresados\n");
            exit(1);
        }
        lista_archivos.regex = regex;

        status = pthread_mutex_init(&mutex, NULL);
        for (i = 0; i < NUM_THREADS; i++) {  // creamos los threads
            //printf("Creando el thread %d\n", i);
            lista_archivos.thread_id = i;
            return_code = pthread_create(&threads[i], NULL, thread_func, (void *)&lista_archivos);
            if (return_code) {
                printf("ERROR; return code from pthread_create() is %d\n", return_code);
                exit(-1);
            }
        }
        for (int i = 0; i < NUM_THREADS; i++)  // para esperar a que todos los threads terminen antes de terminar el programa
            status = pthread_join(threads[i], NULL);

        status = pthread_mutex_destroy(&mutex);
        

        // aqui hay que meter los hilos de ejecucion
        /*
        for (int i = 2; i < argc; i++) {
            regex_file(argv[i], regex);
        }*/

    } else
        printf("Se espera que haya al menos dos argumentos.\n");

    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\nTiempo de ejecución en segundos : %f \n", execution_time);
    log_time(execution_time);
    pthread_exit(NULL);
    exit(0);
}
