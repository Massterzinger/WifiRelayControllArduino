#include <ESP8266WiFi.h>

#define PinLen 8

// Replace with your network credentials
const char* ssid = "KV-54";
//const char* ssid = "milkyRoute";
const char* password = "password";

WiFiServer server(5000);


byte OutPins[PinLen] = {0, 2, 4, 5, 12, 13, 14, 16};
int i = 0;

String webPage = "HTTP/1.1 200 OK\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">    <title>MusicPage</title></head><body>";

void setup() {
  webPage += "<h1>ESP8266 Web Server</h1>";
  for (i = 0; i < PinLen; ++i)
  {
    pinMode(OutPins[i], OUTPUT);
    digitalWrite(OutPins[i], LOW);
    webPage += "<p>Socket #";
    webPage += (i + 1);
    webPage +=  " <a href=\"";
    webPage += (i);
    webPage +=  "1\"><button>ON</button></a>&nbsp;<a href=\"";
    webPage += (i);
    webPage +=  "0\"><button>OFF</button></a></p>";
  }
  webPage += "<p>Socket #All <a href=\"A1\"><button>ON</button></a>&nbsp;<a href=\"A0\"><button>OFF</button></a></p>";
  webPage += "</body></html>\n";
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  server.begin();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop(void) {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  if(req[5] >= '0' && req[5] <= '9'){
    int Ind = (req[5]-'0');
    int pos = req[6]-'0';
    Serial.println(Ind);
    Serial.println(pos);
    digitalWrite(OutPins[Ind], pos);
  }
  if(req[5] == 'A'){
    int pos = req[6]-'0';
    for(i = 0; i < PinLen; ++i){
      digitalWrite(OutPins[i], pos);
    }
  }
  client.flush();
  client.print(webPage);
  delay(1);
}
