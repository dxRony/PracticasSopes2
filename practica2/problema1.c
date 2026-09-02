#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{

    int padre_hijo[2];
    int hijo_padre[2];
    char opcion;
    char palabra[100];
    int tarjeta;

    // creando las dos tuberias
    if (pipe(padre_hijo) == -1)
    {
        perror("Error al crear pipe padre-hijo");
        return 1;
    }

    if (pipe(hijo_padre) == -1)
    {
        perror("Error al crear pipe hijo-padre");
        return 1;
    }

    // creando el proceso hijo
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Error al crear el proceso hijo");
        return 1;
    }

    // proceso hijo
    if (pid == 0)
    {

        // el hijo no escribe en padre_hijo
        close(padre_hijo[1]);

        // el hijo no lee de hijo_padre
        close(hijo_padre[0]);

        int realizar_verificacion;

        // recibiendo la verificacion del padre
        read(padre_hijo[0],
             &realizar_verificacion,
             sizeof(realizar_verificacion));

        if (realizar_verificacion == 1)
        {

            // recibiendo palabra
            read(padre_hijo[0],
                 palabra,
                 sizeof(palabra));

            // calculando longitud manualmente
            int longitud = 0;

            while (palabra[longitud] != '\0')
            {
                longitud++;
            }

            printf("\nCanal activo.\n");
            printf("Palabra invertida: ");

            // mostrando la palabra al reves
            for (int i = longitud - 1; i >= 0; i--)
            {
                printf("%c", palabra[i]);
            }

            printf("\n");
        }

        // reciviendo el numero de tarjeta
        read(padre_hijo[0],
             &tarjeta,
             sizeof(tarjeta));

        char respuesta[20];

        // validando si la tarjeta es par o impar
        if (tarjeta % 2 == 0)
        {
            char aprobado[] = "PAGO_APROBADO";
            int i = 0;

            while (aprobado[i] != '\0')
            {
                respuesta[i] = aprobado[i];
                i++;
            }
            respuesta[i] = '\0';
        }
        else
        {
            char rechazado[] = "PAGO_RECHAZADO";
            int i = 0;

            while (rechazado[i] != '\0')
            {
                respuesta[i] = rechazado[i];
                i++;
            }

            respuesta[i] = '\0';
        }

        // mandando respuesta al padre
        write(hijo_padre[1],
              respuesta,
              sizeof(respuesta));

        // cerrando tuberias
        close(padre_hijo[0]);
        close(hijo_padre[1]);

        exit(0);
    }

    // proceso padre
    else
    {
        // padre no lee de padre_hijo
        close(padre_hijo[0]);

        // padre no escribe en hijo_padre
        close(hijo_padre[1]);

        printf("*****Sistema de pago en linea*****\n");

        printf("Quieres verificar el estado del canal? s/n : ");
        scanf(" %c", &opcion);

        int realizar_verificacion;

        if (opcion == 's' || opcion == 'S')
        {
            realizar_verificacion = 1;
        }
        else
        {
            realizar_verificacion = 0;
        }

        // avisando al hijo si habra verificacion
        write(padre_hijo[1],
              &realizar_verificacion,
              sizeof(realizar_verificacion));

        if (realizar_verificacion == 1)
        {
            printf("Ingrese una palabra: ");
            fflush(stdout);
            scanf("%99s", palabra);

            // mandando palabra al hijo
            write(padre_hijo[1],
                  palabra,
                  sizeof(palabra));
        }

        // solicitando la tarjeta valida
        do
        {
            printf("\nIngrese numero de tarjeta (1000 - 9999): ");
            fflush(stdout);
            scanf("%d", &tarjeta);

            if (tarjeta < 1000 || tarjeta > 9999)
            {
                printf("Numero de tarjeta invalido.\n");
            }

        } while (tarjeta < 1000 || tarjeta > 9999);

        // enviando la tarjeta al hijo
        write(padre_hijo[1],
              &tarjeta,
              sizeof(tarjeta));

        char respuesta[20];

        // recibiendo resultado enviado por el hijo
        read(hijo_padre[0],
             respuesta,
             sizeof(respuesta));

        printf("\nResultado final: %s\n", respuesta);

        // cerrando tuberias
        close(padre_hijo[1]);
        close(hijo_padre[0]);

        // esperando a que termine el hijo
        wait(NULL);
    }

    return 0;
}