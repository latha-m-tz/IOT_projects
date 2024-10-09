      led1 =8;
int led2 =9;
int led3 =10;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);//pinout 9
  pinMode(led2, OUTPUT);//pinout 10
  pinMode(led3, OUTPUT);//pinout 11
  pinMode(led4, OUTPUT);//pinout 12
}
void loop() {
  //led1
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
//led2
digitalWrite(led2, HIGH);   
  delay(500);                      
  digitalWrite(led2, LOW);    
  delay(500);  
//led3
digitalWrite(led3, HIGH);   
  delay(1000);                       
  digitalWrite(led3, LOW);   
  delay(1000);  
 //led4
digitalWrite(led4, HIGH);   
  delay(200);                       
  digitalWrite(led4, LOW);   
  delay(200);   
}
