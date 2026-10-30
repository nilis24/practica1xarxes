/**************************************************************************/
/*                                                                        */
/* L'aplicació ECO amb sockets TCP/IP                                     */
/* Fitxer cliECO.c que implementa el client ECO sobre la capa de          */
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
    /* Declaració de variables, p.e., int n;                                 */

    char resposta = 'S'; //per a saber si s'ha de sortir o no.
    int Socket; //valor del Socket
    int PortServidor;
    char ServidorIp[16];
    char Iplocal[16];
    int Portlocal;
    char IpRemot[16];
    int PortRemot;
    char missatge[1024];

    /* Expressions, estructures de control, crides a funcions, etc.          */

    while (resposta != 'N') {
        printf("Introdueix direcció IP del Servidor:");
        scanf("%15s", ServidorIp);
        getchar();

        printf("Introdueix port del Servidor:");
        scanf("%d", &PortServidor);
        getchar();

        //Ara obtenim el Socket:
        Socket = TCP_CreaSockClient("0.0.0.0", 0);
        if (Socket == -1) { //el -1 en cas de que no vagi
            int error;
            printf("Error al crear el socket: %s\n", T_ObteTextRes(&error));
            exit(-1);
        }

        if (TCP_DemanaConnexio(Socket, ServidorIp, PortServidor) == -1) {
            int error;
            printf("Error al connectar amb el servidor: %s\n", T_ObteTextRes(&error));
            exit(-1);
        }

        if (TCP_TrobaAdrSockLoc(Socket, Iplocal, &Portlocal) == 0 &&
            TCP_TrobaAdrSockRem(Socket, IpRemot, &PortRemot) == 0) {
            printf("Socket local: %s %d\n", Iplocal, Portlocal);
            printf("Socket remot: %s %d\n", IpRemot, PortRemot);
        }

        while (1) { //!??
            printf("Escriu un missatge (. per sortir): ");
            if (!fgets(missatge, sizeof(missatge), stdin)) break;

            if (missatge[0] == '.') {
                break;
            }

            int numBits = TCP_Envia(Socket, missatge, (int)strlen(missatge));
            if (numBits == -1) {
                int error;
                printf("Error en enviar les dades: %s\n", T_ObteTextRes(&error));
                break;
            }

            printf("Bytes enviats: %d\n", numBits);

            int numBitsRebuts = TCP_Rep(Socket, missatge, (int)sizeof(missatge));
            if (numBitsRebuts > 0) {
                if (numBitsRebuts < (int)sizeof(missatge)) missatge[numBitsRebuts] = '\0';
               printf("Retornat pel servidor: %s", missatge);
            } else if (numBitsRebuts == 0) {
                printf("El servidor ha tancat la connexio\n");
                break;
            } else {
                int error;
                printf("Error rebent les dades: %s\n", T_ObteTextRes(&error));
                break;
            }
        }

        TCP_TancaSock(Socket);

        printf("Vols obrir nova connexió? (S/N): "); //suposem que s'introdueix
        scanf(" %c", &resposta);
        getchar();
    }
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */
