/******************************************************************************/
/** \file       startup.c
 *******************************************************************************
 *
 *  \brief      Main application for the beaglebone black webhouse
 *              <p>
 *              The application implements a socket server (ip: localhost,
 *              port:5000) and waits on a connection attempt of the client.
 *              <p>
 *              The client can control  different in- and outputs of the
 *              webhouse. The following objects are available:
 *              <ul>
 *              <li> dlampe: The ceiling lamp can be dimmed from 0 (dark) to
 *              100 (bright)
 *              <li> slampe: The floor lamp can be dimmed from 0 (dark) to
 *              100 (bright)
 *              <li> tv: The TV can be turned on/off
 *              <li> tempsoll: Set's the target temperature within the webhouse
 *              <li> tempist: The current temperature within the house
 *              <li> alarm: The alarm can be turned on/off
 *              <li> pir: The alarm was triggered by the pir
 *              </ul>
 *
 *  \author     wht4
 *
 *  \date       August 2013
 *
 *  \remark     Last Modification
 *               \li wht4, August 2013, Created
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              main
 *  functions  local:
 *              shutdownHook
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Webhouse.h"
#include "Log.h"
#include "BBBConfig.h"
#include "BBBSignal.h"
#include "Json.h"

#include "TCPServer.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
static void shutdownHook(int32_t sig);

//----- Data -------------------------------------------------------------------
enum state {NORMAL, SHUTDOWN, CLOSING};
static volatile int eShutdown = NORMAL;

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    main
 ******************************************************************************/
/** \brief        Starts the socket server (ip: localhost, port:5000) and waits
 *                on a connection attempt of the client.
 *
 *  \type         global
 *
 *  \return       EXIT_SUCCESS
 *
 ******************************************************************************/
int main(int argc, char **argv) {

	BBBError error = BBB_SUCCESS;

	//struct sockaddr_in
	//{
	//	uint8_t        sa_len;      // Length
	//	sa_family_t    sin_family;  // Address family
	//	in_port_t      sin_port;    // Address port (16bit)
	//	struct in_addr sin_addr;    // IP (32bit)
	//	char           sin_zero[8];	// Not used
	//}

	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	/* Initialize the webhouse */
	error = initWebhouse();

	printf("\nStart of BBB Webhouse with Websocket TCP Server on port 5000");

	if ((error == BBB_SUCCESS) && (registerExitHandler(shutdownHook) == BBB_SUCCESS)) {
		// ###############################################################
		// 			start effective TCP WEbsocket Server HERE
		// ###############################################################

		/* Create Server Socked (SS) */
		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		//printf("socket created\n");

		/* Bind */
		//	memset(&server_addr 0, sizeof(server_addr));	// zero the struct before filling the fields
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(SERVER_PORT_NBR);

		if( bind(sockfd, (struct sockaddr*) &serv_addr,
				sizeof(struct sockaddr_in)) < 0) {
			close(sockfd);
			INFOPRINT("\nbinding failed!");
		} else {
			/* Socket bound to desired port now */
			//printf("socket bound to desired port\n");

			/* Listen */
			if (listen(sockfd, BACKLOG) < 0) {
				close(sockfd);
				INFOPRINT("\nlistening failed!");
			} else {
				/* Socket placed in listening state now */
				printf("\nsocket in listening state now\n");

				/* Accept */
				/* get data length of addr_remote */
				clilen = sizeof(cli_addr);

				while (eShutdown != SHUTDOWN) {
					newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr,
							&clilen);

					if (newsockfd < 0) {
						close(sockfd);
					} else {
						printf("\nconnection established");
						bzero(rxBuf, RX_BUFFER_SIZE);
						bzero(txBuf, TX_BUFFER_SIZE);

						while (eShutdown != CLOSING) {
							/* Connection established now, use newSock_id to communicate with client */


							// todo: senden der anfangszustände des webhueslis

							n = recv(newsockfd, rxBuf, RX_BUFFER_SIZE, MSG_DONTWAIT);

							if(n > 0) {
								rxBuf[n] = '\0';
								printf("\nmsg (n=%d) = \n{\n%s\n}\n", n, rxBuf);
								send(newsockfd, rxBuf, n, 0);
							}
							if(n == 0) {
								printf("\nConnection closed by client.");
								eShutdown = CLOSING;
							}
						}
					}
				}
				close(sockfd);
				close(newsockfd);
			}
		}

		// ###############################################################
		// 				END OF TCP SERVER
		// ###############################################################
	} else {
		ERRORPRINT("Failed to start BBB Webhouse");
	}

	/* Detach all resource */
	finalizeWebhouse();
	printf("\n\nStop of BBB webhouse \n");

	return EXIT_SUCCESS;
}

/*******************************************************************************
 *  function :    shutdownHook
 ******************************************************************************/
/** \brief        Handle the registered signals (SIGTERM, SIGINT)
 *
 *  \type         static
 *
 *  \param[in]    sig    incoming signal
 *
 *  \return       void
 *
 ******************************************************************************/
static void shutdownHook(int32_t sig) {

	printf("Ctrl-C pressed....shutdown hook in main");
	eShutdown = SHUTDOWN;
}

