#include <PS4Controller.h>
#include <Wire.h>

bool flag = 0;

/*

ps4Data[9] array:
[0] → 8 buttons  (D-pad + face buttons)
[1] → 8 buttons  (L1,R1,L3,R3,Share,Options,Touchpad,PS)
[2] → L2 value   (0–255, analog)
[3] → R2 value   (0–255, analog)
[4] → LStickX    (-127 to 128)
[5] → LStickY    (-127 to 128)
[6] → RStickX    (-127 to 128)
[7] → RStickY    (-127 to 128)
[8] → omega      (R2–L2 mapped, like a throttle DIFF)

*/

// Byte 0: [Square | Cross | Circle | Triangle | Left | Down | Right | Up]
// Byte 1: [PS | Touchpad | Options | Share | R3 | L3 | R1 | L1]
// Byte 1: [NA | Omega | RY | RX | LY | LX | R2 | L2]

#define size 9
int8_t ps4Data[size] =   {0};
byte sendData[size]  =   {0};       //use pointers

// Byte 0: [Square | Cross | Circle | Triangle | Left | Down | Right | Up]
#define chooseButton_0 B11111111  //choose the button which needs to be used;  0: not to be used; 1: to be used

// Byte 1: [PS | Touchpad | Options | Share | R3 | L3 | R1 | L1]
#define chooseButton_1 B11111111  //choose the button which needs to be used;  0: not to be used; 1: to be used

// Byte 1: [NA | Omega | RY | RX | LY | LX | R2 | L2]
#define chooseButton_2 B01001100  //choose the button which needs to be used;  0: not to be used; 1: to be used

#define buttonMode_0   B11111111  // 0: on press; 1: on click
#define buttonMode_1   B11111111  // 0: on press; 1: on click

#define buttonDown_0   B11111111  // 0: button released; 1: button pressed
#define buttonDown_1   B11111111  // 0: button released; 1: button pressed

#define printData_0    B00000000  // choose which button data to be printed
#define printData_1    B00000000  // choose which button data to be printed
#define printData_2    B00000000  // choose which button data to be printed
int bytesToSend =      0;
int L2, R2, DIFF;
int val;

