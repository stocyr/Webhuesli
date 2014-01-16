/******************************************************************************/
/** \file       TCPServer.c
 *******************************************************************************
 *
 *  \brief
 *
 *  \author     N00bs
 *
 *  \date       Dec 2013
 *
 ******************************************************************************/
/*
 *  functions   blup
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "TCPServer.h"

//#include <arpa/inet.h>
//#include <time.h>

//#include "Webhouse.h"
//#include "Log.h"
//#include "BBBConfig.h"
//#include "BBBSignal.h"
//#include "Json.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

char rxBuf[RX_BUFFER_SIZE];
char txBuf[TX_BUFFER_SIZE];   // Message hier rein

int sockfd = -1;
int newsockfd = -1;


/*******************************************************************************
 *  function :    receiveDataTCP
 ******************************************************************************/
/** \brief
 *
 *  \type
 *
 *  \return
 *
 ******************************************************************************/
int receiveDataTCP()
{
	int rx_data_len;
	int value = 0;

	rx_data_len = recv(newsockfd, &rxBuf[0], RX_BUFFER_SIZE, 0);
	value = rx_data_len;
	if(rx_data_len > 0)
	{
		if(rx_data_len == 1 && rxBuf[0] == 0x88)
		{
			// close condition sent from client
			printf("client sent 0x88, closing connection\n");
			close(newsockfd);
			value = -2;
		}
		else
		{
			/* Process data received */
			/*---------------------------------------------------------------------*/

			printf("received:\n{\n%s\n}\n", rxBuf);

			/*---------------------------------------------------------------------*/
		}
	}
	else if(rx_data_len < 0)
	{
		/* Data not received */
		printf("error reading\n");
		value = -1;
	}

	// do nothing when rx_data_len = 0

	return value;
}

/*******************************************************************************
 *  function :    sendDataTCP
 ******************************************************************************/
/** \brief
 *
 *  \type
 *
 *  \return
 *
 ******************************************************************************/
void sendDataTCP()
{
	int tx_msg_len;

	tx_msg_len = send(newsockfd, &txBuf[0], TX_BUFFER_SIZE, 0);
	if(tx_msg_len > 0)
	{
		/* Data was sent */
		printf("data was sent\n");
	}
	else if(tx_msg_len == 0)
	{
		/* Connection was lost */
		printf("connection lost\n");
	}
	else
	{
		/* Data not sent */
		printf("data not sent\n");
	}
}
