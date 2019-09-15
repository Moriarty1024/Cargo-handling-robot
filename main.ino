//参数设定：1是正向走，2是反向走,track1横着走，track2竖着走
//1代表红色，2代表绿色，3代表蓝色
#include <Servo.h>
//#define colorSign 5
//#define QRcodeSign 6
#define mode 1
#define Speed 300
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
const int frontLeftSensor = 22;
const int frontRightSensor = 24;
const int behindLeftSensor = 26;
const int behindRightSensor = 28;
const int leftLeftSensor = 34;
const int leftRightSensor = 36;
const int rightLeftSensor = 30;
const int rightRightSensor = 32;
int flag = 0;//循迹十字交叉点计数
int flag1 = 0;//物块自右向左为1，2，3---第一次抓取
int flag2 = 0;//物块自右向左为1，2，3---第二次抓取
int flag3 = 0;//物块自右向左为1，2，3---第三次抓取
//int comFlag = 1;//通讯标志位
int colorTime = 0;//第colorTime次识别颜色
String colorOrder = "000"; //扫物块得到的颜色序列
String colorNum = "000";//扫二维码得到的颜色序列
void grab()
{
  myservo3.write(0);
  //delay(5000);
  myservo1.write(175);
  delay(500);
  myservo2.write(116);
  //delay(5000);
  myservo4.write(60);
  //delay(5000);
  myservo3.write(40);
  delay(1000);
  myservo4.write(90);
  delay(500);
  myservo2.write(26);
  delay(500);
  myservo3.write(120);
  //delay(5000);
}
void place()
{
  myservo1.write(18);
  delay(1000);
  myservo3.write(20);
  delay(500);
  myservo2.write(145);
  delay(1000);
  myservo4.write(50);
  delay(500);
  myservo2.write(96);
  delay(500);
  myservo1.write(96);
  //delay(5000);
  myservo3.write(96);
  delay(500);
  myservo4.write(90);
  //delay(5000);
}
void stepmotor(int pin1, int pin2, int dtime, int ttl)
{
  digitalWrite(pin1, ttl);
  for (int x = 0; x < 1; x++) // Loop 200 times
  {
    digitalWrite(pin2, HIGH); // Output high
    delayMicroseconds(dtime); // Wait 1/2 a ms
    digitalWrite(pin2, LOW); // Output low
    delayMicroseconds(dtime); // Wait 1/2 a ms
  }
}
void goAlittle(int x, int a, int b, int c, int d, int ttl1, int ttl2, int dtime = Speed)
{
  for (int i = 0; i < x; i++)
  {
    stepmotor(a, b, dtime, ttl1);
    stepmotor(c, d, dtime, ttl2);
  }
}
void goAlittle1(int x, int dir, int dtime = Speed)
{
  for (int i = 0; i < x; i++)
  {
    if (dir == 1)
    {
      int leftSign = digitalRead(frontLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(frontRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 1); //Go straight
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 0);//this is a bug
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 1); //Go straight
      }
    }
    else
    {
      int leftSign = digitalRead(behindLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(behindRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 0); //Go straight
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 0);//this is a bug
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 0);
      }
    }
  }
}
void goAlittle2(int x, int dir, int dtime = Speed)
{
  for (int i = 0; i < x; i++)
  {
    if (dir == 1)
    {
      int leftSign = digitalRead(leftLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(leftRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 1); //Go straight
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 0);//this is a bug
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 1); //Go straight
      }
    }
    else
    {
      int leftSign = digitalRead(rightLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(rightRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 0); //Go straight
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 0);//this is a bug
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 0);
      }
    }
  }
}
void initialize()
{
  while (digitalRead(frontRightSensor) == 0)
  {
    stepmotor(8, 9, Speed, 0);
    stepmotor(11, 12, Speed, 1);
  }
}
void track1(int count, int dir, int dtime = Speed)
{
  while (flag < count)
  {
    if (dir == 1)
    {
      int leftSign = digitalRead(frontLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(frontRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 1); //Go straight
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 0);
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        flag += 1;
        goAlittle1(300, dir);
      }
    }
    else
    {
      int leftSign = digitalRead(behindLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(behindRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 0);
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(2, 3, dtime, 0);
        stepmotor(5, 6, dtime, 0);
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(2, 3, dtime, 1);
        stepmotor(5, 6, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        flag += 1;
        goAlittle1(300, dir);
      }
    }
  }
}
void track2(int count, int dir, int dtime = Speed)
{
  while (flag < count)
  {
    if (dir == 1)
    {
      int leftSign = digitalRead(leftLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(leftRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 1); //Go straight
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 0);
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        flag += 1;
        goAlittle2(400, dir);
      }
    }
    else
    {
      int leftSign = digitalRead(rightLeftSensor);//White area detected,output low level,indicator is lighted
      int rightSign = digitalRead(rightRightSensor);//Blank area detected,output high level,indicator is outed
      if (leftSign == 0 && rightSign == 0)
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 0);
      }
      else if (leftSign == 1 && rightSign == 0)//frontLeftSensor gets the blank sign,deviate to the right,turn left
      {
        stepmotor(8, 9, dtime, 0);
        stepmotor(11, 12, dtime, 0);
      }
      else if (leftSign == 0 && rightSign == 1) //frontRightSensor gets the blank sign,deviate to the left,turn right
      {
        stepmotor(8, 9, dtime, 1);
        stepmotor(11, 12, dtime, 1);
      }
      else if (leftSign == 1 && rightSign == 1)
      {
        flag += 1;
        goAlittle2(400, dir);
      }
    }
  }
}
void parseQRcode()
{
  while (Serial.available() > 0)
  {
    colorNum = Serial.readString();
  }
}
void getSingleColor()
{
  String color;
  while (Serial.available() > 0)
  {
    color = Serial.readString();
    colorOrder[colorTime] = color.charAt(0);//需要验证charAt（）功能
    colorTime++;
  }
}
void redTrack(int i)
{
  flag = 0;
  track2(1, 1);
  while (digitalRead(frontRightSensor) == 0)
    goAlittle2(1, 1);
  if (i == 1)
  {
    flag = 0;
    track1(1, 0);
    while (digitalRead(leftRightSensor) == 0)
      goAlittle1(1, 0);
    flag = 0;
    track2(1, 1);
    goAlittle2(116, 1);
    place();//delay(3000);//###放物块
    flag = 0;
    track2(1, 0);
    while (digitalRead(frontLeftSensor) == 0)
      goAlittle2(1, 0);
    flag = 0;
    track1(1, 1);
    while (digitalRead(leftLeftSensor) == 0)
      goAlittle1(1, 1);
  }
  else
  {
    flag = 0;
    track1(1, 1);
    while (digitalRead(leftLeftSensor) == 0)
      goAlittle1(1, 1);
    flag = 0;
    track2(1, 1);
    goAlittle2(116, 1);
    place();//delay(3000);//###放物块
    flag = 0;
    track2(1, 0);
    while (digitalRead(behindRightSensor) == 0)
      goAlittle2(1, 0);
    flag = 0;
    track1(1, 0);
    while (digitalRead(leftRightSensor) == 0)
      goAlittle1(1, 0);
  }
}
void blueTrack(int i)
{
  flag = 0;
  track2(1, 1);
  while (digitalRead(frontRightSensor) == 0)
    goAlittle2(1, 1);
  if (i == 1)
  {
    flag = 0;
    track1(1, 1);
    while (digitalRead(leftLeftSensor) == 0)
      goAlittle1(1, 1);
    flag = 0;
    track2(1, 1);
    goAlittle2(116, 1);
    place();//delay(3000);//###放物块
    flag = 0;
    track2(1, 0);
    while (digitalRead(behindRightSensor) == 0)
      goAlittle2(1, 0);
    flag = 0;
    track1(1, 0);
    while (digitalRead(leftRightSensor) == 0)
      goAlittle1(1, 0);
  }
  else
  {
    flag = 0;
    track1(1, 0);
    while (digitalRead(leftRightSensor) == 0)
      goAlittle1(1, 0);
    flag = 0;
    track2(1, 1);
    goAlittle2(116, 1);
    place();//delay(3000);//###放物块
    flag = 0;
    track2(1, 0);
    while (digitalRead(frontLeftSensor) == 0)
      goAlittle2(1, 0);
    flag = 0;
    track1(1, 1);
    while (digitalRead(leftLeftSensor) == 0)
      goAlittle1(1, 1);
  }
}
void greenTrack()
{
  flag = 0;
  track2(2, 1);
  goAlittle2(116, 1);
  place();//delay(3000);//###放物料
  flag = 0;
  track2(1, 0);
  while (digitalRead(frontLeftSensor) == 0)
    goAlittle2(1, 0);
}
void addColor()
{
  if ((colorOrder[0] == '1' || colorOrder[0] == '2') && (colorOrder[1] == '1' || colorOrder[1] == '2'))
    colorOrder[2] = '3';
  else if ((colorOrder[0] == '1' || colorOrder[0] == '3') && (colorOrder[1] == '1' || colorOrder[1] == '3'))
    colorOrder[2] = '2';
  else
    colorOrder[2] = '1';
}
void setup()
{
  Serial.begin(9600);
  pinMode(frontLeftSensor, INPUT);
  pinMode(frontRightSensor, INPUT);
  pinMode(leftLeftSensor, INPUT);
  pinMode(leftRightSensor, INPUT);
  pinMode(behindLeftSensor, INPUT);
  pinMode(behindRightSensor, INPUT);
  pinMode(rightLeftSensor, INPUT);
  pinMode(rightRightSensor, INPUT);
  pinMode(2, OUTPUT); // Enable
  pinMode(3, OUTPUT); // Step
  pinMode(4, OUTPUT); // Dir
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(10, LOW);
  digitalWrite(13, LOW);
  myservo1.attach(41);
  myservo2.attach(42);
  myservo3.attach(43);
  myservo4.attach(44);
  initialize();
}
void loop()
{
  flag = 0;
  track1(3, 1);
  goAlittle1(850, 1);
  Serial.write(5);//给串口输入特定字符串树莓派检测到开始识别颜色
  Serial.flush();
  while (colorOrder[0] == '0')
  {
    getSingleColor();
  }
  //Serial.println(colorOrder);
  //delay(3000);
  while (digitalRead(leftLeftSensor) == 0)
  {
    goAlittle1(1, 1);
  }
  delay(3000);
  Serial.write(5);//给串口输入特定字符串树莓派检测到开始识别颜色
  Serial.flush();
  while (colorOrder[1] == '0')
  {
    getSingleColor();
  }
  //Serial.println(colorOrder);
  addColor();
  //Serial.println(colorOrder);
  //delay(3000);
  flag = 0;
  track1(2, 1);
  //goAlittle1(1000, 1);
  while (digitalRead(leftLeftSensor) == 0)
    goAlittle1(1, 1);
  flag = 0;
  track2(1, 1);
  Serial.write(6);//给串口输入特定字符串树莓派检测到开始扫描二维码
  Serial.flush();
  while (colorNum == "000")
  {
    parseQRcode();
  }
  //Serial.println(colorNum);
  //delay(3000);
  flag = 0;
  track2(1, 0);
  while (digitalRead(behindRightSensor) == 0)
    goAlittle2(1, 0);
  if (colorNum[0] == colorOrder[2])
  {
    flag = 0;
    track1(1, 0);
    goAlittle1(866, 0);
    //goAlittle2(600, 1);
    grab();//delay(3000);//###抓物块
    flag1 = 1;
  }
  else if (colorNum[0] == colorOrder[1])
  {
    flag = 0;
    track1(2, 0);
    while (digitalRead(leftRightSensor) == 0)
      goAlittle1(1, 0);
    //goAlittle2(300, 1);
    grab();//delay(3000);//###抓物块
    flag1 = 2;
  }
  else
  {
    flag = 0;
    track1(2, 0);
    goAlittle1(810, 0);
    //goAlittle2(300, 1);
    grab();//delay(3000);//###抓物块
    flag1 = 3;
  }
  switch (flag1)
  {
    case 1: while (digitalRead(leftRightSensor) == 0)
        goAlittle1(1, 0);
      if (colorNum[0] == '1')
        redTrack(mode);
      else if (colorNum[0] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
    case 2: if (colorNum[0] == '1')
        redTrack(mode);
      else if (colorNum[0] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
    default: while (digitalRead(leftLeftSensor) == 0)
        goAlittle1(1, 1);
      if (colorNum[0] == '1')
        redTrack(mode);
      else if (colorNum[0] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
  }
  if (colorNum[1] == colorOrder[2])
  {
    goAlittle2(666, 0);
    while (digitalRead(frontLeftSensor) == 0)
      goAlittle2(1, 0);
    goAlittle1(766, 1);
    //goAlittle(766, 2, 3, 5, 6, 0, 1);
    grab();//delay(3000);//###抓物块
    flag2 = 1;
  }
  else if (colorNum[1] == colorOrder[1])
  {
    goAlittle2(666, 0);
    while (digitalRead(frontLeftSensor) == 0)
      goAlittle2(1, 0);
    grab();//delay(3000);//###抓物块
    flag2 = 2;
  }
  else
  {
    goAlittle2(666, 0);
    while (digitalRead(behindRightSensor) == 0)
      goAlittle2(1, 0);
    goAlittle1(766, 0);
    //goAlittle(766, 2, 3, 5, 6, 1, 0);
    grab();//delay(3000);//###抓物块
    flag2 = 3;
  }
  switch (flag2)
  {
    case 1: while (digitalRead(leftRightSensor) == 0)
        goAlittle1(1, 0);
      if (colorNum[1] == '1')
        redTrack(mode);
      else if (colorNum[1] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
    case 2: if (colorNum[1] == '1')
        redTrack(mode);
      else if (colorNum[1] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
    default: while (digitalRead(leftLeftSensor) == 0)
        goAlittle1(1, 1);
      if (colorNum[1] == '1')
        redTrack(mode);
      else if (colorNum[1] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
  }
  if (colorNum[2] == colorOrder[2])
  {
    goAlittle2(666, 0);
    while (digitalRead(frontLeftSensor) == 0)
      goAlittle2(1, 0);
    goAlittle1(766, 1);
    //goAlittle(766, 2, 3, 5, 6, 0, 1);
    grab();//delay(3000);//###抓物块
    flag3 = 1;
  }
  else if (colorNum[2] == colorOrder[1])
  {
    goAlittle2(666, 0);
    while (digitalRead(frontLeftSensor) == 0)
      goAlittle2(1, 0);
    grab();//delay(3000);//###抓物块
    flag3 = 2;
  }
  else
  {
    goAlittle2(666, 0);
    while (digitalRead(behindRightSensor) == 0)
      goAlittle2(1, 0);
    goAlittle1(766, 0);
    //goAlittle(766, 2, 3, 5, 6, 1, 0);
    grab();//delay(3000);//###抓物块
    flag3 = 3;
  }
  switch (flag3)
  {
    case 1: while (digitalRead(leftRightSensor) == 0)
        goAlittle1(1, 0);
      if (colorNum[2] == '1')
        redTrack(mode);
      else if (colorNum[2] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
    case 2: if (colorNum[2] == '1')
        redTrack(mode);
      else if (colorNum[2] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
    default: while (digitalRead(leftLeftSensor) == 0)
        goAlittle1(1, 1);
      if (colorNum[2] == '1')
        redTrack(mode);
      else if (colorNum[2] == '2')
        greenTrack();
      else
        blueTrack(mode);
      break;
  }
  flag = 0;
  track1(3, 0);
  while (digitalRead(leftRightSensor) == 0)
    goAlittle1(1, 0);
  flag = 0;
  track2(1, 0);
  goAlittle2(835, 0);
  goAlittle(766, 2, 3, 5, 6, 1, 0);
  while (1)
  {
  }
}
