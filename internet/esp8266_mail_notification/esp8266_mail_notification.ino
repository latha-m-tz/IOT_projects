#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "TZ Training Institute"     // CHANGE IT
#define WIFI_PASSWORD "TZ@TZ@124$" // CHANGE IT

// the sender email credentials
#define SENDER_EMAIL "subhashreelatha@gmail.com"        // CHANGE IT
#define SENDER_PASSWORD "aygo uyrd vyjo ppcc" // CHANGE IT to your Google App password

#define RECIPIENT_EMAIL "devi.o@tamilzorous.com"  // CHANGE IT

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

SMTPSession smtp;

unsigned long previousMillis = 0; // Store the last time email was sent
const long interval = 40000; // Interval for sending email (40 seconds)

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to send the email
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time email was sent

    String subject = "Email Notification from ESP8266";
    String textMsg = "Hi there! this IOT team here, This email is generated by ESP8266 microcontroller board."; // The message content

    gmail_send(subject, textMsg);
  }

  delay(100); // Small delay to prevent excessive looping
}

void gmail_send(String subject, String textMsg) {
  // set the network reconnection option
  MailClient.networkReconnect(true);

  smtp.debug(1);
  smtp.callback(smtpCallback);
  Session_Config config;

  // set the session config
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = SENDER_EMAIL;
  config.login.password = SENDER_PASSWORD;
  config.login.user_domain = F("gmail.com"); // Change to "gmail.com" for Gmail
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3; // Adjust based on your time zone
  config.time.day_light_offset = 0;

  // declare the message class
  SMTP_Message message;

  // set the message headers
  message.sender.name = F("ESP8266");
  message.sender.email = SENDER_EMAIL;
  message.subject = subject;
  message.addRecipient(F("To Whom It May Concern"), RECIPIENT_EMAIL);

  message.text.content = textMsg;
  message.text.transfer_encoding = "base64";
  message.text.charSet = F("utf-8");
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  // set the custom message header
  message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

  // connect to the server
  if (!smtp.connect(&config)) {
    Serial.println("Connection error");
    return;
  }

  // Check login status
  if (!smtp.isLoggedIn()) {
    Serial.println("Not yet logged in.");
  } else if (smtp.isAuthenticated()) {
    Serial.println("Successfully logged in.");
  } else {
    Serial.println("Connected with no Auth.");
  }

  // start sending Email and close the session
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Connection error: ");
    Serial.print("- Status Code: ");
    Serial.println(smtp.statusCode());
    Serial.print("- Error Code: ");
    Serial.println(smtp.errorCode());
    Serial.print("- Reason: ");
    Serial.println(smtp.errorReason().c_str());
  } else {
    // Print confirmation that the email was sent
    Serial.println("Email sent successfully!");
  }
}

// callback function to get the Email sending status
void smtpCallback(SMTP_Status status) {
  // print the current status
  Serial.println(status.info());

  // print the sending result
  if (status.success()) {
    for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
      SMTP_Result result = smtp.sendingResult.getItem(i);
      Serial.print("Status: ");
      Serial.println(result.completed ? "success" : "failed");
      Serial.print("Recipient: ");
      Serial.println(result.recipients.c_str());
      Serial.print("Subject: ");
      Serial.println(result.subject.c_str());
    }
    Serial.println("----------------\n");

    // free the memory
    smtp.sendingResult.clear();
  }
}
