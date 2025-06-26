#include <stdio.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int num){
    fflush(stdin); // Limpiar el buffer de entrada
    fgets(cadena, num, stdin);
    int len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') {
        cadena[len - 1] = '\0'; // Eliminar el salto de línea
    }
}

void guardarCliente(struct Cliente *cliente) {
    FILE *archivo = fopen("clientes.dat", "ab");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de clientes");
        return;
    }
    fwrite(cliente, sizeof(struct Cliente), 1, archivo);
    fclose(archivo);
}

void guardarFactura(struct Factura *factura) {
    FILE *archivo = fopen("facturas.dat", "ab");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de facturas");
        return;
    }
    fwrite(factura, sizeof(struct Factura), 1, archivo);
    fclose(archivo);
}

void guardarPago(struct Pago *pago) {
    FILE *archivo = fopen("pagos.dat", "ab");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de pagos");
        return;
    }
    fwrite(pago, sizeof(struct Pago), 1, archivo);
    fclose(archivo);
}

int menuPrincipal() {
    int opcion;
    printf("=== Sistema de Facturación y Cuentas por Cobrar ===\n");
    printf("1. Registrar Cliente\n");
    printf("2. Registrar Factura\n");
    printf("3. Consultar Facturas Vencidas\n");
    printf("4. Registrar Pago\n");
    printf("5. Generar Reporte Mensual\n");
    printf("6. Salir\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);
    return opcion;
}

void registrarCliente() {
    struct Cliente cliente;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(cliente.nombre, sizeof(cliente.nombre));
    printf("Ingrese la dirección del cliente: ");
    leerCadena(cliente.direccion, sizeof(cliente.direccion));
    printf("Ingrese el teléfono del cliente: ");
    leerCadena(cliente.telefono, sizeof(cliente.telefono));
    
    guardarCliente(&cliente);
    printf("Cliente registrado exitosamente.\n");
}

void registrarFactura() {
    struct Factura factura;
    printf("Ingrese la fecha de la factura (YYYY-MM-DD): ");
    leerCadena(factura.fecha, sizeof(factura.fecha));
    printf("Ingrese el monto de la factura: ");
    scanf("%f", &factura.monto);
    strcpy(factura.estado, "pendiente");
    
    // Generar un ID único para la factura
    FILE *archivo = fopen("facturas.dat", "rb");
    if (archivo != NULL) {
        fseek(archivo, 0, SEEK_END);
        factura.id = ftell(archivo) / sizeof(struct Factura) + 1;
        fclose(archivo);
    } else {
        factura.id = 1; // Primera factura
    }
    
    guardarFactura(&factura);
    printf("Factura registrada exitosamente.\n");
}

void consultarFacturasVencidas() {
    FILE *archivo = fopen("facturas.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de facturas");
        return;
    }
    
    struct Factura factura;
    printf("=== Facturas Vencidas ===\n");
    while (fread(&factura, sizeof(struct Factura), 1, archivo)) {
        if (strcmp(factura.estado, "pendiente") == 0) {
            printf("ID: %d, Fecha: %s, Monto: %.2f, Estado: %s\n", factura.id, factura.fecha, factura.monto, factura.estado);
        }
    }
    
    fclose(archivo);
}

void registrarPago() {
    struct Pago pago;
    printf("Ingrese el ID de la factura a pagar: ");
    scanf("%d", &pago.factura_id);
    printf("Ingrese el monto pagado: ");
    scanf("%f", &pago.monto_pagado);
    printf("Ingrese la fecha del pago (YYYY-MM-DD): ");
    leerCadena(pago.fecha_pago, sizeof(pago.fecha_pago));
    
    // Actualizar el estado de la factura
    FILE *archivoFacturas = fopen("facturas.dat", "rb+");
    if (archivoFacturas == NULL) {
        perror("Error al abrir el archivo de facturas");
        return;
    }
    
    struct Factura factura;
    int encontrado = 0;
    while (fread(&factura, sizeof(struct Factura), 1, archivoFacturas)) {
        if (factura.id == pago.factura_id) {
            encontrado = 1;
            if (strcmp(factura.estado, "pendiente") == 0) {
                strcpy(factura.estado, "pagada");
                fseek(archivoFacturas, -sizeof(struct Factura), SEEK_CUR);
                fwrite(&factura, sizeof(struct Factura), 1, archivoFacturas);
                printf("Pago registrado exitosamente.\n");
            } else {
                printf("La factura ya está pagada.\n");
            }
            break;
        }
    }
    
    fclose(archivoFacturas);
    
    if (!encontrado) {
        printf("Factura no encontrada.\n");
        return;
    }
    
    guardarPago(&pago);
}

void generarReporteMensual() {
    FILE *archivoFacturas = fopen("facturas.dat", "rb");
    if (archivoFacturas == NULL) {
        perror("Error al abrir el archivo de facturas");
        return;
    }
    
    FILE *archivoPagos = fopen("pagos.dat", "rb");
    if (archivoPagos == NULL) {
        perror("Error al abrir el archivo de pagos");
        fclose(archivoFacturas);
        return;
    }
    
    struct Factura factura;
    struct Pago pago;
    float totalFacturado = 0.0f;
    float totalCobrado = 0.0f;
    
    printf("=== Reporte Mensual de Facturación y Cobros ===\n");
    
    // Calcular total facturado
    while (fread(&factura, sizeof(struct Factura), 1, archivoFacturas)) {
        totalFacturado += factura.monto;
        printf("Factura ID: %d, Fecha: %s, Monto: %.2f, Estado: %s\n", factura.id, factura.fecha, factura.monto, factura.estado);
    }
    
    // Calcular total cobrado
    while (fread(&pago, sizeof(struct Pago), 1, archivoPagos)) {
        totalCobrado += pago.monto_pagado;
        printf("Pago para Factura ID: %d, Monto Pagado: %.2f, Fecha Pago: %s\n", pago.factura_id, pago.monto_pagado, pago.fecha_pago);
    }
    
    printf("Total Facturado: %.2f\n", totalFacturado);
    printf("Total Cobrado: %.2f\n", totalCobrado);
    
    fclose(archivoFacturas);
    fclose(archivoPagos);
}
