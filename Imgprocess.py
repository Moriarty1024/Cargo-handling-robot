import pyzbar.pyzbar as pyzbar
import cv2
import numpy as np
import picamera as pca
import time
import os
import collections
from luma.core.interface.serial import i2c, spi
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106
import serial
def readQRcode(x):
    qrdata = ""
    img = cv2.imread(x)
    qr = pyzbar.decode(img)
    for item in qr:
        qrdata = item.data.decode()
    return(qrdata)
def getColorList():
    dict = collections.defaultdict(list)
    lower_red = np.array([156, 43, 46])
    upper_red = np.array([180, 255, 255])
    color_list = []
    color_list.append(lower_red)
    color_list.append(upper_red)
    dict['1']=color_list
    # 红色2
    lower_red = np.array([0, 43, 46])
    upper_red = np.array([10, 255, 255])
    color_list = []
    color_list.append(lower_red)
    color_list.append(upper_red)
    dict['11'] = color_list
    #绿色
    lower_green = np.array([35, 43, 46])
    upper_green = np.array([77, 255, 255])
    color_list = []
    color_list.append(lower_green)
    color_list.append(upper_green)
    dict['2'] = color_list
    #蓝色
    lower_blue = np.array([100, 43, 46])
    upper_blue = np.array([124, 255, 255])
    color_list = []
    color_list.append(lower_blue)
    color_list.append(upper_blue)
    dict['3'] = color_list
    return dict
#处理图片
def judgeColor(frame):
    #print('go in get_color')
    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    maxsum = -100
    color = None
    color_dict = getColorList()
    for d in color_dict:
        mask = cv2.inRange(hsv,color_dict[d][0],color_dict[d][1])
        cv2.imwrite(d+'.jpg',mask)
        binary = cv2.threshold(mask, 127, 255, cv2.THRESH_BINARY)[1]
        binary = cv2.dilate(binary,None,iterations=2)
        img, cnts, hiera = cv2.findContours(binary.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
        sum = 0
        for c in cnts:
            sum+=cv2.contourArea(c)
        if sum > maxsum :
            maxsum = sum
            color = d
    return color
def capturePic():
    cam = pca.PiCamera()
    cam.start_preview()
    time.sleep(2)
    cam.capture("/home/pi/Desktop/COLOR.jpg")
    cam.stop_preview()
    cam.close()
def captureQRcode():
    cam = pca.PiCamera()
    cam.start_preview()
    time.sleep(2)
    cam.capture("/home/pi/Desktop/QRCODE.jpg")
    cam.stop_preview()
    cam.close()
'''def captureimgjudgeclr():
    cam = pca.PiCamera()
    cam.start_preview()
    time.sleep(8)
    cam.capture("/home/pi/Desktop/test.jpg")
    cam.stop_preview()
    img = cv2.imread("/home/pi/Desktop/test.jpg")
    pxc = img[400,300]
    m = max(pxc)
    if m == pxc[0]:
        flag = 3
    elif m ==pxc[1]:
        flag = 2
    else:
        flag = 1
    return flag'''
def display(a,b = "ZZULI"):
    # rev.1 users set port=0
    # substitute spi(device=0, port=0) below if using that interface
    serial = i2c(port=1, address=0x3C)
    # substitute ssd1331(...) or sh1106(...) below if using that device
    device = sh1106(serial)#这里改ssd1306, ssd1325, ssd1331, sh1106
    with canvas(device) as draw:
        draw.rectangle(device.bounding_box,outline="white", fill="black")
        draw.text((55, 16), b, fill="white")
        draw.text((40, 36), "QRcode:"+a, fill="white")
        #draw.text((8, 40), "color distinguish:"+b, fill="white")
def serialParse():
    ser = serial.Serial("/dev/ttyUSB0",9600,timeout = 10)
    serialNum = ser.read(1)
    return serialNum
'''def serialWrite(serialNumber):
    ser = serial.Serial("/dev/ttyUSB0",9600,timeout = 10)
    ser.write(serialNumber)'''
def main():
    while 1:
        ser = serial.Serial("/dev/ttyUSB0",9600,timeout = 10)
        #serialCheck = serialParse()
        serialCheck = ser.read(1)
        if serialCheck == b'\x05':
            capturePic()
            pic = "/home/pi/Desktop/COLOR.jpg"
            img = cv2.imread(pic)
            colorNum = judgeColor(img)
            #print(colorNum)
            ser.write(str.encode(colorNum))
            #print("write over")
            #print(type(colorNum))
            time.sleep(3)
            #serialWrite(colorNum)
        elif serialCheck == b'\x06':
            captureQRcode()
            pic = "/home/pi/Desktop/QRCODE.jpg"
            qrNum = readQRcode(pic)
            #print(qrNum)
            #print(type(qrNum))
            ser.write(str.encode(qrNum))
            display(qrNum)
            #serialWrite(str.encode(qrNum))
            #print("write over")
            time.sleep(3)
main()
