#include <FluoTube.h>

void setup() {
  Serial.begin(115200);
  Serial.println("AVR booting ...");

  pinMode(13, OUTPUT);
  
  FluoTube.Begin();
  
  FluoTube.WiFi.Setting("Androidadry", "hellohello", "fluo"); //Set WiFi (ssid, passkey, hostname)
  FluoTube.WiFi.ServerRest(); // enable Rest Server default port 8080
}

void loop() {
  FluoTube.Run();

  if ( FluoTube.WiFi.Available() )
    Parser ( FluoTube.WiFi.RestRead() ); 
}

void Parser (String data)
{
  if (data == "/l13on")
  {
    digitalWrite(13, HIGH);
  }   
  if (data == "/l13off")
  {
    digitalWrite(13, LOW);
  }
  
  if(data == "ADDR") //default handle to show IP ADDR!!!
  {
    Serial.println("Inviare Richieste REST all indirizzo: ");
    Serial.println( FluoTube.WiFi.Info() );
  }
}
