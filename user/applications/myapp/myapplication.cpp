#include "application.h"
#include "debug.h"
#include "wifitester.h"
#include "RSSIData.pbo.h"

#include <stdio.h>
#include <string.h>

SYSTEM_MODE(MANUAL);

void setup() {
  //pinMode(D7, OUTPUT);
  Serial1.blockOnOverrun(true);
  Serial1.begin(9600);
  Serial.begin(115200);
  WiFi.on();
}

#define NUM_APS (100)

int seq = 0;
WiFiAccessPoint aps[10];
char str_buf[NUM_APS];
const uint8_t delimiter[4] = { 0xab, 0xcd, 0xef, 0x12 };
RSSIData data;

void loop() {
  Serial.println("Entering loop iteration");
  RSSIData_init(&data);
  Serial.println("Initialized RSSI data");

  int found = WiFi.scan(aps, NUM_APS);
  Serial.println("Finished scan");
  for (int i = 0; i < found; i++) {
    WiFiAccessPoint &ap = aps[i];

    sprintf(str_buf, "%02X:%02X:%02X:%02X:%02X:%02X", ap.bssid[0], ap.bssid[1],
            ap.bssid[2], ap.bssid[3], ap.bssid[4], ap.bssid[5]);
    Serial.println(str_buf);
    RSSIData_set_bssid(&data, str_buf, strlen(str_buf));
    RSSIData_set_rssi(&data, ap.rssi);
    RSSIData_to_bytes(&data, (char *)&data, seq++);
    Serial1.write(delimiter, 4);
    Serial1.write((const uint8_t *)&data, sizeof(RSSIData));
  }
}
