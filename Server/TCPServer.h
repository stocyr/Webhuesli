#ifndef TCPSERVER_H_
#define TCPSERVER_H_
/******************************************************************************/
/** \file       TCPServer.h
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
 *  function    .
 *
 ******************************************************************************/
#define SERVER_PORT_NBR 5000	//siehe Webhausdoku s.1

#define BACKLOG 5

#define RX_BUFFER_SIZE 500
#define TX_BUFFER_SIZE 500

extern int sockfd;
extern int newsockfd;
extern char rxBuf[RX_BUFFER_SIZE];
extern char txBuf[TX_BUFFER_SIZE];
extern int sockfd;
extern int newsockfd;

/* prototypes */
int receiveDataTCP();
void sendDataTCP();

#endif /* TCPSERVER_H_ */
