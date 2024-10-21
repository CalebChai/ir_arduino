#include <Adafruit_ADS1X15.h>

// 创建 ADS1115 对象
Adafruit_ADS1115 ads1;  // 地址为 0x48 的芯片
Adafruit_ADS1115 ads2;  // 地址为 0x49 的芯片
Adafruit_ADS1115 ads3;  // 地址为 0x4A 的芯片

void setup(void) {
  Serial.begin(115200);
  Serial.println("Initializing multiple ADS1115...");

  // 初始化每个 ADS1115 模块，传入 I2C 地址
  if (!ads1.begin(0x49)) {
    Serial.println("Failed to initialize ADS1115 (0x48).");
    while (1);  // 如果初始化失败，停止执行
  }
  
  if (!ads2.begin(0x48)) {
    Serial.println("Failed to initialize ADS1115 (0x49).");
    while (1);  // 如果初始化失败，停止执行
  }

  if (!ads3.begin(0x4A)) {
    Serial.println("Failed to initialize ADS1115 (0x4A).");
    while (1);  // 如果初始化失败，停止执行
  }

  // 设置每个 ADS1115 的增益，确保适应输入电压范围
  ads1.setGain(GAIN_TWOTHIRDS);  // ±6.144V
  ads2.setGain(GAIN_TWOTHIRDS);  // ±6.144V
  ads3.setGain(GAIN_TWOTHIRDS);  // ±6.144V
}

void loop(void) {
  // 定义数组存储每个芯片的4个通道电压值
  int16_t adc1[4], adc2[4], adc3[4];
  float volts1[4], volts2[4], volts3[4];

  // 读取芯片1 (地址 0x48) 的4个通道
  for (int i = 0; i < 4; i++) {
    adc1[i] = ads1.readADC_SingleEnded(i);
    volts1[i] = adc1[i] * 0.1875F / 1000.0;  // 默认增益下，每位0.1875mV，转换为伏特
  }

  // 读取芯片2 (地址 0x49) 的4个通道
  for (int i = 0; i < 4; i++) {
    adc2[i] = ads2.readADC_SingleEnded(i);
    volts2[i] = adc2[i] * 0.1875F / 1000.0;
  }

  // 读取芯片3 (地址 0x4A) 的4个通道
  for (int i = 0; i < 4; i++) {
    adc3[i] = ads3.readADC_SingleEnded(i);
    volts3[i] = adc3[i] * 0.1875F / 1000.0;
  }
  Serial.println("START");  // 数据开始标志
  // 输出芯片1的电压值到串口监视器
  for (int i = 0; i < 4; i++) {
    Serial.print("Chip 1 AIN"); Serial.print(i); Serial.print(": ");
    Serial.print(volts1[i]); Serial.println(" V");
  }

  // 输出芯片2的电压值到串口监视器
  for (int i = 0; i < 4; i++) {
    Serial.print("Chip 2 AIN"); Serial.print(i); Serial.print(": ");
    Serial.print(volts2[i]); Serial.println(" V");
  }

  // 输出芯片3的电压值到串口监视器
  for (int i = 0; i < 4; i++) {
    Serial.print("Chip 3 AIN"); Serial.print(i); Serial.print(": ");
    Serial.print(volts3[i]); Serial.println(" V");
  }
  Serial.println("END");  // 数据结束标志
  Serial.println("");  // 添加换行以便更容易阅读
  delay(50);  // 延时1秒，避免输出过快
}
