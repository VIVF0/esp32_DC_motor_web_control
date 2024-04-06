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

    Serial.println(request);
    
    if (request.indexOf("/move/") != -1) {
      int pos = request.indexOf("/move/") + 6; // posição do comando na string
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
    client.println("<title>ESP32 Web Server</title>");
    // Styles css
    client.println("<style>");
    client.println("body{");
    client.println("width:100vw;");
    client.println("height:100vh;");
    client.println("margin:0;");
    client.println("padding:0;");
    client.println("display:flex;");
    client.println("flex-direction:column;");
    client.println("align-items:center;");
    client.println("justify-content:space-evenly;");
    client.println("color-scheme:dark;");
    client.println("background-color:#222222;");
    client.println("color:white;");
    client.println("}");
    client.println("h1{");
    client.println("font-size:2rem;");
    client.println("}");
    client.println("button{");
    client.println("user-select:none;");
    client.println("-webkit-user-select:none;");
    client.println("touch-action:manipulation;");
    client.println("background-color:black;");
    client.println("width:100%;");
    client.println("height:90%;");
    client.println("padding:2rem;");
    client.println("font-size:5rem;");
    client.println("border:none;");
    client.println("border-radius:1.5rem;");
    client.println("display:flex;");
    client.println("align-items:center;");
    client.println("justify-content:center;");
    client.println("}");
    client.println("button:active{");
    client.println("background-color:blue;");
    client.println("}");
    client.println("button:focus{");
    client.println("background-color:gray;");
    client.println("}");
    client.println("h1{");
    client.println("font-size:4rem;");
    client.println("}");
    client.println("input{");
    client.println("width:20rem;");
    client.println("height:5rem;");
    client.println("font-size:3rem;");
    client.println("text-align:center;");
    client.println("}");
    client.println("div{");
    client.println("height:40%;");
    client.println("width:80%;");
    client.println("display:grid;");
    client.println("grid-template-columns:repeat(3,1fr);");
    client.println("grid-gap:1rem;");
    client.println("place-items:center;");
    client.println("}");
    client.println("@media screen and (min-width:100vh){");
    client.println("body{");
    client.println("flex-direction:row;");
    client.println("}");
    client.println("button{");
    client.println("font-size:2rem;");
    client.println("width:8rem;");
    client.println("}");
    client.println("h1{");
    client.println("font-size:2rem;");
    client.println("}");
    client.println("div,input{");
    client.println("height:auto;");
    client.println("width:auto;");
    client.println("font-size:2rem;");
    client.println("}");
    client.println("}");
    client.println("</style>");
    client.println("<script>");
    client.println("const move=(direction)=>fetch(`/move/${direction}?power=${document.getElementById('power').value}`);");
    client.println("const stop=()=>fetch('/move/stop');");
    client.println("</script>");
    //body html
    client.println("</head><body>");
    //input do valor da potencia do motor
    client.println("<header style=\"display:flex;gap:2rem;flex-direction:column;align-items:center;\">");
    client.println("<h1>ESP32 Web Server</h1>");
    client.println("<p><input type='number' id='power' min='0' max='255' value='0'></p>");
    client.println("</header><div>");
    client.println("<button onmousedown=\"move('northwest')\" onmouseup=\"stop()\" ontouchstart=\"move('northwest')\" ontouchend=\"stop()\">↖</button>");
    client.println("<button onmousedown=\"move('forward')\" onmouseup=\"stop()\" ontouchstart=\"move('forward')\" ontouchend=\"stop()\">↑</button>");
    client.println("<button onmousedown=\"move('northeast')\" onmouseup=\"stop()\" ontouchstart=\"move('northeast')\" ontouchend=\"stop()\">↗</button>");
    client.println("<button onmousedown=\"move('left')\" onmouseup=\"stop()\" ontouchstart=\"move('left')\" ontouchend=\"stop()\">←</button>");
    client.println("<button onmousedown=\"stop()\" ontouchstart=\"stop()\">■</button>");
    client.println("<button onmousedown=\"move('right')\" onmouseup=\"stop()\" ontouchstart=\"move('right')\" ontouchend=\"stop()\">→</button>");
    client.println("<button onmousedown=\"move('southwest')\" onmouseup=\"stop()\" ontouchstart=\"move('southwest')\" ontouchend=\"stop()\">↙</button>");
    client.println("<button onmousedown=\"move('backward')\" onmouseup=\"stop()\" ontouchstart=\"move('backward')\" ontouchend=\"stop()\">↓</button>");
    client.println("<button onmousedown=\"move('southeast')\" onmouseup=\"stop()\" ontouchstart=\"move('southeast')\" ontouchend=\"stop()\">↘</button>");
    client.println("</div></body></html>");
    delay(1);
  }
}

void controlarMovimento(String comando, int potencia) {
  if (comando == "stop") {
    // Parar ambos os motores
    digitalWrite(m1t1_Motor1, LOW);
    digitalWrite(m1t2_Motor1, LOW);
    digitalWrite(m1t1_Motor2, LOW);
    digitalWrite(m1t2_Motor2, LOW);
  } else if (comando == "forward") {
    // Movimento para frente
    digitalWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    digitalWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, potencia);
  } else if (comando == "backward") {
    // Movimento para trás
    analogWrite(m1t1_Motor1, potencia);
    digitalWrite(m1t2_Motor1, LOW);
    analogWrite(m1t1_Motor2, potencia);
    digitalWrite(m1t2_Motor2, LOW);
  } else if (comando == "left") {
    // Movimento para a esquerda
    analogWrite(m1t1_Motor1, potencia);
    digitalWrite(m1t2_Motor1, LOW);
    digitalWrite(m1t1_Motor2, LOW);
    analogWrite(m1t2_Motor2, potencia);
  } else if (comando == "right") {
    // Movimento para a direita
    digitalWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    analogWrite(m1t1_Motor2, potencia);
    digitalWrite(m1t2_Motor2, LOW);
  } else if (comando == "northwest") {
    // Movimento para o noroeste
    analogWrite(m1t1_Motor1, potencia);
    digitalWrite(m1t2_Motor1, LOW);
    digitalWrite(m1t1_Motor2, LOW);
    digitalWrite(m1t2_Motor2, LOW);
  } else if (comando == "northeast") {
    // Movimento para o nordeste
    digitalWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    digitalWrite(m1t1_Motor2, LOW);
    digitalWrite(m1t2_Motor2, LOW);
  } else if (comando == "southwest") {
    // Movimento para o sudoeste
    analogWrite(m1t1_Motor1, potencia);
    digitalWrite(m1t2_Motor1, LOW);
    digitalWrite(m1t1_Motor2, LOW);
    digitalWrite(m1t2_Motor2, LOW);
  } else if (comando == "southeast") {
    // Movimento para o sudeste
    digitalWrite(m1t1_Motor1, LOW);
    analogWrite(m1t2_Motor1, potencia);
    digitalWrite(m1t1_Motor2, LOW);
    digitalWrite(m1t2_Motor2, LOW);
  }
}
