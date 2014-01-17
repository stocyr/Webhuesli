/*******************************************************************************
 * \author  kasen1
 * \date    10.01.2014
 *
 * \version 1.0
 *
 * \brief   Receive and transmit JSON
 ******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef RXTXJSON_H_
#define RXTXJSON_H_
 
/* Header-Files --------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
 
/* exported define -----------------------------------------------------------*/
#define MODE_ITEMP 1 /* Only send Ist-Temperatur value                        */
#define MODE_HEIZU 2 /* Only send Heizung value                               */
#define MODE_LICHT 3 /* Only send Lichtschranke value                         */
#define MODE_ALL   4 /* Send Ist-Temperatur, Heizung und Lichtschranke values */

//----- Function prototypes ----------------------------------------------------
extern int transmitAndGetValues(char * txBuf, char mode);
extern void receiveAndSetValues(char * rxBuf, int rx_data_len);

#endif /* RXTXJSON_H_ */
