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

struct parella_adreca {
    char ip[16];
    int port;
};

int main(int argc, char *argv[])
{
    int sesc, scon;
    int bytes_llegits, bytes_escrits;
    char buffer[200];
    char iploc[16];
    int portloc;
    char iprem[16];
    int portrem;
    struct parella_adreca adreca_scon;
    struct parella_adreca adreca_sesc;

    printf("IP local del servidor (0.0.0.0 = totes):");
    if (scanf("%s", iploc) != 1) {
        printf("IP incorrecte\n");
        exit(-1);
    }

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

    if(TCP_TrobaAdrSockLoc(sesc, adreca_sesc.ip, &adreca_sesc.port) == 0) {
        printf("Socket sesc (socket local): %s %d\n", adreca_sesc.ip, adreca_sesc.port);
    } else {
        int error;
        printf("Error getsockname(sesc): %s\n", T_ObteTextRes(&error));
        exit(-1);
    }
    
    scon = TCP_AcceptaConnexio(sesc, iprem, &portrem);
    if (scon == -1) {
        int error;
        printf("Error al acceptar connexió: %s\n", T_ObteTextRes(&error));
        exit(-1);
    }

    /* Ara iprem i portrem contenen l'adreça del socket remot (@IP i #port TCP).            */
    printf("Connexió acceptada de %s:%d\n", iprem, portrem);

    if(TCP_TrobaAdrSockLoc(scon, adreca_scon.ip, &adreca_scon.port) == 0) {
        printf("Socket scon: %s %d\n", adreca_scon.ip, adreca_scon.port);
    } else {
        int error;
        printf("Error getsockname(scon): %s\n", T_ObteTextRes(&error));
        exit(-1);
    }

    if (TCP_TrobaAdrSockRem(scon, iprem, &portrem) == 0) {
        printf("Socket remot: %s %d\n", iprem, portrem);
    }

    /* 5) Crida read() i write()                                                            */
    /* Es retorna (ECO) pel socket connectat scon tot el que hi arriba, fins que el client  */
    /* tanca la connexió (read() retorna 0). 
    */
                                                    
    while ((bytes_llegits = TCP_RepLinia(scon, buffer, sizeof(buffer))) > 0) {
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
