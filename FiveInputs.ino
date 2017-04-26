

/*Five Inputs Requires players to activate 5 inputs to win

2016 Shawn Yates
ClueControl
Want to automate this puzzle ? Visit our website :
www.cluecontrol.com

written and tested with vMicro for VisualStudio

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

written and tested with Arduino 1.6.9


------------------ -
*/

/*
// pin assignments
* Signal     Pin
*            Arduino Uno
* ------------------------
* 0   Serial RX
* 1   Serial TX
* 2   Game switches -1		Input goes high when input is solved, input is low when not solved
* 3   Game switches -2
* 4   Game switches -3
* 5   Game switches -4
* 6   Game switches -5
* 7   Output -1				can be set to active high or active low below

* 8   Output -2
* 9   Output -3
* 10  Output -4
* 11  Output -5
* 12  Master Output
* 13  Disable input			short to ground to disable or reset the game

* 14  (A0)
* 15  (A1)
* 16  (A2)
* 17  (A3)
* 18  (A4/SDA)
* 19  (A5/SCK)




GAME PLAY

If the DISABLE input is connected to ground, the game will not be active.
When the DISABLE input is not connected to anything, the game will be active

Monitor game switches.  For each switch that is activated, the corrosponding
output will be turned on.  When all 5 inputs are activated, the master
output will be turned on.


*/

//*** Settigns here can be changed to configure the game

	const bool	OUTPUT_ON = HIGH;			//change this to use active high or active low outputs



//***********************************************************************************************
//**  things below this line should not be changed unless you are pretty sure of what you are  **
//**  doing.  But, feel free to read, experiment and learn.                                    **
//***********************************************************************************************


	const	byte	DEBOUNCE	= 50;								//how many mS to delay and resample input

	const	byte	INPINS[]	= { 2, 3, 4, 5, 6 };				//array of input pins to monitor
	const	byte	OUTPINS[]	= { 7, 8, 9, 10, 11 };				//array of output pins to control

	const	byte	MASTEROUT	= 12;								//Master output pin
	const	byte	DISABLE		= 13;

	const	bool	OUTPUT_OFF = !OUTPUT_ON;						//Off is the opposite of on.  Who would have thunk it?

	bool	Winning = false;										//a flag to know if players won or not

void setup() {

	for (byte x = 0; x < 5; x++)
	{
		pinMode(INPINS[x], INPUT);									// setup all the intput pins
		pinMode(OUTPINS[x], OUTPUT);								// and output pins
		digitalWrite(OUTPINS[x], OUTPUT_OFF);						// and make sure the output pins are turned off
	}

	pinMode(MASTEROUT, OUTPUT);										// setup master output
	digitalWrite(MASTEROUT, OUTPUT_OFF);							// and make sure its turned off

	pinMode(DISABLE, INPUT_PULLUP);									// make the disable have a pull up so it can be left open

	Serial.begin(9600);												//serial interface for debugging and monitoring
	Serial.println("Serial Interface started");

}

void loop()
{

	if (DebounceSW(DISABLE))										//if disable is pulled low,
	{
		for (byte x = 0; x < 5; x++)								//make sure all the outputs are turned off
		{
			digitalWrite(OUTPINS[x], OUTPUT_OFF);						
		}
		digitalWrite(MASTEROUT, OUTPUT_OFF);						// and the master is turned off
		Winning = false;											// and clear the winning flag
		return;														// and return to loop - meaning nothign below here runs
																	// when disable is active
	}

	if (Winning) return;

	//** below only runs if DISABLE is not active  AND if players have not already won the game

	Winning = true;													//assume the player will win

	for (byte x = 0; x < 5; x++)									// loop through all 5 inputs
	{
		if (!DebounceSW(INPINS[x]))									// the DEBOUNCE routine assumes the input is active low so
		{															// invert the reading here DEBOUNCE will be FALSE when the input is active (high)
			digitalWrite(OUTPINS[x], OUTPUT_ON);					// The input is active - turn on the output

		}
		else														// else teh input is NOT active, so
		{
			Winning = false;										// change winning to false
			digitalWrite(OUTPINS[x], OUTPUT_OFF);					// and turn off the output
		}
	}  //end if FOR loop

	if (Winning)													// if winning is true
	{
		digitalWrite(MASTEROUT, OUTPUT_ON);							// turn on the master output
	}
	
}

bool DebounceSW(byte SWx)
{
	//Read the switch 5 times, 10mS apart.  Return 
	//the value of the majority of the reads.
	//this is to prevent false or erattic triggering
	//caused by the internal mechanical bounce of a switch

	byte HighCounts = 0;

	for (int x = 0; x <5; x++)
	{
		if (!digitalRead(SWx))    //invert the reading due to the use of pullups
		{
			HighCounts++;
		}

		delay(10);

	}
	return (HighCounts > 2);    //if there are more than 2 hights, return high
}




