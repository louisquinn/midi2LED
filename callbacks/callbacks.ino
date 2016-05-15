#include <MIDI.h>

int pwm_val;

/* Create a MIDI instance */
MIDI_CREATE_DEFAULT_INSTANCE();

/* Callback for a note on */
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    pwm_val = map(velocity,0,127,0,255);
    analogWrite(3,pwm_val);
}

/* Callback for a note off */
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    analogWrite(3,0);
}

/* Callback for pitchbend */
void handlePitchBend(byte channel, int bend)
{
    pwm_val = map(bend,0,8191,0,255);
    analogWrite(3,pwm_val);
}

/* ********************************************************** */

/* Setup Arduino */
void setup()
{
    /* Set pin 3 for analog output */
    pinMode(3,OUTPUT);

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
