#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
#include <Thermistor.h>
int led = 4;
Servo webserver;
int pos = 0;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //Mac
byte ip[] = { 192, 168, 0, 112 };                      // ip para o arduino
byte gateway[] = { 192, 168, 0, 1 };                   // gateway
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(8880);                           //porta para o webserver
String readString;
Thermistor temp(0);

void setup() {
  Serial.begin(9600);
   while (!Serial) {  // espera o serial - [Leonardo]
    ;
  }
  pinMode(led, OUTPUT);
  webserver.attach(7); // inicia microserver
  Ethernet.begin(mac, ip, gateway, subnet);  //inicia ethernet
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  EthernetClient client = server.available();   // Faz a conexão com o cliente
  int temperature = temp.getTemp();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (readString.length() < 100) {
          readString += c;
         }

         if (c == '\n') { //Caso o request acabe
           Serial.println(readString); //Imprime o status no serial

           client.println("HTTP/1.1 200 OK"); //gera nova página
           client.println("Content-Type: text/html");
           client.println();
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Projeto Arduino residencial</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H2>Temperatura Atual: ");
           client.println(temperature);
           client.println("&deg;C</H2>");
           client.println("<H1>Arduino residencial</H1>");
           client.println("<hr />");
           client.println("<br />");
           client.println("<H2>Sistemas em tempo real</H2>");
           client.println("<br />");
           client.println("<a href=\"/?button1on\"\">Ligar Led</a>");
           client.println("<a href=\"/?button1off\"\">Apagar Led</a><br />");
           client.println("<br />");
           client.println("<br />");
           client.println("</BODY>");
           client.println("</HTML>");

           delay(1);
           client.stop();
           //Controler de botões
           if (readString.indexOf("?button1on") >0){
               digitalWrite(led, HIGH);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(led, LOW);
           }
            //Limpa a tela
            readString="";

         }
       }
    }
}
}
