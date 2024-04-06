#include <Arduino.h>
#include <WiFi.h>

int m1t1 = 22;
int m1t2 = 23;

int m1t1_Motor2 = 26;
int m1t2_Motor2 = 27;

const char* ssid = "ESP32AP";
const char* password = "123456789";

WiFiServer server(8080);

void setup() {
  Serial.begin(115200);
  pinMode(m1t1, OUTPUT);
  pinMode(m1t2, OUTPUT);
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

    int pos = request.indexOf("/move=foward?power=");
    if (pos != -1) {
      // min 126, max 255
      int power = request.substring(23, 26).toInt();
      
      analogWrite(m1t1, LOW);
      analogWrite(m1t2, power);
      analogWrite(m1t1_Motor2, LOW);
      analogWrite(m1t2_Motor2, power);
    }
    if (request.indexOf("/move=stop") != -1) {
      analogWrite(m1t1, LOW);
      analogWrite(m1t2, LOW);
      analogWrite(m1t1_Motor2, LOW);
      analogWrite(m1t2_Motor2, LOW);
    }
    pos = request.indexOf("/move=backward?power=");
    if (pos != -1) {
      // min 127, max 255
      int power = request.substring(25, 28).toInt();
      
      analogWrite(m1t2, LOW);
      analogWrite(m1t1, power);
      analogWrite(m1t2_Motor2, LOW);
      analogWrite(m1t1_Motor2, power);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<style>");
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
    client.println("body {");
    client.println("margin: 0 auto;");
    client.println("display: flex;");
    client.println("flex-direction: column;");
    client.println("align-items: center;");
    client.println("}");
    client.println("input {");
    client.println("width: 150px;");
    client.println("height: 60px;");
    client.println("font-size: 35px;");
    client.println("text-align: center;");
    client.println("}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body><h1>ESP32 Web Server</h1>");
    client.println("<p><input type='number' id='power' min='0' max='255' value='0'></p>");
    client.println("<p><button onmousedown=\"fetch('/move=foward?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=foward?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↑</button></p>""");
    client.println("<p><button onmousedown=\"fetch('/move=left?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=left?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">←</button>""");
    client.println("<button onmousedown=\"fetch('/move=right?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=right?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">→</button></p>""");
    client.println("<button onmousedown=\"fetch('/move=stopt?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=stop?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">■</button>""");
    client.println("<p><button onmousedown=\"fetch('/move=backward?power=' + document.getElementById('power').value)\" onmouseup=\"fetch('/move=stop')\" ontouchstart=\"fetch('/move=backward?power=' + document.getElementById('power').value)\" ontouchend=\"fetch('/move=stop')\">↓</button></p>");
    client.println("</body></html>");
    delay(1);
  }
}
