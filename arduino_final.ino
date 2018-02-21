// Programa : Sistema de alarme
#include <SoftwareSerial.h>

#define DebugSerial  Serial
SoftwareSerial GprsSerial(10,11); // RX, TX

int pir1 = 3;  //Pino ligado ao sensor PIR Norte
int pir2 = 4;  //Pino ligado ao sensor PIR Sul
int pir3 = 5;  //Pino ligado ao sensor PIR Leste
int pir4 = 6;  //Pino ligado ao sensor PIR Oeste
int PinA0 = A0; //Sensor de gás/fumaça
int leitura_sensor = 300;
int Leds = 7; //Pino ligado aos LEDs de alarme
int Buzz = 8; //Pino Ligado às buzinas de alarme
int acionamentoNorte;  //Variavel para guardar valor do sensor
int acionamentoSul;  //Variavel para guardar valor do sensor
int acionamentoLeste;  //Variavel para guardar valor do sensor
int acionamentoOeste;  //Variavel para guardar valor do sensor
int ldr = A2;    // selecione o pino de entrada ao LDR
int valdr;
int ldrlamp = 9;
void setup()
{
  pinMode(pir1, INPUT);   //Define pino sensor como entrada
  pinMode(pir2, INPUT); 
  pinMode(pir3, INPUT); 
  pinMode(pir4, INPUT); 
  pinMode(PinA0, INPUT);
  pinMode(Leds, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(Buzz, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(ldr, INPUT);
  pinMode(ldrlamp, OUTPUT);
  digitalWrite(pir1, LOW);
  digitalWrite(pir2, LOW);
  digitalWrite(pir3, LOW);
  digitalWrite(pir4, LOW);
  digitalWrite(Buzz, LOW);
  digitalWrite(Leds, LOW);
  
  DebugSerial.begin(9600); // the serial monitor baud rate
  GprsSerial.begin(9600); // the GPRS baud rate
}

void SendTextMessageNorte()
{
  DebugSerial.print("AT+CMGF=1\r"); //Para mandar em modo Texto
  delay(100);
  DebugSerial.println("AT+CMGS=+5571991644692"); // envia o sms
  delay(100);
  DebugSerial.println("Movimento Detectado Corredor Norte!!"); //Conteudo da mensagem
  delay(100);
  DebugSerial.println((char)26); //codigo ASCII dp ctrl+z é 26
  delay(100);
  DebugSerial.println();
}
void SendTextMessageSul()
{
  DebugSerial.print("AT+CMGF=1\r"); 
  delay(100);
  DebugSerial.println("AT+CMGS=+5571991644692");
  delay(100);
  DebugSerial.println("Movimento Detectado Corredor Sul!!");
  delay(100);
  DebugSerial.println((char)26);
  delay(100);
  DebugSerial.println();
}
void SendTextMessageLeste()
{
  DebugSerial.print("AT+CMGF=1\r"); 
  delay(100);
  DebugSerial.println("AT+CMGS=+5571991644692");
  delay(100);
  DebugSerial.println("Movimento Detectado Corredor Leste!!");
  delay(100);
  DebugSerial.println((char)26);
  delay(100);
  DebugSerial.println();
}
void SendTextMessageOeste()
{
  DebugSerial.print("AT+CMGF=1\r"); 
  delay(100);
  DebugSerial.println("AT+CMGS=+5571991644692"); 
  
  DebugSerial.println("Movimento Detectado Corredor Oeste!!");
  delay(100);
  DebugSerial.println((char)26);
  delay(100);
  DebugSerial.println();
}
void SendTextMessageGas()
{
  DebugSerial.print("AT+CMGF=1\r"); 
  delay(100);
  DebugSerial.println("AT+CMGS=+5571991644692"); 
  
  DebugSerial.println("Vazamento de Gás!!");
  delay(100);
  DebugSerial.println((char)26);
  delay(100);
  DebugSerial.println();
}
void DialVoiceCall()
{
  DebugSerial.println("AT+SNFS=0");
  delay(100);
  DebugSerial.println("ATD+5571991644692;"); //Disca o numero
  delay(100);
  DebugSerial.println();
}

void loop()
{
 valdr = analogRead(ldr);    // ler o valor do potenciômetro
 Serial.println(valdr);            //emvia valor para o pc
 if(valdr >= 110){
  digitalWrite(ldrlamp, LOW); 
 }
 else {
  digitalWrite(ldrlamp, HIGH);
  }
 acionamentoNorte = digitalRead(pir1); //Le o valor do sensor PIR
 acionamentoSul = digitalRead(pir2); //Le o valor do sensor PIR
 acionamentoLeste = digitalRead(pir3); //Le o valor do sensor PIR
 acionamentoOeste = digitalRead(pir4); //Le o valor do sensor PIR
 int valor_analogico = analogRead(PinA0);
 Serial.print("Leitura: ");
 Serial.println(valor_analogico);
 
  if (valor_analogico > leitura_sensor){//SE VALOR LIDO NO PINO ANALÓGICO FOR MAIOR QUE O VALOR LIMITE, FAZ 
 
 SendTextMessageGas();
  digitalWrite(Leds, HIGH);
  digitalWrite(Buzz, HIGH);
  delay(5000);
  DebugSerial.println("vazamento de gás detectado!");
  digitalWrite(Leds, LOW);
  digitalWrite(Buzz, LOW);
  delay (5000);
  DialVoiceCall();
 }else{ 
 digitalWrite(Buzz, LOW);//BUZZER DESLIGADO
 digitalWrite(Leds, LOW);
 }
 delay(500); //INTERVALO DE 100 MILISSEGUNDOS
 
 if (acionamentoNorte == HIGH)  //Movimento detectado no corredor Norte
 {
  SendTextMessageNorte();
  digitalWrite(Leds, HIGH);
  digitalWrite(Buzz, HIGH);
  delay(5000);
  DebugSerial.println("acionou norte");
  digitalWrite(Leds, LOW);
  digitalWrite(Buzz, LOW);
  delay (5000);
  DialVoiceCall();
 }else{ 
 digitalWrite(Buzz, LOW);//BUZZER DESLIGADO
 digitalWrite(Leds, LOW);
 }
 if (acionamentoSul == HIGH)  //Movimento detectado no corredor Sul
 {
  SendTextMessageSul();
  digitalWrite(Leds, HIGH);
  digitalWrite(Buzz, HIGH);
  delay(5000);
  DebugSerial.println("acionou sul");
  digitalWrite(Leds, LOW);
  digitalWrite(Buzz, LOW);
  delay(5000);
  DialVoiceCall();
 }else{ 
 digitalWrite(Buzz, LOW);//BUZZER DESLIGADO
 digitalWrite(Leds, LOW);
 }
 if (acionamentoLeste == HIGH)  //Movimento detectado no corredor Leste
 {
  SendTextMessageLeste();
  digitalWrite(Leds, HIGH);
  digitalWrite(Buzz, HIGH);  
  delay(5000);
  DebugSerial.println("acionou Leste");
  digitalWrite(Leds, LOW);
  digitalWrite(Buzz, LOW);
  delay (5000);
  DialVoiceCall();
 }else{ 
 digitalWrite(Buzz, LOW);//BUZZER DESLIGADO
 digitalWrite(Leds, LOW);
 }
 if (acionamentoOeste == HIGH)  //Movimento detectado no corredor Oeste
 {
  SendTextMessageOeste();
  digitalWrite(Leds, HIGH);
  digitalWrite(Buzz, HIGH);
  delay(5000);
  DebugSerial.println("acionou Oeste");
  digitalWrite(Leds, LOW);
  digitalWrite(Buzz, LOW);
  delay (5000);
  DialVoiceCall();
 }
 if ((acionamentoNorte == LOW)&&(acionamentoSul == LOW)&&(acionamentoLeste == LOW)&&(acionamentoOeste == LOW)){
digitalWrite(Leds, LOW);
digitalWrite(Buzz, LOW);
}
 
}
