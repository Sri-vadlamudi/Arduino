/****************************************************************************************************************************
  BlynkSimpleWiFiNINA_RP2040.h
  For RP2040 boards using WiFiNINA Shields

  Blynk_WiFiNINA_WM is a library for the Mega, Teensy, SAM DUE, nRF52, STM32, SAMD and RP2040 boards 
  (https://github.com/khoih-prog/Blynk_WiFiNINA_WM) to enable easy configuration/reconfiguration and
  autoconnect/autoreconnect of WiFiNINA/Blynk

  Modified from Blynk library v0.6.1 https://github.com/blynkkk/blynk-library/releases
  Built by Khoi Hoang https://github.com/khoih-prog/Blynk_WiFiNINA_WM
  Licensed under MIT license


  Original Blynk Library author:
  @file       BlynkSimpleWiFiNINA.h
  @author     Volodymyr Shymanskyy
  @license    This project is released under the MIT License (MIT)
  @copyright  Copyright (c) 2018 Volodymyr Shymanskyy
  @date       Sep 2018
  @brief

  Version: 1.1.0

  Version Modified By   Date        Comments
  ------- -----------  ----------   -----------
  1.0.0   K Hoang      07/04/2020  Initial coding
  1.0.1   K Hoang      09/04/2020  Add support to SAM DUE, Teensy, STM32
  1.0.2   K Hoang      15/04/2020  Fix bug. Add SAMD51 support.
  1.0.3   K Hoang      05/05/2020  Add nRF52 support, MultiWiFi/Blynk, Configurable Config Portal Title, 
                                   Default Config Data and DRD. Update examples.
  1.0.4   K Hoang      13/05/2020  Add support to Arduino UNO WiFi R2 
  1.1.0   K Hoang      28/05/2021  Add support to Nano_RP2040_Connect, RASPBERRY_PI_PICO using Arduino mbed or pico core
                                   Enable scan of WiFi networks for selection in Configuration Portal
 *****************************************************************************************************************************/


#ifndef BlynkSimpleWiFiNINA_RP2040_h
#define BlynkSimpleWiFiNINA_RP2040_h

#if ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || \
      defined(ARDUINO_GENERIC_RP2040) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) )
  #if defined(WIFININA_USE_RP2040)
    #undef WIFININA_USE_RP2040
  #endif
  #define WIFININA_USE_RP2040      true
#else
  #error This code is intended to run only on the RP2040-based boards ! Please check your Tools->Board setting.
#endif

#define BLYNK_WIFININA_WM_VERSION        "Blynk_WiFiNINA_WM v1.1.0"

//////////////////////////////////////////////
// From v1.1.0 to display correct BLYNK_INFO_DEVICE

#define BLYNK_USE_128_VPINS

#if defined(BLYNK_INFO_DEVICE)
  #undef BLYNK_INFO_DEVICE
#endif
#define BLYNK_BUFFERS_SIZE    4096

#if defined(BLYNK_INFO_DEVICE)
  #undef BLYNK_INFO_DEVICE
#endif

#if defined(BOARD_NAME)
  #define BLYNK_INFO_DEVICE   BOARD_NAME
#elif defined(BOARD_TYPE)
  #define BLYNK_INFO_DEVICE   BOARD_TYPE
#else
  #define BLYNK_INFO_DEVICE   "RP2040"
#endif

//////////////////////////////////////////////

#ifndef BLYNK_INFO_CONNECTION
  #define BLYNK_INFO_CONNECTION  "WiFiNINA"
#endif

#define BLYNK_SEND_ATOMIC
//#define BLYNK_USE_SSL

#include <WiFiNINA_Generic.h>

#ifdef BLYNK_USE_SSL
  #define BLYNK_SERVER_PORT BLYNK_DEFAULT_PORT_SSL
#else
  #define BLYNK_SERVER_PORT BLYNK_DEFAULT_PORT
#endif

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

class BlynkWifiCommon
    : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
public:
    BlynkWifiCommon(BlynkArduinoClient& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass)
    {
        int status = WiFi.status();
        // check for the presence of the shield:
        if (status == WL_NO_MODULE) {
            BLYNK_LOG1(BLYNK_F("NoNINA"));
            return;
        }

        BLYNK_LOG2(BLYNK_F("WiFiNINA Firmware Version: "), WiFi.firmwareVersion());

        // attempt to connect to Wifi network:
        while (status != WL_CONNECTED) 
        {
            BLYNK_LOG2(BLYNK_F("Con2:"), ssid);
            if (pass && strlen(pass)) 
            {
                status = WiFi.begin((char*)ssid, (char*)pass);
            } 
            else 
            {
                status = WiFi.begin((char*)ssid);
            }
            
            if (status == WL_CONNECTED)
            {
              BLYNK_LOG1(BLYNK_F("ConW OK"));
            }
            else
            {
              BLYNK_LOG1(BLYNK_F("ConW not OK"));
            }

            millis_time_t started = BlynkMillis();
            while ((status != WL_CONNECTED) && (BlynkMillis() - started < 10000))
            {
                BlynkDelay(200);
                status = WiFi.status();
            }
        }

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP:", myip);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_SERVER_PORT)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_SERVER_PORT)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_SERVER_PORT)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_SERVER_PORT)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port);
        while(this->connect() != true) {}
    }

};

//static WiFiSSLClient _blynkWifiClient;
static WiFiClient _blynkWifiClient;

static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif    //BlynkSimpleWiFiNINA_RP2040_h
