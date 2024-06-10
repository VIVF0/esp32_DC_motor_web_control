#include <Arduino.h>
#include <ESP8266WiFi.h>

int m1t1_Motor1 = 5;
int m1t2_Motor1 = 16;

int m1t1_Motor2 = 2;
int m1t2_Motor2 = 4;

int potencia = 255;
double mult1 = 0.5;
double mult2 = 0.75;

double comando[9][4] = {
  // stop 0
  {0, 0, 0, 0},
  // frente 1
  {0, potencia, 0, potencia},
  // trás 2
  { potencia, 0, potencia, 0},
  // direita 3
  {0, potencia*mult1, potencia, 0},
  // esquerda 4
  {potencia, 0, 0, potencia*mult1},
  // noroeste 5
  {0, potencia*mult2, 0, potencia},
  // nordeste 6
  {0, potencia, 0, potencia*mult2},
  // sudeste 7
  {potencia*mult2, 0, potencia, 0},
  // sudoeste 8
  {potencia, 0, potencia*mult2, 0},
};

const char* ssid = "ESP32AP";
const char* password = "123456789";

WiFiServer server(8080);

void setup() {
  Serial.begin(115200);
  pinMode(m1t1_Motor1, OUTPUT);
  pinMode(m1t2_Motor1, OUTPUT);
  pinMode(m1t1_Motor2, OUTPUT);
  pinMode(m1t2_Motor2, OUTPUT);

  WiFi.softAP(ssid, password);
  delay(100); // Delay para o AP se configurar corretamente
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Endereço IP do AP: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    Serial.println(request);
    
    if (request.indexOf("/move/") != -1) {
      int pos = request.indexOf("/move/") + 6; // posição do comando na string
      int comandoIndex = request.substring(pos, request.indexOf("?")).toInt();

      analogWrite(m1t1_Motor1, comando[comandoIndex][0]);
      analogWrite(m1t2_Motor1, comando[comandoIndex][1]); 
      analogWrite(m1t1_Motor2, comando[comandoIndex][2]); 
      analogWrite(m1t2_Motor2, comando[comandoIndex][3]);
    }
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.print("<!DOCTYPE HTML><html><head><meta charset=\"utf-8\"/><title>ESP32 Web Server</title>");
    client.print("<style>body{width:100vw;height:100vh;margin:0;padding:0;display:flex;flex-direction:column;align-items:center;justify-content:space-evenly;color-scheme:dark;background-color:#222222;color:white;}h1{font-size:2rem;}button{user-select:none;-webkit-user-select:none;touch-action:manipulation;background-color:black;width:100%;height:90%;padding:2rem;font-size:5rem;border:none;border-radius:1.5rem;display:flex;align-items:center;justify-content:center;}button:active{background-color:blue;}button:focus{background-color:gray;}h1{font-size:4rem;}input{width:20rem;height:5rem;font-size:3rem;text-align:center;}div{height:40%;width:80%;display:grid;grid-template-columns:repeat(3,1fr);grid-gap:1rem;place-items:center;}@media screen and (min-width:100vh){body{flex-direction:row;}button{font-size:2rem;width:8rem;}h1{font-size:2rem;}div,input{height:auto;width:auto;font-size:2rem;}}</style>");
    client.print("<script>const move=(direction)=>fetch(`/move/${direction}?`);</script>");
    client.print("</head><body><header style=\"display:flex;gap:2rem;flex-direction:column;align-items:center;\"><h1>ESP32 Web Server</h1></header><div><button onmousedown=\"move('5')\" onmouseup=\"move('0')\" ontouchstart=\"move('5')\" ontouchend=\"move('0')\">↖</button><button onmousedown=\"move('1')\" onmouseup=\"move('0')\" ontouchstart=\"move('1')\" ontouchend=\"move('0')\">↑</button><button onmousedown=\"move('6')\" onmouseup=\"move('0')\" ontouchstart=\"move('6')\" ontouchend=\"move('0')\">↗</button><button onmousedown=\"move('4')\" onmouseup=\"move('0')\" ontouchstart=\"move('4')\" ontouchend=\"move('0')\">←</button><button onmousedown=\"move('0')\" ontouchstart=\"move('0')\">■</button><button onmousedown=\"move('3')\" onmouseup=\"move('0')\" ontouchstart=\"move('3')\" ontouchend=\"move('0')\">→</button><button onmousedown=\"move('8')\" onmouseup=\"move('0')\" ontouchstart=\"move('8')\" ontouchend=\"move('0')\">↙</button><button onmousedown=\"move('2')\" onmouseup=\"move('0')\" ontouchstart=\"move('2')\" ontouchend=\"move('0')\">↓</button><button onmousedown=\"move('7')\" onmouseup=\"move('0')\" ontouchstart=\"move('7')\" ontouchend=\"move('0')\">↘</button></div></body></html>");
    delay(1);
  }
}
