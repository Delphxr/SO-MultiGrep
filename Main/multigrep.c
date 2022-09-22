/*
- Proyecto 1
- Oswaldo Ramírez - 2020044182
*
*/

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGEXSZ 128
#define LINESZ 1024

// recibe el nombre de un archivo y un regex para ejecutarlo
int regex_file(char filename[], regex_t regex) {
    FILE *file_input;  // variable para archivo
    char line[LINESZ];
    int current_line = 1;

    file_input = fopen(filename, "r");  // abrimos el archivo

    if (file_input == NULL) {
        printf("! No se pudo abrir el archivo: %s correctamente, compruebe que exista o que hayan los permisos necesarios\n", filename);
        fclose(file_input);
        return -1;
    }

    while (fgets(line, LINESZ, file_input) != NULL) {
        if(!regexec(&regex, line, 0, NULL, 0)){ //se da el caso

            //por cuestiones de estetica recortamos el string si es muy largo
            if (strlen(line) > 50)
                printf("%s\t%d\t%.50s(...)\n",filename,current_line,line);
            else
                printf("%s\t%d\t%s",filename,current_line,line);
        }
        //printf("%s",line);
        current_line++;
    }

    //printf("+ Backup para %s creado correctamente! \n", filename);

    fclose(file_input);
    printf("\n");
    return 0;
}

int main(int argc, char *argv[]) {
    int i;
    regex_t regex;
    char regex_s[REGEXSZ];

    // revisamos si hay mas de 2 argumentos (un regex y un archivo como minimo), el argumento 0 es el nombre del programa
    if (argc >= 3) {
        strcpy(regex_s, argv[1]);  // guardamos el regex en un string

        // compilamos el regex y verificamos errores
        if (regcomp(&regex, regex_s, REG_EXTENDED)) {
            printf("No se pudo compilar el regex, verifique los datos ingresados\n");
            exit(1);
        }

        for (int i = 2; i < argc; i++) {
            regex_file(argv[i],regex);
        }

    } else
        printf("Se espera que haya al menos dos argumentos.\n");

    exit(0);
}
