#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"

#define TAMANIO_BUFFER 20

typedef int ESTADO;
int linea=1;
typedef enum {
    INICIO, 
    FIN, 
    LEER, 
    ESCRIBIR, 
    ID, 
    CONSTANTE, 
    PARENIZQUIERDO,
    PARENDERECHO, 
    PUNTOYCOMA, 
    COMA, 
    ASIGNACION, 
    SUMA, 
    RESTA, 
    FDT,
    ERROR
} TIPO_TOKEN;

char _buffer[TAMANIO_BUFFER + 1];
int  _pos = 0;

void LimpiarBuffer(void) {
    memset(_buffer, '\0', TAMANIO_BUFFER + 1);
    _pos = 0;
}

void AgregarCaracter(int caracter) 
{
    _buffer[_pos++] = caracter;
}

const char *Buffer(void) 
{
    _buffer[_pos] = '\0';
    return _buffer;
}

TOKEN EsReservada(void) 
{
    if (strcmp(_buffer, "inicio") == 0) return INICIO;
    if (strcmp(_buffer, "fin") == 0) return FIN;
    if (strcmp(_buffer, "leer") == 0) return LEER;
    if (strcmp(_buffer, "escribir") == 0) return ESCRIBIR;
    return ID;
}

int ObtenerColumna(int simbolo)
{
    if (isalpha(simbolo))
        return 0;
    if (isdigit(simbolo))
        return 1;
    if (isspace(simbolo))
        return 11;

    switch (simbolo)
    {
    case '+':
        return 2;
    case '-':
        return 3;
    case '(':
        return 4;
    case ')':
        return 5;
    case ',':
        return 6;
    case ';':
        return 7;
    case ':':
        return 8;
    case '=':
        return 9;
    case -1:
        return 10;
    default:
        return 12;
    }
}


void formarToken(TOKEN token, char *tipoDeToken){
    switch (token)
    {
    case INICIO:
        strncpy(tipoDeToken, "INICIO", 17); 
        break;
        
    case LEER:
        strncpy(tipoDeToken, "LEER", 17); 
        break;
        
    case ESCRIBIR:
        strncpy(tipoDeToken, "ESCRIBIR", 17); 
        break;
        
    case FIN:
        strncpy(tipoDeToken, "FIN", 17); 
        break;
        
    case ID: 
        strncpy(tipoDeToken, "ID", 17); 
        break;
        
    case CONSTANTE:
        strncpy(tipoDeToken, "CONSTANTE", 17); 
        break;

    case PARENIZQUIERDO:
        strncpy(tipoDeToken, "PARENIZQUIERDO", 17); 
        break;
        
    case PARENDERECHO:
        strncpy(tipoDeToken, "PARENDERECHO", 17); 
        break;
        
    case PUNTOYCOMA:
        strncpy(tipoDeToken, "PUNTOYCOMA", 17); 
        break;
        
    case COMA:
    strncpy(tipoDeToken, "COMA", 17); 
    break;
    
    case ASIGNACION :
    strncpy(tipoDeToken, "ASIGNACION", 17);
    break; 

    case SUMA :
    strncpy(tipoDeToken, "SUMA", 17); 
    break;

    case RESTA :
    strncpy(tipoDeToken, "RESTA", 17); 
    break;

    case FDT:
    strncpy(tipoDeToken, "FDT", 17); 
    break;
    
    case ERROR:
    strncpy(tipoDeToken, "ERROR", 17); 
    break;
}
}

ESTADO Transicion(ESTADO estado, int simbolo) 
{
    static ESTADO TT[19][13] = 
    {
    //   L   D   +   -   (   )   ,    ;   :   =  fdt sp  otro
        {1 , 3 , 5 , 6 , 7 , 8 , 9 , 10, 11, 15, 13, 0 , 17}, // 0
        {1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 17}, // 1
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 2
        {14, 3 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4,  4 , 4 , 17}, // 3
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 4    
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 5
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 6
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 7
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 8
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 9
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 10
        {16, 16, 16, 16, 16, 16, 16, 16, 16, 12, 16, 16, 17}, // 11
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 12
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 13
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 14
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 15
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 16
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 17
        {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // 99
    };
    int columna = ObtenerColumna(simbolo);
    return TT[estado][columna];
}

void agregarRestoDePalabraAlBuffer(int c){
    AgregarCaracter(c);
    while ((c = getchar())){
        if(ObtenerColumna(c)>1) break;
            AgregarCaracter(c);
        }
    ungetc(c, stdin);
}

TOKEN Scanner(void) 
{
    TOKEN token = 0;
    int c, pos = 0;
    ESTADO estado = 0;
    LimpiarBuffer();

    while ((c = getchar())) 
    {
        if(c=='\n'){
            linea+=1;
        }
        estado = Transicion(estado, c);
        switch (estado) 
        {
            case 1:
            case 3:
                AgregarCaracter(c);
                break;
            case 2:
                ungetc(c, stdin);
                return EsReservada();
            case 4:
                ungetc(c, stdin);
                return CONSTANTE;
            case 5:
                AgregarCaracter(c);
                return SUMA;
            case 6:
                AgregarCaracter(c);
                return RESTA;
            case 7:
                AgregarCaracter(c);
                return PARENIZQUIERDO; 
            case 8:
                AgregarCaracter(c);
                return PARENDERECHO; 
            case 9:
                AgregarCaracter(c);
                return COMA; 
            case 10:
                AgregarCaracter(c);
                return PUNTOYCOMA; 
            case 11:
                AgregarCaracter(c);
                break;
            case 12:
                AgregarCaracter(c);
                return ASIGNACION;
            case 14:
                fprintf(stderr, "Error lexico en linea: %d, digito inmediatamente seguido de letra:\n", linea);
                agregarRestoDePalabraAlBuffer(c);
                return ERROR;
            case 15:
                 fprintf(stderr, "Error lexico en linea: %d, simbolo \"=\" mal colocado. Debe ir un \":\" adelante: \n", linea);
                agregarRestoDePalabraAlBuffer(c);
                return ERROR;
            case 16:
                fprintf(stderr, "Error lexico en linea: %d, seguido a \":\", solo puede ir \"=\" \n", linea);
                agregarRestoDePalabraAlBuffer(c);
                return ERROR;
            case 17:
                fprintf(stderr, "Error lexico en linea: %d, simbolo no reconocido\n", linea);
                agregarRestoDePalabraAlBuffer(c);
                return ERROR;
            case 99:
                fprintf(stderr, "Error, estado de relleno\n");
                agregarRestoDePalabraAlBuffer(c);
                return ERROR;
            case 13:
                return FDT;
        }
    }

    return FDT;
}
