#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ArduinoJson.h>  // 引入 Arduino JSON 库

const uint16_t recv_pin = D2;  // ESP8266 D2 腳連接 IR 接收器

IRrecv irrecv(recv_pin);  // 設置接收引腳
decode_results results;  // 儲存解碼後的 IR 信號

void setup()
{
  Serial.begin(115200);  // 初始化串口監視器
  irrecv.enableIRIn();  // 啟動 IR 接收功能
  Serial.println("IR Receiver is ready");
}

void loop()
{
  if (irrecv.decode(&results))  // 檢查是否有接收到 IR 信號
  {
    // 創建一個 JSON 文件
    StaticJsonDocument<256> doc;

    // 將紅外線信號值保存為 JSON 物件
    doc["signal"] = results.value;  // 接收到的信號值
    doc["rawlen"] = results.rawlen;  // 信號的長度

    // 創建一個數組來保存脈衝時間
    JsonArray pulses = doc.createNestedArray("pulses");
    for (int i = 1; i < results.rawlen; i++) {
      pulses.add(results.rawbuf[i]);  // 添加每個脈衝的時間
    }

    // 將結果轉換為 JSON 字符串並顯示
    String output;
    serializeJson(doc, output);  // 將 JSON 物件轉換為字符串
    Serial.println(output);  // 顯示 JSON 字符串

    irrecv.resume();  // 準備接收下一個信號
  }
}
