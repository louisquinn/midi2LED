byte command_byte;
byte second_byte;
byte third_byte;

byte noteOn = 144;
byte noteOff = 128;
byte controller = 176;
byte pitchBend = 224;
int pwm_val;

int temp;
int temp2;

int status;

void setup()
{
  Serial.begin(115200);
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);

  digitalWrite(13,LOW);
}

/* Main loop here */
void loop()
{
  status = midi_read();
  delay(50);
}

/********** Functions **********/
int midi_read()
{
    do
    {
        command_byte = Serial.read();   /* Command byte */
        second_byte = Serial.read();    /* Note */
        third_byte = Serial.read();     /* Velocity */
    
        if(command_byte == noteOn && third_byte > 0)
        {
            pwm_val = map(third_byte,0,127,0,255);
            analogWrite(3,pwm_val);
        }
        if(command_byte == noteOff || (command_byte == noteOn && third_byte == 0))
        {
            analogWrite(3,0);
        }
            
        if(command_byte == pitchBend)
        {
            pwm_val = map(second_byte,0,16382,0,255);
            analogWrite(3,pwm_val);
        }

        if(command_byte == controller)
        {
            /* Increases light level to almost half then fades out */
            if(second_byte == 14)
            {
                temp = analogRead(3);
                temp2 = map(temp,0,1023,0,255);
                if(temp < 150)
                {
                    for(int i = temp; i < 150; i++)
                    {
                        analogWrite(3,i);
                        delay(300);
                    }
                }
                for(int i = 150; i > 0; i-=5)
                {
                    analogWrite(3,i);
                    delay(150);
                }
                analogWrite(3,0);
                serial_flush();
            }
        }
    }
    while(Serial.available() > 2);
}

void serial_flush()
{
  if(Serial.available() > 0)
  {
    char t = Serial.read();
  }
}
