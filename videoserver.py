import cv2
import numpy as np
import socket
import time
import rospy
from sensor_msgs.msg import Image, CompressedImage
from cv_bridge import CvBridge

def imageCallBack(img):
    global sendImage, imgOK

    imgOK = True
    # sendImage = CvBridge().imgmsg_to_cv2(img, img.encoding)
    sendImage = CvBridge().compressed_imgmsg_to_cv2(img)
    sendImage = cv2.resize(sendImage, (640, 480))

# imageTopic = '/camera/color/image_raw'
imageTopic = '/object_detect/compressed'
imgOK = False
sendImage = CompressedImage()
rospy.init_node('send_image')
sub = rospy.Subscriber(imageTopic, CompressedImage, imageCallBack)

# 创建socket对象
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('', 1235)
sock.bind(server_address)
sock.listen(5)

conn, addr = sock.accept()
print('connected to %s'%str(addr))

# 等待客户端连接并启动新线程
while not rospy.is_shutdown():
    if imgOK:
        imgOK = False

        ratio = 90
        params = [cv2.IMWRITE_JPEG_QUALITY, ratio]  # ratio:0~100
        _, img = cv2.imencode('.jpg', sendImage, params)

        # 将JPEG格式的图像数据通过socket发送到客户端
        img_data = np.array(img).tobytes()
        conn.sendall(len(img_data).to_bytes(4, byteorder='little'))
        time.sleep(0.02)
        conn.sendall(img_data)

        # 等待客户端接收图像并返回ACK信号
        ack = conn.recv(3)

    rospy.wait_for_message(imageTopic, CompressedImage, timeout=None)
