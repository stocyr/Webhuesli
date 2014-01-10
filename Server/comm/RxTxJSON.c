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
void receiveAndSetValues()
{
	/* Webhuesli variables, initialised with default values */
	boolE Fernseher = FALSE;
	char Stehlampe = 0;
	char Kronleuchter = 0;
	char TemperaturSoll = 20;
	
	/* JSON variables */
	// char * pcString = "{\"Hello\":\"World\"}";
	char * pcValue = NULL;
	json_t *jsonMsg = NULL;

	/* Handle received JSON */
	jsonMsg = createJsonFromBuffer(rxBuf, rx_data_len);
	if(jsonMsg != NULL)
	{
		/* Fernseher */
		pcValue = getJsonStringValue(jsonMsg, "TV");
		if(pcValue != NULL)
		{
			if(strcmp(pcValue, "ON") == 0)
			{
				/* pcValue ist "ON" */
				Fernseher = TRUE;
			}
			else
			{
				/* pcValue ist nicht "ON" */
				Fernseher = FALSE;
			}
		}
		/* Stehlampe */
		pcValue = getJsonStringValue(jsonMsg, "Lampe");
		if(pcValue != NULL)
		{
			/* String zu Zahl konvertieren */
			Stehlampe = atoi(pcValue);
		}
		/* Kronleuchter */
		pcValue = getJsonStringValue(jsonMsg, "Leuchter");
		if(pcValue != NULL)
		{
			/* String zu Zahl konvertieren */
			Kronleuchter = atoi(pcValue);
		}
		/* Soll-Temperatur */
		pcValue = getJsonStringValue(jsonMsg, "TempSoll");
		if(pcValue != NULL)
		{
			/* String zu Zahl konvertieren */
			TemperaturSoll = atoi(pcValue);
		}
		
		/* Free ressources */
		cleanUpJson(jsonMsg);
	}

	/* Assign values */
	/* Fernseher */
	if(Fernseher == TRUE)
	{
		turnTVOn();
	}
	else
	{
		turnTVOff();
	}
	/* Stehlampe */
	dimSLampe(Stehlampe);
	/* Kronleuchter */
	dimDLampe(Kronleuchter);
	/* Soll-Temperatur */
	dimHeizung(TemperaturSoll);
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
void transmitAndGetValues(char mode)
{
	/* Assign values */
	char TemperaturIst = getTempIst();
	char Heizung       = getHeizungState();
	char Lichtschranke = getAlarmState();
	
	/* JSON variables */
	// char * pcString = "{\"Hello\":\"World\"}";
	json_t *jsonMsg = NULL;
	char * pcMsg = NULL;
	int32_t s32MsgSize = 0;

	/* Handle JSON to transmit all values */
	jsonMsg = createNewJsonMsg();
	if(jsonMsg != NULL)
	{
		/* Define key-value pairs */
		char jsonBuffer[20];
		/* Check mode */
		if(mode == MODE_ALL || mode = MODE_ITEMP)
		{
			/* Ist-Temperatur */
			sprintf(jsonBuffer, "%d", TemperaturIst);
			setJsonStringKeyValue(jsonMsg, "TempIst", jsonBuffer);
		}
		if(mode == MODE_ALL || mode = MODE_HEIZU)
		{
			/* Heizung */
			sprintf(jsonBuffer, "%d", Heizung);
			setJsonStringKeyValue(jsonMsg, "Heizung", jsonBuffer);
		if(mode == MODE_ALL || mode = MODE_LICHT)
		{
			/* Lichtschranke */
			sprintf(jsonBuffer, "%d", Lichtschranke);
			setJsonStringKeyValue(jsonMsg, "Burglar", jsonBuffer);
		}
		
		/* Create JSON string */
		pcMsg = getJsonMsg(jsonMsg, &s32MsgSize);
		if(pcMsg != NULL)
		{
			/* Write to transmit buffer */
			txBuf = pcMsg;

			/* Free ressources */
			cleanUpSentJsonMsg(jsonMsg, pcMsg);
		}
		else
		{
			/* Free ressources */
			cleanUpJson(jsonMsg);
		}
	}
}