#include <ESP8266WiFi.h>                 
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TamilZorous"; // WiFi Name (You want to connect)
const char* password = "TZ@12477"; // WiFi Password


const char* host="http://arduino.tamilzorous.com/";
const char* URL_SINGLE_UPDATE ="http://arduino.tamilzorous.com/api/setDataSingleUpdate"; // post



PROGMEM const int button1 = D1;
PROGMEM const int button2 = D2;
PROGMEM const int button3 = D3;
PROGMEM const int button4 = D4;

PROGMEM const int led1 = D5;
PROGMEM const int led2 = D6;
PROGMEM const int led3 = D7;
PROGMEM const int led4 = D8;


String switch1 = "";// D1
String switch2 = "";// D2
String switch3 = "";// D3
String switch4 = "";// D4

String old1 = "";// D1
String old2 = "";// D2
String old3 = "";// D3
String old4 = "";// D4

String kkk = ""; // token
String header; // This storees the HTTP request

void setup() {
  
  Serial.begin(115200);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  pinMode(button4,INPUT_PULLUP);
  
  //output light concept
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
 
 
 

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while(WiFi.status() !=WL_CONNECTED) //Wifi connection
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wifi connected");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
  Serial.println("NesMask: ");
  Serial.println(WiFi.subnetMask());
  Serial.println("Getway: ");
  Serial.println(WiFi.gatewayIP());

}

