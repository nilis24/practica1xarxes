/**************************************************************************/
/*                                                                        */
/* L'aplicació ECO amb sockets TCP/IP                                     */
/* Fitxer serECO.c que implementa el servidor ECO sobre la capa de        */
/* transport TCP (fent crides a la "nova" interfície de la capa de        */
/* transport o "nova" interfície de sockets).                             */
/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "p1-tTCP.h"

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc, char *argv[])
{
    int sesc, scon;
    int bytes_llegits, bytes_escrits;
    char buffer[200];
    char iploc[16] = "0.0.0.0";
    int portloc;
    char iprem[16];
    int portrem;

    printf("Introdueix port del Servidor:");
    if (scanf("%d", &portloc) != 1) {
        printf("Port incorrecte\n");
        exit(-1);
    }

    sesc = TCP_CreaSockServidor(iploc, portloc);
    if (sesc == -1) {
        int error;
        printf("Error al crear el socket: %s\n", T_ObteTextRes(&error));
        exit(-1);
    }

    /* 4) Crida accept()                                                                    */
    /* Ara espera una petició de connexió TCP d’un client, i un cop rebuda, l'accepta. El   */
    /* descriptor de socket retornat, scon, serà un nou identificador del socket local per  */
    /* a la connexió que s'acaba d'establir. Ambdós sockets tenen la mateixa adreça (@IP i  */
    /* #port TCP), però sesc és un socket “d’escolta” i scon és un socket "connectat". En   */
    /* aquest cas, però, com que sesc no tenia assignada una @IP concreta, sinó qualsevol,  */
    /* és a dir, totes les de la màquina (l'@IP 0.0.0.0), scon tindrà la “mateixa” @IP,     */
    /* però “concretada” a una d’elles (el S.O. li assignarà l’@IP que hagi fet servir el   */
    /* client en la petició de connexió).                                                   */
    scon = TCP_AcceptaConnexio(sesc, iprem, &portrem);
    if (scon == -1) {
        int error;
        printf("Error al acceptar connexió: %s\n", T_ObteTextRes(&error));
        exit(-1);
    }

    /* Ara iprem i portrem contenen l'adreça del socket remot (@IP i #port TCP).            */
    printf("Connexió acceptada de %s:%d\n", iprem, portrem);

    if (TCP_TrobaAdrSockLoc(scon, iploc, &portloc) == 0 && TCP_TrobaAdrSockRem(scon, iprem, &portrem) == 0) {
        printf("Socket local: %s %d\n", iploc, portloc);
        printf("Socket remot: %s %d\n", iprem, portrem);
    }

    /* 5) Crida read() i write()                                                            */
    /* Es retorna (ECO) pel socket connectat scon tot el que hi arriba, fins que el client  */
    /* tanca la connexió (read() retorna 0).                                                */
    while ((bytes_llegits = TCP_Rep(scon, buffer, sizeof(buffer))) > 0) {
        printf("%.*s", bytes_llegits, buffer);
        printf("Bytes rebuts: %d\n", bytes_llegits);
        fflush(stdout);
        bytes_escrits = TCP_Envia(scon, buffer, bytes_llegits);
        if (bytes_escrits == -1) {
            int error;
            printf("Error al enviar el missatge: %s\n", T_ObteTextRes(&error));
            exit(-1);
        }
    }
    if (bytes_llegits == -1) {
        int error;
        printf("Error al rebre el missatge: %s\n", T_ObteTextRes(&error));
        exit(-1);
    }
    /* 6) Crida close()                                                                     */
    /* Es tanca el socket scon, que com que és un socket TCP, també vol dir que es tanca la */
    /* connexió TCP establerta, i es tanca el socket d'escolta sesc.                        */
    if (TCP_TancaSock(scon) == -1) {
        int error;
        printf("Error al tancar socket: %s\n", T_ObteTextRes(&error));
        exit(-1);
    }
    if (TCP_TancaSock(sesc) == -1) {
        int error;
        printf("Error al tancar socket: %s\n", T_ObteTextRes(&error));
        exit(-1);
    }

    exit(0);
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */
