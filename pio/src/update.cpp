#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

BearSSL::PublicKey signPubKey(R"EOF(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA6cw5Rpn9dIFhqKHZXRl7
xs+3qRHJSlDP5s1EjPqsDas6txovE8etu9AlatdeDFlM1JHac1Ao3BlY0UI5a8C4
BcqHlXWS8oAbNvdf1TYCVWv42fbolFCNodCt063pZd9dOffbPXrezTtnFdgzdRqi
k6ffvDQdfnBuQcE1a3TnEA/v4uQL1QzkXUiHQIFgz6QDeUZGL0sY1UugVWwy64N8
Vhhnb0ODCGBBXYSBoYdxG1O90PqNKS+5V/kOc8e471W1pG3wrdXOq5+Pp6xOLeL4
TmE+IZC/b/YfboD7mjeguFSwWHxcDsFQGE5/ieAn/hynUjOkQ8gREr5JvMbpstFk
6wIDAQAB
-----END PUBLIC KEY-----
)EOF");
BearSSL::HashSHA256 hash;
BearSSL::SigningVerifier sign( &signPubKey );

String getCurrentFirmwareVersion() {
  return ESP.getSketchMD5();
}

bool attemptFirmwareUpdate(const char* firmware_url) {

  WiFiClient client;
  Serial.printf("[update] Attempting firmware upgrade using URL  %s\n", firmware_url);

  auto ret = ESPhttpUpdate.update(client, firmware_url);
  switch(ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[update] failed. Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[update] No update available.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("[update] Update ok."); // may not be called since we reboot the ESP
      return true;
  }
  return false;
}
