/*******************************************************************************
 * \author  kasen1
 * \date    10.01.2014
 *
 * \version 1.0
 *
 * \brief   Receive and transmit JSON
 ******************************************************************************/

/* Header-Files --------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "Json.h"
#include "RxTxJSON.h"
#include "Webhouse.h"

/* Implementation ------------------------------------------------------------*/

/*******************************************************************************
 *  function :    receiveAndSetValues
 ******************************************************************************/
/** \brief        Receives values as JSON and sets them
 *
 *  \param[in]    none
 *
 *  \return       none
 *
 ******************************************************************************/
void receiveAndSetValues(char * rxBuf, int rx_data_len) {
	/* Webhuesli variables, initialised with default values */
	char Stehlampe = 0;
	char Kronleuchter = 0;
	char TemperaturSoll = 20;
	char * last_occurrence;

	/* JSON variables */
	// char * pcString = "{\"Hello\":\"World\"}";
	char * pcValue = NULL;
	json_t *jsonMsg = NULL;

	/* preprocess JSON msg: delete all {...} pairs except the last one */
	last_occurrence = strrchr(rxBuf, '{');
	rx_data_len = (rxBuf + rx_data_len) - last_occurrence;
	//printf("DEBUG:\n%d, %d, %d:\n\"%s\"", (int)rxBuf, (int)last_occurrence, rx_data_len, last_occurrence);
	rxBuf = last_occurrence;

	/* Handle received JSON */
	jsonMsg = createJsonFromBuffer(rxBuf, rx_data_len);
	if (jsonMsg != NULL) {
		/* Fernseher */
		pcValue = getJsonStringValue(jsonMsg, "TV");
		if (pcValue != NULL) {
			if (strcmp(pcValue, "ON") == 0) {
				/* pcValue ist "ON" */
				turnTVOn();
				printf("\n TV on");
			} else {
				/* pcValue ist nicht "ON" */
				turnTVOff();
				printf("\n TV off");
			}
		}
		/* Stehlampe */
		pcValue = getJsonStringValue(jsonMsg, "Lampe");
		if (pcValue != NULL) {
			/* String zu Zahl konvertieren */
			Stehlampe = atoi(pcValue);
			dimSLampe(Stehlampe);
			printf("\n Stehlampe: %d", Stehlampe);
		}
		/* Kronleuchter */
		pcValue = getJsonStringValue(jsonMsg, "Leuchter");
		if (pcValue != NULL) {
			/* String zu Zahl konvertieren */
			Kronleuchter = atoi(pcValue);
			dimDLampe(Kronleuchter);
			printf("\n Kronleuchter: %d", Kronleuchter);
		}
		/* Soll-Temperatur */
		pcValue = getJsonStringValue(jsonMsg, "TempSoll");
		if (pcValue != NULL) {
			/* String zu Zahl konvertieren */
			TemperaturSoll = atoi(pcValue);
			dimHeizung(TemperaturSoll);
			printf("\n Temperatur: %d", TemperaturSoll);
		}

		/* Free ressources */
		cleanUpJson(jsonMsg);
	}
}

/*******************************************************************************
 *  function :    receiveAndSetValues
 ******************************************************************************/
/** \brief        Receives values as JSON and sets them
 *
 *  \param[in]    mode = MODE_ITEMP : Only send Ist-Temperatur value                       
 *                mode = MODE_HEIZU : Only send Heizung value                              
 *                mode = MODE_LICHT : Only send Lichtschranke value                        
 *                mode = MODE_ALL   : Send Ist-Temperatur, Heizung und Lichtschranke values
 *
 *  \return       none
 *
 ******************************************************************************/
int transmitAndGetValues(char * txBuf, char mode) {
	/* Assign values */
	char TemperaturIst = getTempIst();
	char Heizung = getHeizungState();
	char Lichtschranke = getAlarmState();

	/* JSON variables */
	// char * pcString = "{\"Hello\":\"World\"}";
	json_t *jsonMsg = NULL;
	char * pcMsg = NULL;
	int length = 0;

	/* Handle JSON to transmit all values */
	jsonMsg = createNewJsonMsg();
	if (jsonMsg != NULL) {
		/* Define key-value pairs */
		char jsonBuffer[20];
		/* Check mode */
		if (mode == MODE_ALL || mode == MODE_ITEMP) {
			/* Ist-Temperatur */
			sprintf(jsonBuffer, "%d", TemperaturIst);
			setJsonStringKeyValue(jsonMsg, "TempIst", jsonBuffer);
		}
		if (mode == MODE_ALL || mode == MODE_HEIZU) {
			/* Heizung */
			sprintf(jsonBuffer, "%d", Heizung);
			setJsonStringKeyValue(jsonMsg, "Heizung", jsonBuffer);
		}
		if (mode == MODE_ALL || mode == MODE_LICHT) {
			/* Lichtschranke */
			sprintf(jsonBuffer, "%d", Lichtschranke);
			setJsonStringKeyValue(jsonMsg, "Burglar", jsonBuffer);
		}

		/* Create JSON string */
		pcMsg = getStringRep(jsonMsg);
		if (pcMsg != NULL) {
			/* Write to transmit buffer */
			txBuf = pcMsg;

			while(txBuf[length] != '\0')
				length++;

			/* Free ressources */
			cleanUpStringRep(pcMsg);
		}
		/* Free ressources */
		cleanUpJson(jsonMsg);
	}

	return length;
}
