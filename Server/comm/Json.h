#ifndef JSON_H_
#define JSON_H_
/******************************************************************************/
/** \file       Json.h
 *******************************************************************************
 *
 *  \brief      Provides functions to communicate with json objects between
 *              client and host.
 *              <p>
 *              The module allows to create json objects out of a buffer or a
 *              string. Further an empty json object can be created. There
 *              is a function to get the string represantation of a json object,
 *              a function to add a key/value pair and a function to get a value
 *              to a corresponding key. Keep in mind, that all resource of a
 *              json object must be freed by the corresponding function.
 *              <p>
 *              Example of creating a json message out of a string:
 *              <pre>
 *              char * pcString = "{\"Hello\":\"World\"}";
 *              char * pcValue = NULL;
 *              json_t *jsonMsg = NULL;
 *
 *              jsonMsg = createJsonFromString(pcString);
 *              if(jsonMsg != NULL) {
 *                  pcValue = getJsonStringValue(jsonMsg, "Hello");
 *                  if(pcValue != NULL)
 *                      printf("%s", pcValue);
 *
 *                  // Don't forget to free ressources!
 *                  cleanUpJson(jsonMsg);
 *              }
 *              </pre>
 *              <p>
 *              Example of creating your own json message:
 *              <pre>
 *              json_t *jsonMsg = NULL;
 *              char * pcMsg = NULL;
 *              int32_t s32MsgSize = 0;
 *
 *              jsonMsg = createNewJsonMsg();
 *              if(jsonMsg != NULL) {
 *
 *                  setJsonStringKeyValue(jsonMsg, "Hello", "World");
 *                  pcMsg = getJsonMsg(jsonMsg, &s32MsgSize);
 *                  if(pcMsg != NULL) {
 *                  	printf("%s", pcMsg);
 *
 *                  	// Don't forget to free ressources!
 *                  	cleanUpSentJsonMsg(jsonMsg, pcMsg);
 *                  } else {
 *                      // Don't forget to free ressources!
 *                      cleanUpJson(jsonMsg);
 *                  }
 *              }
 *              </pre>
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    createJsonFromBuffer
 *              createJsonFromString
 *              createNewJsonMsg
 *              cleanUpJson
 *              getStringRep
 *              cleanUpStringRep
 *              getJsonStringValue
 *              setJsonStringKeyValue
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <stdint.h>

#include "jansson.h"
#include "BBBTypes.h"
//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern json_t * createJsonFromBuffer(char *pcMsg, uint32_t u32Length);

extern json_t * createJsonFromString(char *pcMsg);

extern json_t * createNewJsonMsg(void);

extern BBBError cleanUpJson(json_t *pJson);

extern char *   getStringRep(json_t * pJson);

extern void     cleanUpStringRep(char * pcJsonMsg);

extern char *   getJsonStringValue(json_t * pJson, char * pcKey);

extern BBBError setJsonStringKeyValue(json_t * pJson, char * pcKey, char * pcValue);

//----- Data -------------------------------------------------------------------

#endif /* JSON_H_ */
