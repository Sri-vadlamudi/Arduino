/****************************************************************************************************************************
  dynamicParams.h
  For SAM DUE boards using WiFiNINA Shields

  Blynk_WiFiNINA_WM is a library for the Mega, Teensy, SAM DUE, nRF52, STM32, SAMD and RP2040 boards 
  (https://github.com/khoih-prog/Blynk_WiFiNINA_WM) to enable easy configuration/reconfiguration and
  autoconnect/autoreconnect of WiFiNINA/Blynk

  Modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
  Built by Khoi Hoang https://github.com/khoih-prog/Blynk_WiFiNINA_WM
  Licensed under MIT license
 *****************************************************************************************************************************/

#ifndef dynamicParams_h
#define dynamicParams_h

#if USE_BLYNK_WM

//#define USE_DYNAMIC_PARAMETERS      true

/////////////// Start dynamic Credentials ///////////////

//Defined in <BlynkSimpleWiFiNINA_DUE_WM.h>
/**************************************
  #define MAX_ID_LEN                5
  #define MAX_DISPLAY_NAME_LEN      16

  typedef struct
  {
  char id             [MAX_ID_LEN + 1];
  char displayName    [MAX_DISPLAY_NAME_LEN + 1];
  char *pdata;
  uint8_t maxlen;
  } MenuItem;
**************************************/

#if USE_DYNAMIC_PARAMETERS

#define MAX_MQTT_SERVER_LEN      34
char MQTT_Server  [MAX_MQTT_SERVER_LEN + 1]   = "default-mqtt-server";

#define MAX_MQTT_PORT_LEN        6
char MQTT_Port   [MAX_MQTT_PORT_LEN + 1]  = "1883";

#define MAX_MQTT_USERNAME_LEN      34
char MQTT_UserName  [MAX_MQTT_USERNAME_LEN + 1]   = "default-mqtt-username";

#define MAX_MQTT_PW_LEN        34
char MQTT_PW   [MAX_MQTT_PW_LEN + 1]  = "default-mqtt-password";

#define MAX_MQTT_SUBS_TOPIC_LEN      34
char MQTT_SubsTopic  [MAX_MQTT_SUBS_TOPIC_LEN + 1]   = "default-mqtt-SubTopic";

#define MAX_MQTT_PUB_TOPIC_LEN       34
char MQTT_PubTopic   [MAX_MQTT_PUB_TOPIC_LEN + 1]  = "default-mqtt-PubTopic";

MenuItem myMenuItems [] =
{
  { "mqtt", "MQTT Server",      MQTT_Server,      MAX_MQTT_SERVER_LEN },
  { "mqpt", "Port",             MQTT_Port,        MAX_MQTT_PORT_LEN   },
  { "user", "MQTT UserName",    MQTT_UserName,    MAX_MQTT_USERNAME_LEN },
  { "mqpw", "MQTT PWD",         MQTT_PW,          MAX_MQTT_PW_LEN },
  { "subs", "Subs Topics",      MQTT_SubsTopic,   MAX_MQTT_SUBS_TOPIC_LEN },
  { "pubs", "Pubs Topics",      MQTT_PubTopic,    MAX_MQTT_PUB_TOPIC_LEN },
};

uint16_t NUM_MENU_ITEMS = sizeof(myMenuItems) / sizeof(MenuItem);  //MenuItemSize;

#else

MenuItem myMenuItems [] = {};

uint16_t NUM_MENU_ITEMS = 0;

#endif    //USE_DYNAMIC_PARAMETERS

/////// // End dynamic Credentials ///////////

#endif      //USE_BLYNK_WM

#endif      //dynamicParams_h
