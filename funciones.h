/* 3. Sistema de Facturación y Cuentas por Cobrar para una PyME
Contexto / Consigna:
Una PyME necesita llevar un control básico de sus ventas a crédito y cobrar las facturas pendientes.

Funcionalidades esperadas:

Registrar clientes y sus facturas (fecha, monto, estado: pagada/pendiente).

Consultar facturas vencidas.

Registrar pagos parciales o totales.

Generar reporte mensual de facturación y cobros.

Guardar la información persistente en archivos binarios (clientes.dat, facturas.dat).

*/
#include <stdio.h>
#include <string.h>

struct Cliente {
    char nombre[50];
    char direccion[100];
    char telefono[15];
};

struct Factura {
    int id;
    char fecha[11]; // Formato: YYYY-MM-DD
    float monto;
    char estado[10]; // "pagada" o "pendiente"
};

struct Pago {
    int factura_id;
    float monto_pagado;
    char fecha_pago[11]; // Formato: YYYY-MM-DD
};

void leerCadena(char *cadena, int num);
void guardarCliente(struct Cliente *cliente);
void guardarFactura(struct Factura *factura);
void guardarPago(struct Pago *pago);
int menuPrincipal();
void registrarCliente();
void registrarFactura();
void consultarFacturasVencidas();
void registrarPago();
void generarReporteMensual();