void psData(){

  //Buttons data
  //up button
  if(buttonMode_0 & 1 && chooseButton_0 & 1){                
    ps4Data[0] |= buttonDown_0 & 1 ? PS4.event.button_down.up : PS4.event.button_up.up;
    if(printData_0 & 1 && ps4Data[0] & 1) Serial.println("Up click");
  }
  else if(chooseButton_0 & 1){
    ps4Data[0] |=  PS4.Up();
    if(printData_0 & 1 && ps4Data[0] & 1) Serial.println("Up press");
  }
  
  //right button
  if(buttonMode_0 & 1 << 1 && chooseButton_0 & 1 << 1){      
    ps4Data[0] |= (buttonDown_0 & 1 << 1 ? PS4.event.button_down.right : PS4.event.button_up.right) << 1;
    if(printData_0 & 1 << 1 && ps4Data[0] & 1 << 1) Serial.println("Right click");
  }
  else if(chooseButton_0 & 1 << 1){
    ps4Data[0] |=  PS4.Right() << 1;
    if(printData_0 & 1 << 1 && ps4Data[0] & 1 << 1) Serial.println("Right press");
  }


  //down button
  if(buttonMode_0 & 1 << 2 && chooseButton_0 & 1 << 2){      
    ps4Data[0] |= (buttonDown_0 & 1 << 2 ? PS4.event.button_down.down : PS4.event.button_up.down) << 2;
    if(printData_0 & 1 << 2 && ps4Data[0] & 1 << 2) Serial.println("Down click");
  }else if(chooseButton_0 & 1 << 2){
    ps4Data[0] |=  PS4.Down() << 2;
    if(printData_0 & 1 << 2 && ps4Data[0] & 1 << 2) Serial.println("Down press");
  }


  //left button
  if(buttonMode_0 & 1 << 3 && chooseButton_0 & 1 << 3){      
    ps4Data[0] |= (buttonDown_0 & 1 << 3 ? PS4.event.button_down.left : PS4.event.button_up.left) << 3;
    if(printData_0 & 1 << 3 && ps4Data[0] & 1 << 3) Serial.println("Left click");
  }
  else if(chooseButton_0 & 1 << 3){
    ps4Data[0] |=  PS4.Left() << 3;
    if(printData_0 & 1 << 3 && ps4Data[0] & 1 << 3) Serial.println("Left press");
  }


  //triangle button   
  if(buttonMode_0 & 1 << 4 && chooseButton_0 & 1 << 4){      
    ps4Data[0] |= (buttonDown_0 & 1 << 4 ? PS4.event.button_down.triangle : PS4.event.button_up.triangle) << 4;
    if(printData_0 & 1 << 4 && ps4Data[0] & 1 << 4) Serial.println("Triangle click");
  }
  else if(chooseButton_0 & 1 << 4){
    ps4Data[0] |=  PS4.Triangle() << 4;
    if(printData_0 & 1 << 4 && ps4Data[0] & 1 << 4) Serial.println("Triangle press");
  }


  //circle button
  if(buttonMode_0 & 1 << 5 && chooseButton_0 & 1 << 5){      
    ps4Data[0] |= (buttonDown_0 & 1 << 5 ? PS4.event.button_down.circle : PS4.event.button_up.circle) << 5;
    if(printData_0 & 1 << 5 && ps4Data[0] & 1 << 5) Serial.println("Circle click");
  }
  else if(chooseButton_0 & 1 << 5){
    ps4Data[0] |=  PS4.Circle() << 5;
    if(printData_0 & 1 << 5 && ps4Data[0] & 1 << 5) Serial.println("Circle press");
  }


  //cross button
  if(buttonMode_0 & 1 << 6 && chooseButton_0 & 1 << 6){      
    ps4Data[0] |= (buttonDown_0 & 1 << 6 ? PS4.event.button_down.cross : PS4.event.button_up.cross) << 6;
    if(printData_0 & 1 << 6 && ps4Data[0] & 1 << 6) Serial.println("Cross click");
  }
  else if(chooseButton_0 & 1 << 6){
    ps4Data[0] |=  PS4.Cross() << 6;
    if(printData_0 & 1 << 6 && ps4Data[0] & 1 << 6) Serial.println("Cross press");
  }


  //square button
  if(buttonMode_0 & 1 << 7 && chooseButton_0 & 1 << 7){        
    ps4Data[0] |= (buttonDown_0 & 1 << 7 ? PS4.event.button_down.square : PS4.event.button_up.square) << 7;
    if(printData_0 & 1 << 7 && ps4Data[0] & 1 << 7) Serial.println("Square click");
  }
  else if(chooseButton_0 & 1 << 7){
    ps4Data[0] |=  PS4.Square() << 7;
    if(printData_0 & 1 << 7 && ps4Data[0] & 1 << 7) Serial.println("Square press");
  }


  //L1 button
  if(buttonMode_1 & 1 && chooseButton_1 & 1){                
    ps4Data[1] |= (buttonDown_1 & 1? PS4.event.button_down.l1 : PS4.event.button_up.l1);
    if(printData_1 & 1 && ps4Data[1] & 1) Serial.println("L1 click");
  }
  else if(chooseButton_1 & 1){
    ps4Data[1] |=  PS4.L1();
    if(printData_1 & 1 && ps4Data[1] & 1) Serial.println("L1 press");
  }


  //R1 button
  if(buttonMode_1 & 1 << 1 && chooseButton_1 & 1 << 1){      
    ps4Data[1] |= (buttonDown_1 & 1 << 1 ? PS4.event.button_down.r1 : PS4.event.button_up.r1) << 1;
    if(printData_1 & 1 << 1 && ps4Data[1] & 1 << 1) Serial.println("R1 click");
  }
  else if(chooseButton_1 & 1 << 1){
    ps4Data[1] |=  PS4.R1() << 1;
    if(printData_1 & 1 << 1 && ps4Data[1] & 1 << 1) Serial.println("R1 press");
  }


  //L3 button
  if(buttonMode_1 & 1 << 2 && chooseButton_1 & 1 << 2){      
    ps4Data[1] |= (buttonDown_1 & 1 << 2 ? PS4.event.button_down.l3 : PS4.event.button_up.l3) << 2;
    if(printData_1 & 1 << 2 && ps4Data[1] & 1 << 2) Serial.println("L3 click");
  }
  else if(chooseButton_1 & 1 << 2){
    ps4Data[1] |=  PS4.L3() << 2;
    if(printData_1 & 1 << 2 && ps4Data[1] & 1 << 2) Serial.println("L3 press");
  }


  //R3 button
  if(buttonMode_1 & 1 << 3 && chooseButton_1 & 1 << 3){      
    ps4Data[1] |= (buttonDown_1 & 1 << 3 ? PS4.event.button_down.r3 : PS4.event.button_up.r3) << 3;
    if(printData_1 & 1 << 3 && ps4Data[1] & 1 << 3) Serial.println("R3 click");
  }
  else if(chooseButton_1 & 1 << 3){
    ps4Data[1] |=  PS4.R3() << 3;
    if(printData_1 & 1 << 3 && ps4Data[1] & 1 << 3) Serial.println("R3 press");
  }


  //share button
  if(buttonMode_1 & 1 << 4 && chooseButton_1 & 1 << 4){      
    ps4Data[1] |= (buttonDown_1 & 1 << 4 ? PS4.event.button_down.share : PS4.event.button_up.share) << 4;
    if(printData_1 & 1 << 4 && ps4Data[1] & 1 << 4) Serial.println("Share click");
  }
  else if(chooseButton_1 & 1 << 4){
    ps4Data[1] |=  PS4.Share() << 4;
    if(printData_1 & 1 << 4 && ps4Data[1] & 1 << 4) Serial.println("Share press");
  }


  //options button
  if(buttonMode_1 & 1 << 5 && chooseButton_1 & 1 << 5){      
    ps4Data[1] |= (buttonDown_1 & 1 << 5 ? PS4.event.button_down.options : PS4.event.button_up.options) << 5;
    if(printData_1 & 1 << 5 && ps4Data[1] & 1 << 5) Serial.println("Option click");
  }
  else if(chooseButton_1 & 1 << 5){
    ps4Data[1] |=  PS4.Options() << 5;
    if(printData_1 & 1 << 5 && ps4Data[1] & 1 << 5) Serial.println("Option press");
  }


  //touchpad button
  if(buttonMode_1 & 1 << 6 && chooseButton_1 & 1 << 6){      
    ps4Data[1] |= (buttonDown_1 & 1 << 6 ? PS4.event.button_down.touchpad : PS4.event.button_up.touchpad) << 6;
    if(printData_1 & 1 << 6 && ps4Data[1] & 1 << 6) Serial.println("Touchpad click");
  }
  else if(chooseButton_1 & 1 << 6){
    ps4Data[1] |=  PS4.Touchpad() << 6;
    if(printData_1 & 1 << 6 && ps4Data[1] & 1 << 6) Serial.println("Touchpad press");
  }


  //ps button
  if(buttonMode_1 & 1 << 7 && chooseButton_1 & 1 << 7){      
    ps4Data[1] |= (buttonDown_1 & 1 << 7 ? PS4.event.button_down.ps : PS4.event.button_up.ps) << 7;
    if(printData_1 & 1 << 7 && ps4Data[1] & 1 << 7) Serial.println("PS click");
  }
  else if(chooseButton_1 & 1 << 7){
    ps4Data[1] |=  PS4.PSButton() << 7;
    if(printData_1 & 1 << 7 && ps4Data[1] & 1 << 7) Serial.println("PS press");
  }
  // if(buttonMode_1 & 1 << 7 && chooseButton_1 & 1 << 7){      //battery option
  //   ps4Data[1] |= (buttonDown_1 & 1 << 7 ? PS4.event.button_down.ps : PS4.event.button_up.ps) << 7;
  //   if(printData_1 & 1 << 7 && ps4Data[1] & 1 << 7) Serial.println("PS click");
  // }
  // else if(chooseButton_1 & 1 << 7){
  //   ps4Data[1] |=  PS4.PSButton() << 7;
  //   if(printData_1 & 1 << 7 && ps4Data[1] & 1 << 7) Serial.println("PS press");
  // }

  // L2-Value
  if(chooseButton_2 & 1){
    ps4Data[2] = PS4.L2Value();
    if(printData_2 & 1) Serial.print(ps4Data[2] + String("  "));
  }

  // R2-Value
  if(chooseButton_2 & 1 << 1){
    ps4Data[3] = PS4.R2Value();
    if(printData_2 & 1 << 1) Serial.print(ps4Data[3] + String("  "));
  }

  // LX-Joystick
  if(chooseButton_2 & 1 << 2){
    val = PS4.LStickX();
    if(abs(val) < 10)
      val = 0;
    ps4Data[4] = val;
    if(printData_2 & 1 << 2) Serial.print(ps4Data[4] + String("  "));
  }

  // LY_Joystick
  if(chooseButton_2 & 1 << 3){
    val = PS4.LStickY();
    if(abs(val) < 10)
      val = 0;
    ps4Data[5] = val;
    if(printData_2 & 1 << 3) Serial.print(ps4Data[5] + String("  "));
  }

  // RX-Joystick
  if(chooseButton_2 & 1 << 4){
    ps4Data[6] = PS4.RStickX();
    if(printData_2 & 1 << 4) Serial.print(ps4Data[6] + String("  "));
  }

  // RY-Joystick
  if(chooseButton_2 & 1 << 5){
    ps4Data[7] = PS4.RStickY();
    if(printData_2 & 1 << 5) Serial.print(ps4Data[7] + String("  "));
  }

  // Omega
  if(chooseButton_2 & 1 << 6){
    R2 = PS4.R2Value();
    L2 = PS4.L2Value();
    DIFF = R2 - L2;
    DIFF = map(DIFF, -255, 255, -127, 128);
    if(abs(DIFF) < 10)
      DIFF = 0;

    // Serial.println((String)r2 + "   " + L2+"    "+DIFF);
    ps4Data[8] = DIFF; 
    if(printData_2 & 1 << 6) Serial.print(ps4Data[8] + String("  "));
  }
  Serial.println();
}




