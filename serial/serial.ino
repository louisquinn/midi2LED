byte commandByte;
byte secondByte;
byte thirdByte;

byte noteOn = 144;
byte noteOff = 128;
byte controller = 176;
byte pitchBend = 224;
int pwmVal;

void setup()
{
  Serial.begin(115200);
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);

  digitalWrite(13,LOW);
}

int checkMIDI()
{
  do
  {
    commandByte = Serial.read();//read command byte
    secondByte = Serial.read();//read next byte (MSB, or Note)
    thirdByte = Serial.read();//read final byte (LSB or velocity)

    if (commandByte == noteOn)
    {
      if (secondByte > 0 && thirdByte > 0)
      {
        pwmVal = map(thirdByte,0,127,0,255);
        analogWrite(3,pwmVal);//turn on led with velocity value
        //delay(10);
      }
    }
    
    else if(commandByte == noteOff)
    {
      analogWrite(3,0);
    }
    
    if(commandByte == pitchBend)
    {
      /*
      //analogWrite(3,50);
      pwmVal = map(secondByte,0,16382,0,255);
      analogWrite(3,pwmVal); //pitchbend value 
      */

      for (int i = 20; i < 200; i++)
      {
        analogWrite(3,i);
        delay(5);
      }
      analogWrite(3,200);
      for (int j = 200; j > 20; j--)
      {
        //if(j != 21)
        //{
          analogWrite(3,j);
          delay(5);
        //}
        //else
        //{
        //  analogWrite(3,j);
        //}

      }
      //return 1;
      serialFlush();
    }

    if(commandByte == controller)
    {
      if(secondByte == 14)
      {
        int temp = analogRead(3);
        int temp2 = map(temp,0,1023,0,255);
        if(temp < 150)
        {
          for (int j = temp; j < 150; j++)
          {
            analogWrite(3,j);
            delay(300);
          }
        }
        for (int i = 150; i > 0; i-=5)
        {
          analogWrite(3,i);
          delay(150);
        }
        analogWrite(3,0);
        serialFlush();
      }
    }
  }
  while (Serial.available());//when at least three bytes available
}

void loop()
{
  int status = checkMIDI();
  /*if(status == 1)
  {
    serialFlush();
    digitalWrite(13,HIGH);
    checkMIDI();
  }
  
  else
  {
    digitalWrite(13,LOW);
    checkMIDI();
  }
  */
  delay(50);
  //Serial.println(thirdByte);
  //digitalWrite(13,LOW);
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   

