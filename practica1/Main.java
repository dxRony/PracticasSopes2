import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        // inicializando scanner
        Scanner scanner = new Scanner(System.in);

        System.out.println("Simulador de Planificacion");
        System.out.println("Ejercicio 4 - Practica 1");

        // guardando el numero de priocesos
        System.out.print("Ingrese el numero de procesos: ");
        int cantidad;

        while (true) {
            cantidad = leerEntero(scanner, "Ingrese el numero de procesos: ");

            if (cantidad > 0) {
                break;
            }
            System.out.println("Error: la cantidad de procesos debe ser mayor que 0.");
        }

        // creando almacen de procesos
        List<Proceso> procesos = new ArrayList<>();
        // recorriendo cada proceso para la toma de datos
        for (int i = 1; i <= cantidad; i++) {
            System.out.println("\nProceso P" + i);
            int at;

            while (true) {
                at = leerEntero(scanner, "Tiempo de llegada (AT): ");

                if (at >= 0) {
                    break;
                }
                System.out.println("Error: el tiempo de llegada no puede ser negativo.");
            }

            int bt;
            while (true) {
                bt = leerEntero(scanner, "Tiempo de rafaga (BT): ");

                if (bt > 0) {
                    break;
                }
                System.out.println("Error: el tiempo de rafaga debe ser mayor que 0.");
            }
            Proceso proceso = new Proceso("P" + i, at, bt);
            procesos.add(proceso);
        }

        System.out.println("\nProcesos ingresados:");
        // mostrando la info de cada proceso ingresado
        for (Proceso proceso : procesos) {
            System.out.println(proceso.getNombre() + " | AT: " + proceso.getAt() + " | BT: " + proceso.getBt());
        }

        int opcion;

        while (true) {
            // guardando el algoritmo a utilizar
            System.out.println("\nSeleccione el algoritmo de planificacion:");
            System.out.println("1. FCFS");
            System.out.println("2. SJF");

            opcion = leerEntero(scanner, "Opcion: ");
            if (opcion == 1 || opcion == 2) {
                break;
            }
            System.out.println("Error: seleccione una opcion valida (1 o 2).");
        }

        // switch para ejecutar el algoritmo
        switch (opcion) {
            case 1:
                System.out.println("\nAlgoritmo seleccionado: FCFS");
                fcfs(procesos);
                mostrarResultados(procesos);
                break;

            case 2:
                System.out.println("\nAlgoritmo seleccionado: SJF");
                sjf(procesos);
                mostrarResultados(procesos);
                break;

            default:
                System.out.println("\nOpcion no valida.");
                break;
        }
        scanner.close();
    }

    // metodo para manejar los numeros enteros
    private static int leerEntero(Scanner scanner, String mensaje) {

        while (true) {
            try {
                System.out.print(mensaje);
                int valor = Integer.parseInt(scanner.nextLine());
                return valor;
            } catch (NumberFormatException e) {
                System.out.println("Error: debe ingresar un numero entero.");
            }
        }
    }

    // metodo del algoritmo fcfs
    private static void fcfs(List<Proceso> procesos) {

        // ordenando por tiempo de llegada
        procesos.sort((p1, p2) -> Integer.compare(p1.getAt(), p2.getAt()));

        int tiempoActual = 0;
        System.out.println("\n Diagrama de Gantt ");

        for (Proceso proceso : procesos) {
            // Si la cpu queda libre antes de que llegue el siguiente proceso
            if (tiempoActual < proceso.getAt()) {
                tiempoActual = proceso.getAt();
            }
            int inicio = tiempoActual;
            // ejecuta el proceso completo
            tiempoActual += proceso.getBt();

            int ct = tiempoActual;
            int tat = ct - proceso.getAt();
            int wt = tat - proceso.getBt();
            int rt = inicio - proceso.getAt();

            proceso.setCt(ct);
            proceso.setTat(tat);
            proceso.setWt(wt);
            proceso.setRt(rt);

            System.out.print("[" + inicio + " - " + ct + ": " + proceso.getNombre() + "] ");
        }
        System.out.println();
    }

    // metodo para mostrar los resultados del algoritnmo y los promedios
    private static void mostrarResultados(List<Proceso> procesos) {

        double sumaWt = 0;
        double sumaTat = 0;

        System.out.println("\n  Resultados  ");
        System.out.println("Proceso\tAT\tBT\tCT\tTAT\tWT\tRT");

        for (Proceso proceso : procesos) {
            System.out.println(
                    proceso.getNombre() + "\t"
                            + proceso.getAt() + "\t"
                            + proceso.getBt() + "\t"
                            + proceso.getCt() + "\t"
                            + proceso.getTat() + "\t"
                            + proceso.getWt() + "\t"
                            + proceso.getRt());

            sumaWt += proceso.getWt();
            sumaTat += proceso.getTat();
        }

        double promedioWt = sumaWt / procesos.size();
        double promedioTat = sumaTat / procesos.size();

        System.out.println("\nTiempo de espera promedio (WT): " + promedioWt);
        System.out.println("Tiempo de retorno promedio (TAT): " + promedioTat);
    }

    // metodo para manejar el algoritmo sjf
    private static void sjf(List<Proceso> procesos) {

        List<Proceso> pendientes = new ArrayList<>(procesos);
        List<Proceso> ejecutados = new ArrayList<>();
        int tiempoActual = 0;

        System.out.println("\n  Diagrama de Gantt  ");

        while (!pendientes.isEmpty()) {
            Proceso seleccionado = null;
            for (Proceso proceso : pendientes) {

                if (proceso.getAt() <= tiempoActual) {

                    if (seleccionado == null
                            || proceso.getBt() < seleccionado.getBt()
                            || (proceso.getBt() == seleccionado.getBt() && proceso.getAt() < seleccionado.getAt())) {

                        seleccionado = proceso;
                    }
                }
            }

            // si todavia no ha llegado ningun proceso
            // se avanza hasta el siguiente tiempo de llegada
            if (seleccionado == null) {
                int siguienteLlegada = Integer.MAX_VALUE;

                for (Proceso proceso : pendientes) {
                    if (proceso.getAt() < siguienteLlegada) {
                        siguienteLlegada = proceso.getAt();
                    }
                }

                tiempoActual = siguienteLlegada;
                continue;
            }

            int inicio = tiempoActual;
            // sjf no preventivo ejecuta completamente el proceso.
            tiempoActual += seleccionado.getBt();

            int ct = tiempoActual;
            int tat = ct - seleccionado.getAt();
            int wt = tat - seleccionado.getBt();
            int rt = inicio - seleccionado.getAt();

            seleccionado.setCt(ct);
            seleccionado.setTat(tat);
            seleccionado.setWt(wt);
            seleccionado.setRt(rt);

            System.out.print("[" + inicio + " - " + ct + ": " + seleccionado.getNombre() + "] ");

            ejecutados.add(seleccionado);
            pendientes.remove(seleccionado);
        }
        System.out.println();
        // se deja la lista en el orden real de ejecucion.
        procesos.clear();
        procesos.addAll(ejecutados);
    }
}
