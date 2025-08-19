#include <SPI.h>
#include <MFRC522.h>

// RFID pins
#define SS_PIN    21
#define RST_PIN   22
#define MOSI_PIN  23
#define MISO_PIN  19
#define SCK_PIN   18

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Authorized UIDs and names
String authorizedUIDs[] = {"233B1FBE", "A242CA01", "63698205"};
String employeeNames[] = {"Md. Mehedi Hasan", "Maria Rashid", "Emon Shahriare"};
const int NUM_AUTH = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);

String getUIDString(byte *uid, byte uidSize) {
  String uidStr = "";
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] < 0x10) uidStr += "0";
    uidStr += String(uid[i], HEX);
  }
  uidStr.toUpperCase();
  return uidStr;
}

String getNameByUID(String uid) {
  for (int i = 0; i < NUM_AUTH; i++) {
    if (authorizedUIDs[i] == uid) return employeeNames[i];
  }
  return "Unknown";
}

void setup() {
  Serial.begin(115200);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  mfrc522.PCD_Init();
  Serial.println("RFID Attendance Ready");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidStr = getUIDString(mfrc522.uid.uidByte, mfrc522.uid.size);
    String name = getNameByUID(uidStr);
    String status = (name != "Unknown") ? "SUCCESS" : "UNAUTHORIZED";
    Serial.printf("RFID:%s,%s,%s\n", uidStr.c_str(), name.c_str(), status.c_str());
    delay(1500); // Prevent duplicate reads
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}
