#include <Arduino.h>
#include <WiFi.h>

int m1t1_Motor1 = 22;
int m1t2_Motor1 = 23;

int m1t1_Motor2 = 26;
int m1t2_Motor2 = 27;

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

    Serial.println(request)
    
    if (request.indexOf("/move=") != -1) {
      int pos = request.indexOf("/move=") + 6; // posição do comando na string
      String comando = request.substring(pos, request.indexOf("?"));
      
      int powerPos = request.indexOf("power=") + 6; // posição do valor de potência
      int power = request.substring(powerPos).toInt();

      controlarMovimento(comando, power);
    }
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta charset=\"utf-8\"/>");
    client.println("<style>");
    //button css
    client.println("button {");
    client.println("user-select: none;"); 
    client.println("-webkit-user-select: none;");
    client.println("touch-action: manipulation;"); 
    client.println("background-color: black;"); 
    client.println("color: white;");
    client.println("width: 200px;");
    client.println("font-size: 30px;");
    client.println("border: none;");
    client.println("border-radius: 25px;");
    client.println("}");
    client.println("button:active {");
    client.println("background-color: blue;"); 
    client.println("}");
    client.println("button:focus {");
    client.println("background-color: gray;"); 
    client.println("}");
    //body css
    client.println("body {");
    client.println("margin: 0 auto;");
    client.println("display: flex;");
    client.println("flex-direction: column;");
    client.println("align-items: center;");
    client.println("}");
    //div css
    client.println("div {");
    client.println("display: grid;");
    client.println("grid-template-columns: repeat(3, 1fr);");
    client.println("grid-gap: 20px;");
    client.println("place-items: center;");
    client.println("}");
    //input css
    client.println("input {");
    client.println("width: 150px;");
    client.println("height: 60px;");
    client.println("font-size: 35px;");
    client.println("text-align: center;");
    client.println("}");
    client.println("</style>");
    client.println("</head>");
    //body html
    client.println("<body><h1>ESP32 Web Server</h1>");
    //input do valor da potencia do motor
    client.println("<p><input type='number' id='power' min='0' max='255' value='0'></p>");
    //northwest
    client.println("<button onmousedown=\"fetch('/move=northwest?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=northwest?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↖</button>");
    //foward
    client.println("<button onmousedown=\"fetch('/move=foward?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=foward?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↑</button>");
    //northeast
    client.println("<button onmousedown=\"fetch('/move=northeast?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=northeast?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↗</button>");    
    //left
    client.println("<button onmousedown=\"fetch('/move=left?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=left?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">←</button>");
    //stop
    client.println("<button onmousedown=\"fetch('/move=stop?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=stop?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">■</button>");
    //right
    client.println("<button onmousedown=\"fetch('/move=right?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=right?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">→</button>");
    //southwest
    client.println("<button onmousedown=\"fetch('/move=southwest?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=southwest?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↙</button>");
    //backward
    client.println("<button onmousedown=\"fetch('/move=backward?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=backward?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↓</button>");
    //southeast
    client.println("<button onmousedown=\"fetch('/move=southeast?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=southeast?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↘</button>");    
    client.println("</body></html>");
    delay(1);
  }
}

void controlarMovimento(String comando, int potencia) {
  if (comando == "stop") {
    // Parar ambos os motores
    analogWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, LOW);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, LOW);
  } else if (comando == "forward") {
    // Movimento para frente
    analogWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, potencia);
  } else if (comando == "backward") {
    // Movimento para trás
    analogWrite(m1t1_Motor1, potencia);
    analogWrite(m1t2_Motor1, LOW);
    analogWrite(m1t1_Motor2, potencia);
    analogWrite(m1t2_Motor2, LOW);
  } else if (comando == "left") {
    // Movimento para a esquerda
    analogWrite(m1t1_Motor1, potencia);
    analogWrite(m1t2_Motor1, LOW);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, potencia);
  } else if (comando == "right") {
    // Movimento para a direita
    analogWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    analogWrite(m1t1_Motor2, potencia);
    analogWrite(m1t2_Motor2, LOW);
  } else if (comando == "northwest") {
    // Movimento para o noroeste
    analogWrite(m1t1_Motor1, potencia);
    analogWrite(m1t2_Motor1, LOW);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, LOW);
  } else if (comando == "northeast") {
    // Movimento para o nordeste
    analogWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, LOW);
  } else if (comando == "southwest") {
    // Movimento para o sudoeste
    analogWrite(m1t1_Motor1, potencia);
    analogWrite(m1t2_Motor1, LOW);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, LOW);
  } else if (comando == "southeast") {
    // Movimento para o sudeste
    analogWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    analogWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, LOW);
  }
}
