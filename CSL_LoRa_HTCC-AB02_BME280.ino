/*
 * Based on CubeCell example libraries
 * 
 * Heltec HTCC-AB02 and a BME280 TPRH sensor for Zihao Zhang at CCNY Architecture
 * Uses Adafruit's BME280 library as Seeed no good
 * 
 * Jason Cayetano, Ricardo Toledo-Crow, NGENS ASRC CUNY,rtoledocrow@gc.cuny.edu, Oct 2022
 */

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Adafruit_BME280.h>
#include <Wire.h>

/*
   set LoraWan_RGB to Active,the RGB active in loraWan
   RGB red means sending;
   RGB purple means joined done;
   RGB blue means RxWindow1;
   RGB yellow means RxWindow2;
   RGB green means received done;
*/

/* OTAA para*/
// For devEui the last two bytes are random.
// For the rest see https://lora-developers.semtech.com/documentation/tech-papers-and-guides/the-book/deveui/
uint8_t devEui[] = { 0xFE, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xXX, 0xXX }; //
// aka joinEUI Use all 0x00 as described here: https://lora-developers.semtech.com/documentation/tech-papers-and-guides/the-book/joineui
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// random from TTI website random
uint8_t appKey[] = { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX }; 

/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda, 0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef, 0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 180000; // 3 mins

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
  Number of trials to transmit the frame, if the LoRaMAC layer did not
  receive an acknowledgment. The MAC performs a datarate adaptation,
  according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
  to the following table:

  Transmission nb | Data Rate
  ----------------|-----------
  1 (first)       | DR
  2               | DR
  3               | max(DR-1,0)
  4               | max(DR-1,0)
  5               | max(DR-2,0)
  6               | max(DR-2,0)
  7               | max(DR-3,0)
  8               | max(DR-3,0)

  Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
  the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
Adafruit_BME280 bme280;

static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
    appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
    if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
    if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
    for example, if use REGION_CN470,
    the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW); // turn Vext on
  delay(250); // wait for Vext to stabilize
  bme280.begin();
  delay(250); // wait for begin. should actually test.
  float temperature = bme280.readTemperature();
  float pressure = bme280.readPressure()/100.00;
  float humidity = bme280.readHumidity();

  //float sealevelPressure = bme280.readSealevelPressure();

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  //    Serial.print("Real altitude = ");
  //    Serial.print(bme280.readAltitude(101500));
  //    Serial.println(" meters");
  Wire.end();// turn off comms before power
  delay(1000); 
  digitalWrite(Vext, HIGH); // Vext off

  float batteryVoltage = getBatteryVoltage()/1000.00;

  unsigned char *puc;
  puc = (unsigned char *)(&temperature);
  appDataSize = 16;
  appData[0] = puc[0];
  appData[1] = puc[1];
  appData[2] = puc[2];
  appData[3] = puc[3];

  puc = (unsigned char *)(&pressure);
  appData[4] = puc[0];
  appData[5] = puc[1];
  appData[6] = puc[2];
  appData[7] = puc[3];

  puc = (unsigned char *)(&humidity);
  appData[8] = puc[0];
  appData[9] = puc[1];
  appData[10] = puc[2];
  appData[11] = puc[3];

  puc = (unsigned char *)(&batteryVoltage);
  appData[12] = puc[0];
  appData[13] = puc[1];
  appData[14] = puc[2];
  appData[15] = puc[3];

  Serial.print("T:");  Serial.print(temperature);
  Serial.print(" P:"); Serial.print(pressure);
  Serial.print(" H:"); Serial.print(humidity);
  Serial.print(" V:"); Serial.print(batteryVoltage);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println(__FILE__);

#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
}

void loop()
{
  switch ( deviceState )
  {
    case DEVICE_STATE_INIT:
      {
#if(LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
        getDevParam();
#endif
        printDevParam();
        LoRaWAN.init(loraWanClass, loraWanRegion);
        deviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        LoRaWAN.join();
        break;
      }
    case DEVICE_STATE_SEND:
      {
        prepareTxFrame( appPort );
        LoRaWAN.send();
        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {
        LoRaWAN.sleep();
        break;
      }
    default:
      {
        deviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}
