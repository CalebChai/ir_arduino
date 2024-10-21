import serial
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# 串口初始化，波特率为115200，超时时间为0.05秒
ser = serial.Serial('COM21', 115200, timeout=0.05)

# 延迟一段时间以确保串口连接稳定
time.sleep(2)

# 设置条形图的标签
labels = ['Chip 1 AIN0', 'Chip 1 AIN1', 'Chip 1 AIN2', 'Chip 1 AIN3',
          'Chip 2 AIN0', 'Chip 2 AIN1', 'Chip 2 AIN2', 'Chip 2 AIN3',
          'Chip 3 AIN0', 'Chip 3 AIN1', 'Chip 3 AIN2', 'Chip 3 AIN3']

# 初始化空的电压列表
voltages = [0] * 12

# 创建条形图对象
fig, ax = plt.subplots()
bars = ax.bar(labels, voltages)

# 设置图表标题和标签
ax.set_title('Voltage Readings from 3 ADS1115 Chips')
ax.set_ylabel('Voltage (V)')
ax.set_ylim(0, 5)  # 将Y轴范围固定为0到5伏
plt.xticks(rotation=45, ha="right")  # 使标签倾斜以便更好地显示
plt.tight_layout()

def read_voltage_data():
    voltages = []
    start_flag = False  # 标志数据是否开始传输

    while True:
        try:
            # 从串口读取一行数据并解码
            line = ser.readline().decode('utf-8').strip()

            # 检查起始标志
            if line == "START":
                voltages = []  # 开始新一组电压数据
                start_flag = True
            elif line == "END" and start_flag:
                # 如果接收到 END 标志并且已经开始了数据读取
                if len(voltages) == 12:
                    return voltages  # 返回成功读取的12个电压值
                else:
                    print("Error: Incomplete data, expected 12 values but got", len(voltages))
                    voltages = []  # 重置电压数据
            elif start_flag and ' V' in line:
                # 提取电压值，例如 "Chip 1 AIN0: 3.123 V"
                voltage = float(line.split(': ')[1].replace(' V', ''))
                voltages.append(voltage)

        except Exception as e:
            print(f"Error reading data: {e}")
            break

def update_bars(frame):
    global voltages

    # 读取12个电压值
    new_voltages = read_voltage_data()
    
    # 检查是否成功读取到12个电压值
    if len(new_voltages) == 12:
        voltages = new_voltages  # 更新电压值
        for bar, new_voltage in zip(bars, voltages):
            bar.set_height(new_voltage)  # 更新每个条的高度
    
    return bars

# 使用 FuncAnimation 实时更新图表
ani = FuncAnimation(fig, update_bars, interval=50)  # 每1000ms (1秒) 更新一次

# 显示图表
plt.show()
