#include "Wire.h"

//pin selection
int red=11;
int black=10;
int reset=12;

//paths
int path[][28] = { {29,20,19,30,33,39,38,42,43,44,36,35,27,22,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                   {29,30,31,18,19,14,5,7,4,8,9,12,2,13,21,28,27,26,23,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                   {29,32,33,24,25,26,27,28,21,13,2,12,3,0,4,7,5,14,19,18,17,8,9,16,23,-1,-1,-1},
                   {29,30,33,39,38,24,31,18,15,14,5,7,4,8,9,12,2,13,21,28,34,35,36,26,23,-1,-1,-1}
                 };
int path_count = 4;

//global variables
int selected_path;
int step = 0;
int current_move = 0;
byte inputs = 0;

void setup() {
  selectPath();

  pinMode(red,OUTPUT); 
  pinMode(black,OUTPUT);
  pinMode(reset,INPUT);
  
  Serial.begin(9600);
  Wire.begin(); // wake up I2C bus
}

void loop() {
  if (digitalRead(reset) == HIGH) {
    selectPath();
    step = 0;
    current_move = 0;
  }
  getCurrentMove();
  //Serial.println(current_move);
  if (current_move == path[selected_path][step])
  {
    flipBlack();
    step = step+1;
    Serial.println("+1");
  }
  else if (((step >= 1) && (current_move == path[selected_path][step-1])) || ((step >= 2) && (current_move == path[selected_path][step-2])))
  {
    //Serial.println("stalling");
    if (path[selected_path][step] == -1)
    {
      flicker();
      Serial.println("fini");
    }
  }
  else
  {
    flipRed();
    step = 0;
    Serial.println("back to 0");
  }
  delay(50);
}

void flipRed() {
  digitalWrite(red,HIGH);
  digitalWrite(black,LOW);
  delay(100);
  digitalWrite(red,LOW);
}

void flipBlack() {
  digitalWrite(black,HIGH);
  digitalWrite(red,LOW);
  delay(100);
  digitalWrite(black,LOW);
}

void flicker() {
   for (int i=0; i <= 5; i++){
      flipBlack();
      flipRed();
   } 
}

void selectPath() {
  selected_path = random(path_count);
}

void getCurrentMove() {
  inputs = 0;
  Wire.beginTransmission(0x20); // 000 pin MCP
  Wire.write(0x13); // B side
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);
  inputs=Wire.read();
  if (inputs>0)
  {
    for (int i = 0; i < 8; i++)
    {
      if (bitRead(inputs, i) == 1)
      {
        current_move = i;
      }
    }
  }
  else
  {
    Wire.beginTransmission(0x20); // 000 pin MCP
    Wire.write(0x12); // A side
    Wire.endTransmission();
    Wire.requestFrom(0x20, 1);
    inputs=Wire.read();
    if (inputs>0)
    {
      for (int i = 0; i < 8; i++)
      {
        if (bitRead(inputs, i) == 1)
        {
          current_move = i+8;
        }
      }
    }
    else
    {
      Wire.beginTransmission(0x21); // 001 pin MCP
      Wire.write(0x13); // B side
      Wire.endTransmission();
      Wire.requestFrom(0x21, 1);
      inputs=Wire.read();
      if (inputs>0)
      {
        for (int i = 0; i < 8; i++)
        {
          if (bitRead(inputs, i) == 1)
          {
            current_move = i+16;
          }
        }
      }
      else
      {
        Wire.beginTransmission(0x21); // 001 pin MCP
        Wire.write(0x12); // A side
        Wire.endTransmission();
        Wire.requestFrom(0x21, 1);
        inputs=Wire.read();
        if (inputs>0)
        {
          for (int i = 0; i < 8; i++)
          {
            if (bitRead(inputs, i) == 1)
            {
              current_move = i+24;
            }
          }
        }
        else
        {
          Wire.beginTransmission(0x22); // 010 pin MCP
          Wire.write(0x13); // B side
          Wire.endTransmission();
          Wire.requestFrom(0x22, 1);
          inputs=Wire.read();
          if (inputs>0)
          {
            for (int i = 0; i < 8; i++)
            {
              if (bitRead(inputs, i) == 1)
              {
                current_move = i+32;
              }
            }
          }
          else
          {
            Wire.beginTransmission(0x22); // 010 pin MCP
            Wire.write(0x12); // A side
            Wire.endTransmission();
            Wire.requestFrom(0x22, 1);
            inputs=Wire.read();
            if (inputs>0)
            {
              for (int i = 0; i < 8; i++)
              {
                if (bitRead(inputs, i) == 1)
                {
                  current_move = i+40;
                }
              }
            }
          }
        }
      }
    }
  }
}
