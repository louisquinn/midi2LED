#include <MIDI.h>

#define PIN 3

int pwm_val;
int i;
int pin_read;

/* Create a MIDI instance */
MIDI_CREATE_DEFAULT_INSTANCE();

/* Callback for a note on */
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    pwm_val = map(velocity,0,127,0,255);
    pin_read = analogRead(PIN);

    /* If LED state is less than velocity val: ramp up */
    if(pin_read < pwm_val)
    {
        for(i = pin_read; i <= pwm_val; i++)
        {
            analogWrite(PIN,i);
        }
    }
    /* If LED state is higher than velocity val: ramp down */
    else if(pin_read > pwm_val)
    {
        for(i = pin_read; i >= pwm_val; i--)
        {
            analogWrite(PIN,i);
        }
    }
    /* If the values are the same, write the pwm_val */
    else if(pin_read == pwm_val)
    {
        analogWrite(PIN,pwm_val);
    }
}

/* Callback for a note off */
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    /* Ramp down to zero to avoid hard transitions */
    for(i = analogRead(PIN); i >= 0; i--)
    {
        analogWrite(PIN,i);
    }
}

/* Callback for pitchbend */
void handlePitchBend(byte channel, int bend)
{
    pwm_val = map(bend,0,8192,0,255);
    analogWrite(PIN,pwm_val);
}

/* ********************************************************** */

/* Setup Arduino */
void setup()
{
    /* Set pin 3 for analog output */
    pinMode(PIN,OUTPUT);

    /* Register callbacks */
    MIDI.setHandleNoteOn(handleNoteOn);  
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(handlePitchBend);

    /* Initiate MIDI and serial communications, listen to channel 1 */
    MIDI.begin(1);//(MIDI_CHANNEL_OMNI);
    Serial.begin(115200);
}

/* Main loop */
void loop()
{
    /* Read the serial port */
    MIDI.read();
}