void loop() {     //1
   // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {   //2

//switch value read
switch1 = digitalRead(button1);
switch2 = digitalRead(button2);
switch3 = digitalRead(button3);
switch4 = digitalRead(button4);

//Serial.println("switch1 : "+switch1);
//Serial.println("switch2 : "+switch2);
//Serial.println("switch3 : "+switch3);
//Serial.println("switch4 : "+switch4);
   

    //post method

     HTTPClient http;    //Declare object of class HTTPClient
     
    http.begin("http://arduino.tamilzorous.com/api/userLogin");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
    StaticJsonBuffer<200> jsonBuffer;  //JsonBuffer is the entry point for using the library
    JsonObject& object1 = jsonBuffer.createObject();
    object1["username"] = "Mohanraj";
    object1["password"] = "Mohan@123";
    String body;
    object1.printTo(body);
    
    int httpCode = http.POST(body);
     //String httpCode = http.POST(body);
//Serial.println("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
     
   // Serial.println("poooooooooo httpCode:"+ body);
    //Send the request
    String payload = http.getString(); 
    //Serial.println("poooooooooo payload :"+ payload );
    //Get the response payload
 const size_t bufferSize1 = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer1(bufferSize1);
      JsonObject& root = jsonBuffer1.parseObject(http.getString());
  
String token=root["token"];
//Serial.println("token :" +token);
kkk = token;
String userId=root["user_id"];
//Serial.print("token:");
//Serial.println(token);
//Serial.print("userId:");
//Serial.println(userId);



// http.begin("http://143.110.190.254/api/getRoomDetails?id="+userId); //request

   http.begin("http://arduino.tamilzorous.com/api/getRoomDetails?id="+userId+"&type=ardunio&room_id=21&mac_address=123456857"); //request
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
    http.addHeader("Authorization", "Bearer "+kkk);
   

 //get method
      
    int httpCode1 = http.GET();
    
    
    //Check the returning code   

                                                                   
    if (httpCode1 > 0) {  //3
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonArray& root = jsonBuffer.parseArray(http.getString());   //JsonArray ----1
      // Parameters

     
   
 for (int i = 0; i < root.size(); i++) {  //4
  //Serial.println("ggggggggg :"+root.size());
  
    
   JsonObject& Room = root[i];  //Json object-----2
//
    String icon_id = Room["id"];
    
   

 //Serial.println("icon_id :" + icon_id );//id=5


//venki..................................................................
   JsonArray& roomList = Room["room_Details"];//room_Details=Json array//   JsonArray ----3
      
      for (int v = 0; v < Room["room_Details"].size();v++){  //5
        //Serial.println("v = "+v);

JsonObject& RoomDataList = roomList[v];   //JsonObject -------4
        String roomId = RoomDataList["id"];
   //     Serial.println("roomid :"+roomId);//roomID=21
        
         JsonArray& acc = RoomDataList["data"];  //JsonArray ----5

         //Serial.println("vvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
     //    Serial.println("roomid = "+roomId);
         
        
      //if (roomId == BOARD_ROOM_ID) 
      if (roomId == "21"){  //6
        for (int k = 0; k < RoomDataList["data"].size(); k++) {  //7
          
       // Serial.println("k = "+k);
         
          JsonObject& RoomData = acc[k];       //JsonObject -----6
          String dataValue = RoomData["data_value"];
          //String id = RoomData["id"];
          String acc_id = RoomData["id"];
          String action = RoomData["action"];
          String dataAction = "";


//          
//          Serial.println("data_value:"+dataValue);
//          Serial.println("id:"+acc_id);
//          Serial.println("action:"+action);

 
              //---------------switch  1

if (dataValue == "1") {

          Serial.println("old1 :"+old1);
                Serial.println("switch1 data :"+switch1);   
           if (switch1 != old1) {
              old1 = switch1;
              
              if (switch1 == "1") {
                digitalWrite(led1, HIGH);
                dataAction = "1";
                Serial.println("switch1 light on :"+switch1);
              }
              else
              {
                digitalWrite(led1, LOW);
                dataAction = "0";

                Serial.println("switch1 light on  :"+switch1);
              }
              //---------------------POST SWITCH BOARD DATA-------------------------------
               Serial.println("action Datatatta  :"+dataAction);
               setSwitchData(acc_id, dataAction);
               Serial.println("acc_id  :"+acc_id);
            }else {
              
              if (  action == "1"  )
              {

                digitalWrite(led1, HIGH);
                 dataAction = "1";
                Serial.println("light1 on");
              }
              else if (action == "0") {
                digitalWrite(led1, LOW);
                 dataAction = "0";
                Serial.println("light1 off");
              }

            }

         }
              




if (dataValue == "2") {

          Serial.println("old2 :"+old2);
                Serial.println("switch2 data :"+switch2);   
           if (switch2 != old2) {
              old2 = switch2;
              
              if (switch2 == "1") {
                digitalWrite(led2, HIGH);
                dataAction = "1";
                Serial.println("switch1 light on :"+switch2);
              }
              else
              {
                digitalWrite(led2, LOW);
                dataAction = "0";

                Serial.println("switch2 light on  :"+switch2);
              }
              //---------------------POST SWITCH BOARD DATA-------------------------------
               Serial.println("action Datatatta  :"+dataAction);
               setSwitchData(acc_id, dataAction);
            }else {
              
              if (  action == "1"  )
              {

                digitalWrite(led2, HIGH);
                 dataAction = "1";
                Serial.println("light2 on");
              }
              else if (action == "0") {
                digitalWrite(led2, LOW);
                 dataAction = "0";
                Serial.println("light2 off");
              }

            }

         }
              


//          //---------------switch  3
//
if (dataValue == "3") {
//  if (SWITCH_1_POSITION != SWITCH_1_OLD_POSITION) {
//              SWITCH_1_OLD_POSITION = SWITCH_1_POSITION;
          Serial.println("old3 :"+old3);
                Serial.println("switch3 data :"+switch3);   
           if (switch3 != old3) {
              old3 = switch3;
              
              if (switch3 == "1") {
                digitalWrite(led3, HIGH);
                dataAction = "1";
                Serial.println("switch3 light on :"+switch3);
              }
              else
              {
                digitalWrite(led3, LOW);
                dataAction = "0";

                Serial.println("switch3 light on  :"+switch3);
              }
              //---------------------POST SWITCH BOARD DATA-------------------------------
               Serial.println("action Datatatta  :"+dataAction);
               setSwitchData(acc_id, dataAction);
            }else {
              
              if (  action == "1"  )
              {

                digitalWrite(led3, HIGH);
                 dataAction = "1";
                Serial.println("light3 on");
              }
              else if (action == "0") {
                digitalWrite(led3, LOW);
                 dataAction = "0";
                Serial.println("light3 off");
              }

            }

         }
//              
//          //---------------switch  4

if (dataValue == "4") {
//  if (SWITCH_1_POSITION != SWITCH_1_OLD_POSITION) {
//              SWITCH_1_OLD_POSITION = SWITCH_1_POSITION;
          Serial.println("old4 :"+old4);
                Serial.println("switch4 data :"+switch4);   
           if (switch4 != old4) {
              old4 = switch4;
              
              if (switch4 == "1") {
                digitalWrite(led4, HIGH);
                dataAction = "1";
                Serial.println("switch4 light on :"+switch4);
              }
              else
              {
                digitalWrite(led4, LOW);
                dataAction = "0";

                Serial.println("switch4 light on  :"+switch4);
              }
              //---------------------POST SWITCH BOARD DATA-------------------------------
               Serial.println("action Datatatta  :"+dataAction);
               setSwitchData(acc_id, dataAction);
            }else {
              
              if (  action == "1"  )
              {

                digitalWrite(led4, HIGH);
                 dataAction = "1";
                Serial.println("light4 on");
              }
              else if (action == "0") {
                digitalWrite(led4, LOW);
                 dataAction = "0";
                Serial.println("light4 off");
              }

            }

         }
}//7
}  //6

}  //5
  }//4
   
    }  //3 
    http.end();   //Close connection    
  }   //2
  // Delay
  delay(500);
}  //1