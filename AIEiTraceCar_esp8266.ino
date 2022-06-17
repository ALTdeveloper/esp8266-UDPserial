#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "NachoNeko"//改成你的热点名字，要英文且是2.4GHz的热点
#define STAPSK  "12345678"//热点密码
#endif

unsigned int localPort = 8888;      // local port to listen on
char  Lazurit[] = "abababab";

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back
String comdata = "";//字符串函数

WiFiUDP Udp;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.print(packetBuffer);//打印comdata数据,给stm32
  }

  if (Serial.available() > 0)//判读是否串口有数据
  {
    String comdata = "";//缓存清零
    while (Serial.available() > 0)//循环串口是否有数据
    {
      comdata += char(Serial.read());//叠加数据到comdata
      delay(2);//延时等待响应
    }
    if (comdata.length() > 0)//如果comdata有数据
    {
      char string_tmp[128] = "0";
      int data_len = 0;
      while (comdata[data_len] != '\0')//把除了\0以外的东西转到char[]
      {
        string_tmp[data_len] = comdata[data_len];
        data_len++;
      }
      string_tmp[data_len] = '\n';


      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket("192.168.43.157",8888); //"192.168.43.80"接收端ip,8888接收端端口号port,看连接的热点现场配
      Udp.write(string_tmp);
      Udp.endPacket();
    }
  }

}
