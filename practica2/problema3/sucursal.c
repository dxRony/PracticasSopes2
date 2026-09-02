#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PATH "/tmp/pagos_fifo"

int main() {

    char mensaje[100];

    printf("***** Sucursal *****\n");

    printf("Ingrese total de pagos procesados o escriba 'cerrar': ");
    scanf("%99s", mensaje);

    // abriendo FIFO para escritura
    int fd = open(FIFO_PATH, O_WRONLY);

    if (fd == -1) {
        perror("Error al abrir la FIFO");
        printf("Verifique que el centro de operaciones haya creado la FIFO.\n");
        return 1;
    }

    // enviando mensaje al centro
    write(fd, mensaje, strlen(mensaje));

    close(fd);

    printf("Reporte enviado correctamente.\n");

    return 0;
}