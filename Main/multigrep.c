/*
- Proyecto 1
- Oswaldo Ramírez - 2020044182
*/

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define REGEXSZ 128 //tamanno maximo de un regex
#define LINESZ 1024  //tamanno maximo de una linea en un archivo
#define NUM_THREADS 5 //numero de hilos en los que va a corrrer el programa

void menu(){
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
        if(!regexec(&regex, line, 0, NULL, 0)){ //se da el caso del regex

            //por cuestiones de estetica recortamos el string si es muy largo
            if (strlen(line) > 50)
                printf("%s\t%d\t%.50s(...)\n",filename,current_line,line);
            else
                printf("%s\t%d\t%s",filename,current_line,line);
        }

        current_line++;
    }

    fclose(file_input);
    printf("\n");
    return 0;
}

int main(int argc, char *argv[]) {
    int i;
    regex_t regex;
    char regex_s[REGEXSZ]; //aqui vamos a guardar el regex
    clock_t start, end; // hora de inicio y finalizacion deel programa
    double execution_time; // para tener el tiempo de ejecucion del programa
    
    menu();
    start = clock();

    // revisamos si hay mas de 2 argumentos (un regex y un archivo como minimo), el argumento 0 es el nombre del programa
    if (argc >= 3) {
        strcpy(regex_s, argv[1]);  // guardamos el regex en un string

        // compilamos el regex y verificamos errores
        if (regcomp(&regex, regex_s, REG_EXTENDED)) {
            printf("No se pudo compilar el regex, verifique los datos ingresados\n");
            exit(1);
        }

        //aqui hay que meter los hilos de ejecucion
        for (int i = 2; i < argc; i++) {
            regex_file(argv[i],regex);
        }

    } else
        printf("Se espera que haya al menos dos argumentos.\n");

    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Tiempo de ejecución en segundos : %f \n", execution_time);

    exit(0);
}
