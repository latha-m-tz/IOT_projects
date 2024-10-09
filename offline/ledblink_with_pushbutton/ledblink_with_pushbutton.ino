const int BUTTON_PIN = 7;
const int LED_PIN =  3;   

int buttonState = 0;  

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

void loop() {
  buttonState = digitalRead(BUTTON_PIN);

  if(buttonState == LOW)         
    digitalWrite(LED_PIN, HIGH); 
  else                           
    digitalWrite(LED_PIN, LOW);  
}
