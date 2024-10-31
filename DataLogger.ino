#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <Ticker.h>

#define DEBUG 1

#if DEBUG
#define  P_R_I_N_T(x)   Serial.println(x)
#else
#define  P_R_I_N_T(x)
#endif


Ticker mqttTicker;
const char* ssid = "japan-2.4";
const char* password = "@dminPower";

//Your Domain name with URL path or IP address with path
const char* server = "192.168.1.4";
const int port = 5000;
String serverName = "http://192.168.1.4:9999/cm";//SONOFF WEB SERVER

//enter your file name
const char* LOG_FILE = "/log.csv";//read-only pointer variable

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 30 seconds (30000)
unsigned long timerDelay = 20000;
String payload = "hi,010";


void setup() {
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  if(mountSPIFFS()){      
    //loadConfigFile();
    Serial.println("SPIFFS mount success");  
  }
  else    {
    Serial.println("An Error has occurred while mounting SPIFFS");
  }

  mqttTicker.attach(20, readFile);
  
}



void loop() {
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "?cmnd=status%2010";
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        DynamicJsonDocument doc(256);
        deserializeJson(doc, payload);
        String Timestamp = doc["StatusSNS"]["Time"];
        String Power = doc["StatusSNS"]["ENERGY"]["Power"];
//        Serial.println(Timestamp+","+Power);
        String data = Timestamp+","+Power;
//        String data ="{\"t\":"+Timestamp+";\"p\":"+Power+"}";
        writeFile(data);        
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

      delay(1000);
//      sendData(payload);
//      sendCSV();
      sendFile();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


void sendData(String data){
//    String data="power,100.1";
    HTTPClient http;
    http.begin(server, port, "/json");
    http.addHeader("Content-Type", "text/csv");
    http.addHeader("Content-Length", String(payload.length()));
  //  http.addHeader("Content-Disposition", "form-data; name=\"testfile\"; filename=\"123.csv\"" );//Content-Disposition: form-data; name="testfile"; filename="1521369140.htm
  //  http.addHeader("Authorization", "Bearer your_AUTH_TOKEN");
  
    // Send the HTTP POST request and check the response
    int httpResponseCode = http.POST(data);
    if (httpResponseCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    } else {
      Serial.printf("Error code: %s\n", http.errorToString(httpResponseCode).c_str());
    }
  
        // Free resources
     http.end();
}


void sendCSV(){
//    String data="power,100.1";
      HTTPClient http;
      http.begin(server, port, "/json");

     // Set content type header
        http.addHeader("Content-Type", "text/csv");
        http.addHeader("Content-Length", String(payload.length()));
     
  
      // Send the HTTP POST request and check the response
      int httpResponseCode = http.POST(payload);
      if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      } else {
        Serial.printf("Error code: %s\n", http.errorToString(httpResponseCode).c_str());
      }
    
          // Free resources
       http.end();
}





void sendFile(){
//    String data="power,100.1";
      HTTPClient http;
      http.begin(server, port, "/json");

    // Make a HTTP request and add HTTP headers
    //String boundary = "CustomizBoundarye----";
    //change with your content type
    //String contentType = "audio/x-wav";


    String requestHead = "--DataSoftJapan\r\nContent-Disposition: form-data; name=\"file\"; filename=\"log.csv\"\r\nContent-Type: text/csv\r\n\r\n";

    String tail = "\r\n--DataSoftJapan--\r\n";

    String content = requestHead + payload + tail;
    int contentLength =  requestHead.length() + payload.length() + tail.length();
    Serial.printf("%s: %i\n", "Payload size:", contentLength);

         // Set content type header
//        http.addHeader("Content-Type", "text/csv");
        http.addHeader("Content-Length", String(contentLength, DEC));
//        http.addHeader("Content-Type: multipart/form-data; boundary=DataSoftJapan");
        String contentType = "multipart/form-data; boundary=DataSoftJapan";
        http.addHeader("Content-Type", contentType);

  
      // Send the HTTP POST request and check the response
      int httpResponseCode = http.POST(content);
      if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      } else {
        Serial.printf("Error code: %s\n", http.errorToString(httpResponseCode).c_str());
      }
    
       // Free resources
       http.end();
}




void mqttPublish(String payload){
 // Construct the HTTP POST request

        
}
