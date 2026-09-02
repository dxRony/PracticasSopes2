#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define FIFO_PATH "/tmp/pagos_fifo"

int main() {

    char buffer[100];
    int total_pagos = 0;
    int cantidad_reportes = 0;

    printf("***** Centro de Operaciones *****\n");

    // creando FIFO
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        // si ya existe, no se toma obligatoriamente como un error grave
        printf("FIFO ya existe o no pudo crearse nuevamente.\n");
    }

    printf("Esperando reportes de las sucursales...\n\n");

    while (1) {

        // abriendo FIFO para lectura
        int fd = open(FIFO_PATH, O_RDONLY);

        if (fd == -1) {
            perror("Error al abrir la FIFO");
            return 1;
        }

        // limpiando buffer
        for (int i = 0; i < 100; i++) {
            buffer[i] = '\0';
        }

        // leyendo el mensaje
        int bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);

        close(fd);

        if (bytes_leidos <= 0) {
            continue;
        }

        buffer[bytes_leidos] = '\0';

        // validnado si se recibio el mensaje cerrar
        if (strcmp(buffer, "cerrar") == 0) {

            printf("\nMensaje de cierre recibido.\n");
            break;
        }

        // convirtiendo el reporte recibido a entero
        int pagos = atoi(buffer);

        // Obtener hora actual
        time_t ahora;
        struct tm *hora_actual;

        time(&ahora);
        hora_actual = localtime(&ahora);

        printf("Reporte recibido a las %02d:%02d:%02d\n",
               hora_actual->tm_hour,
               hora_actual->tm_min,
               hora_actual->tm_sec);

        printf("Pagos procesados por sucursal: %d\n\n", pagos);

        total_pagos += pagos;
        cantidad_reportes++;
    }

    printf("\n***** Resumen del dia *****\n");
    printf("Reportes recibidos: %d\n", cantidad_reportes);
    printf("Total de pagos procesados: %d\n", total_pagos);

    // eliminando  FIFO
    unlink(FIFO_PATH);

    printf("\nCentro de operaciones cerrado.\n");

    return 0;
}