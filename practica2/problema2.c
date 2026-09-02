#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define TOTAL_PEDIDOS 20

int main() {

    int tuberia[2];
    int pedidos[TOTAL_PEDIDOS];

    // creando la tuberia
    if (pipe(tuberia) == -1) {
        perror("Error al crear la tuberia");
        return 1;
    }

    printf("***** Bodega de distribucion de camisas *****\n\n");

    // el padre registra primero los 20 pedidos
    for (int i = 0; i < TOTAL_PEDIDOS; i++) {

        do {
            printf("Ingrese las unidades del pedido %d (1 - 100): ", i + 1);
            fflush(stdout);

            scanf("%d", &pedidos[i]);

            if (pedidos[i] < 1 || pedidos[i] > 100) {
                printf("Valor invalido. Debe estar entre 1 y 100.\n");
            }

        } while (pedidos[i] < 1 || pedidos[i] > 100);
    }

    printf("\nRegistro de pedidos finalizado.\n");
    printf("Iniciando estaciones de empaque...\n\n");

    // creando la primera estacion
    pid_t hijo1 = fork();

    if (hijo1 < 0) {
        perror("Error al crear la estacion 1");
        return 1;
    }

    if (hijo1 == 0) {

        // el hijo solo lee de la tuberia 
        close(tuberia[1]);

        int pedido;
        int pedidos_procesados = 0;
        int total_unidades = 0;

        // se leen pedidos mientras haya informacion en la tuberia
        while (read(tuberia[0], &pedido, sizeof(pedido)) > 0) {

            pedidos_procesados++;
            total_unidades += pedido;

            printf("Estacion 1 proceso pedido de %d unidades.\n", pedido);
            fflush(stdout);

            // simulando el tiempo de procesamiento
            usleep(100000);
        }

        printf("\n***** Reporte Estacion 1 *****\n");
        printf("Pedidos procesados: %d\n", pedidos_procesados);
        printf("Total de unidades: %d\n", total_unidades);

        close(tuberia[0]);

        exit(0);
    }

    // crenado la segunda estacion
    pid_t hijo2 = fork();

    if (hijo2 < 0) {
        perror("Error al crear la estacion 2");
        return 1;
    }

    if (hijo2 == 0) {

        // el hijo solo lee de la tuberia
        close(tuberia[1]);

        int pedido;
        int pedidos_procesados = 0;
        int total_unidades = 0;

        while (read(tuberia[0], &pedido, sizeof(pedido)) > 0) {

            pedidos_procesados++;
            total_unidades += pedido;

            printf("Estacion 2 proceso pedido de %d unidades.\n", pedido);
            fflush(stdout);

            // simulando el tiempo de procesamiento
            usleep(100000);
        }

        printf("\n***** Reporte Estacion 2 *****\n");
        printf("Pedidos procesados: %d\n", pedidos_procesados);
        printf("Total de unidades: %d\n", total_unidades);

        close(tuberia[0]);

        exit(0);
    }

    //proceso padre

    //el padre solo escribe
    close(tuberia[0]);

    //enviando los 20 pedidos a la banda transportadora
    for (int i = 0; i < TOTAL_PEDIDOS; i++) {

        write(tuberia[1],
              &pedidos[i],
              sizeof(pedidos[i]));
    }

    // cerrando el extremo de escritura
    close(tuberia[1]);

    // esperando a las dos estaciones
    waitpid(hijo1, NULL, 0);
    waitpid(hijo2, NULL, 0);

    printf("\nTodos los pedidos fueron procesados.\n");

    return 0;
}