#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main (int argc, char *argv[]) {
    
    struct Cliente cliente;
    struct Factura factura;
    struct Pago pago;
    int opcion;

    do
    {
        opcion = menuPrincipal();
        switch (opcion) {
            case 1:
                registrarCliente();
                break;
            case 2:
                registrarFactura();
                break;
            case 3:
                consultarFacturasVencidas();
                break;
            case 4:
                registrarPago();
                break;
            case 5:
                generarReporteMensual();
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while ((opcion = menuPrincipal()) != 6);
    
    return 0;
}