int dataToSend(){
  if(flag){
    int i = 0;

/*
ps4Data[9] array:
[0] → 8 buttons  (D-pad + face buttons)
[1] → 8 buttons  (L1,R1,L3,R3,Share,Options,Touchpad,PS)
[2] → L2 value   (0–255, analog)
[3] → R2 value   (0–255, analog)
[4] → LStickX    (-127 to 128)
[5] → LStickY    (-127 to 128)
[6] → RStickX    (-127 to 128)
[7] → RStickY    (-127 to 128)
[8] → omega      (R2–L2 mapped, like a throttle DIFF)
*/

    // DPad + Face Buttons
    if(chooseButton_0){
      sendData[i] = ps4Data[0];
      i++;
    }

    // Buttons
    if(chooseButton_1){
      sendData[i] = ps4Data[1];
      i++;
    }

    // L2
    if(chooseButton_2 & 1){
      sendData[i] = ps4Data[2];
      i++;
    }

    // R2
    if(chooseButton_2 & 1 << 1){
      sendData[i] = ps4Data[3];
      i++;
    }

    // LStickX
    if(chooseButton_2 & 1 << 2){
      sendData[i] = ps4Data[4];
      i++;
    }

    // LStickY
    if(chooseButton_2 & 1 << 3){
      sendData[i] = ps4Data[5];
      i++;
    }

    // RStickX
    if(chooseButton_2 & 1 << 4){
      sendData[i] = ps4Data[6];
      i++;
    }

    // RStickY
    if(chooseButton_2 & 1 << 5){
      sendData[i] = ps4Data[7];
      i++;
    }

    // Omega
    if(chooseButton_2 & 1 << 6){
      sendData[i] = ps4Data[8];
      i++;
    }
    // Serial.println(i);
    return i;
  }
}

void requestEvent(){
  if(flag == 1){
    Wire.write(sendData, bytesToSend);
    for(int i = 0; i < size; i++){
      ps4Data[i] = 0;
    }
  }
}

void onDisConnect(){
  ps4Data[0] = 0;
  ps4Data[1] = 0;
  ps4Data[2] = 0;
  ps4Data[3] = 0;
  ps4Data[4] = 0;
  ps4Data[5] = 0;
  ps4Data[6] = 0;
  ps4Data[7] = 0;
  ps4Data[8] = 0;
}

void setup() {
  Serial.begin(115200);
  // PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  Serial.println("Reached");
  PS4.begin();
  PS4.attach(psData);
  Serial.println("Startxx");
  while(!PS4.isConnected());
  delay(1000);
  Wire.begin(8);
  Wire.onRequest(requestEvent);

  // delay(1000);
}

void loop() {
  if(PS4.isConnected()){
    flag = 1;
    bytesToSend = dataToSend();
  }
  //   flag = 0;
  // if(PS4.isConnected()){
    // flag = 1;
  // }
}