from http import client
import serial
import socket
import struct
from threading import Thread
import random

import rospy
from std_msgs.msg import Bool

class SerialData:
    def __init__(self):
        self.sensorData = [0 for _ in range(5)]

        thread = Thread(target=self.handleSerial)
        thread.daemon = True
        thread.start()

    def handleSerial(self):
        ser = serial.Serial('/dev/ttyUSB1', 115200)

        while True:
            data = ser.readline()
            numbers = data.split(b',') # 按逗号分隔数据
            numbers = [int(n) for n in numbers]
            check = numbers[-1] # 获取校验码
            checksum = sum(numbers[:-1]) & 0x0F

            if checksum == check:
                print('verify OK')
                self.sensorData = numbers[:-1]
            else:
                print('verify ERROR')

    @property
    def score(self):
        return self.sensorData[0] 
    @property
    def CO2(self):
        return self.sensorData[1]
    @property
    def TVOC(self):
        return self.sensorData[2]
    @property
    def T(self):
        return self.sensorData[3]
    @property
    def H(self):
        return self.sensorData[4]

if __name__ == '__main__':
    serialMonitor = SerialData()
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(('',1234))
    soc.listen(5)

    client, _ = soc.accept()

    rospy.init_node('data_transfer')
    pub = rospy.Publisher('person_detected', Bool, queue_size=1)
    rate = rospy.Rate(10) # 10HZ

    while True:
        # 判断EM9D是否检测到人，发送信号进行进一步处理
        if serialMonitor.score > 0:
            pub.publish(True)
        else:
            pub.publish(False)

        # 发送传感器数据
        client.sendall(struct.pack('4i', serialMonitor.CO2, serialMonitor.TVOC, serialMonitor.T, serialMonitor.H))
        rate.sleep()