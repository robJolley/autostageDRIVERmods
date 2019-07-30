//autostage.cpp//   
#include <SPI.h>
#include "motiontmc2130stepper.h"
//#include "intepretCommand.h"
#include "globalAutostageVars.h"
#include <EEPROM.h>
serialReturn returnedData; 	
int responceToSend = 0;
int charCount = 0;
boolean LS_Moving = false;
boolean TT_Moving = false;

angularTMC2130Stepper TMC2130_TT = angularTMC2130Stepper(EN_PIN_TT, DIR_PIN_TT, STEP_PIN_TT, CS_PIN_TT, TTLS);
linearTMC2130Stepper TMC2130_LS = linearTMC2130Stepper(EN_PIN_LS, DIR_PIN_LS, STEP_PIN_LS, CS_PIN_LS, LST1, LST2);

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
	Serial.begin(115200);// Change to 115200??
	Serial.setTimeout(50);// Set the minimum timeout to 50ms to slow hangups,,  may be ideal to decrease this further.....
	pinMode(MTR_DRV, OUTPUT);  
	DDRC = DDRC & B11110011;// Mask to set pin A2 and A3 to input,  ignoring others used.
	PORTC = PORTC & B11110011;
	PORTC = PORTC | B00001100;
	inputString.reserve(16);
	TMC2130_TT.initilise();//Initilise with default settings
	TMC2130_LS.initilise();//Initilise with default settings
	indicatorLight(ORANGE);
	delay(50);
	digitalWrite(EN_PIN_TT, LOW);
    digitalWrite(EN_PIN_LS, LOW);
	TMC2130_LS.load();//Sets position to move to load to
	//	Serial.println("Out of home");
	//	TMC2130_TT.load();
//	delayMicroseconds(50);
	TMC2130_TT.currentPos = 0.0;
	TMC2130_LS.currentPos = 0.0;
	indicatorLight(GREEN);
	linearPosition = 0;
	angularPosition = 0;
  returnedData.posnumber = 0;
  returnedData.numberpos = SAMPLEARRAY;//(sizeof(sampleArray) / sizeof(sampleArray[0]));
  newPosition = 100;
}

void loop()
{	
	LS_Moving = TMC2130_LS.run();
  TT_Moving = TMC2130_TT.run();
  inmove =(LS_Moving || TT_Moving);
	
	
	if ((inmove == true) && (inmoveOld == false))//Each return 1 if moving, also will move single step/microstep if steps available to take to desired position
	{
		indicatorLight(ORANGE);
    TMC2130_LS.rms_current(1400);
    TMC2130_TT.rms_current(600);    
	}
	if((inmove == false) && (inmoveOld == true))
	{
		responceToSend = MOVECOMPLETE;
    returnedData.posnumber = newPosition;//Previous move complete.  Waits for either next or prev
		indicatorLight(GREEN);
    TMC2130_LS.rms_current(400);
    TMC2130_TT.rms_current(200);
	}
	
	if (stringComplete) 
	{
		returnedData = interpretCommand(inputString);
		if((returnedData.move == true) && (returnedData.responce == GOTOACK) && (inmove == false))//Valid command to move
		{
			TMC2130_LS.moveAbsolute(returnedData.lin);
			TMC2130_TT.moveAbsolute(returnedData.ang);
			responceToSend = GOTOACK;
			
		}
		else if((returnedData.move == true) && (returnedData.responce == SEQUENCEACK) && (inmove == false))//Valid command to move
		{
			newPosition = 0;
			TMC2130_LS.moveAbsolute(findPosLS());
			TMC2130_TT.moveAbsolute(findPosTT());
			responceToSend = SEQUENCEACK;
			
		}

   
    else if((returnedData.move == true) && (returnedData.responce == NEXTACK) && (inmove == false))//Valid command to move
    { 
      if((newPosition == 100) || (newPosition == 200))
        {
          newPosition = 0;
          TMC2130_LS.moveAbsolute(findPosLS());
          TMC2130_TT.moveAbsolute(findPosTT());
        
          responceToSend = NEXTACK;
          }
      else if(newPosition >= returnedData.numberpos ||newPosition < 0 )
        responceToSend = COMMANDERROR;
      else
        {
          newPosition++;   
          TMC2130_LS.moveAbsolute(findPosLS());
          TMC2130_TT.moveAbsolute(findPosTT());         
          responceToSend = NEXTACK;
        }
      
    }
    else if((returnedData.move == true) && (returnedData.responce == GOPOSACK) && (inmove == false))//Valid command to move
    {
      if(newPosition >= returnedData.numberpos)
        responceToSend = COMMANDERROR;
      else
        {
          newPosition = returnedData.posnumber;
          TMC2130_LS.moveAbsolute(findPosLS());
          TMC2130_TT.moveAbsolute(findPosTT());          
          responceToSend = GOPOSACK;
        }
      
    }
    else if((returnedData.move == true) && (returnedData.responce == STRPOSACK) && (inmove == false))//Valid command to move
    {

          newPosition = 0;
          TMC2130_LS.moveAbsolute(findPosLS());
          TMC2130_TT.moveAbsolute(findPosTT());          
          responceToSend = STRPOSACK;
      
    }
    
    else if((returnedData.move == true) && (returnedData.responce == PREVACK) && (inmove == false))//Valid command to move
    { 

      if((newPosition < 1) || (newPosition == 100) || (newPosition == 200))
        responceToSend = COMMANDERROR;
      else
        {
        newPosition--;  
        TMC2130_LS.moveAbsolute(findPosLS());
        TMC2130_TT.moveAbsolute(findPosTT());
        responceToSend = PREVACK;
        }
    
    }
   
		else if ((returnedData.move == true) && (returnedData.responce == STRPOSACK ||returnedData.responce == GOTOACK || returnedData.responce == GOPOSACK|| returnedData.responce == CENTREACK|| returnedData.responce == LOADACK || returnedData.responce == SEQUENCEACK || returnedData.responce == NEXTACK || returnedData.responce == PREVACK) && (inmove == true))//Cannot move as aleady moving
		{
			if (returnedData.responce == GOTOACK)
			  responceToSend = GOTOBSY;
       if (returnedData.responce == STRPOSACK)
        responceToSend = STRPOSBSY;
       
      else if (returnedData.responce == SEQUENCEACK)
        responceToSend = SEQBSY;
      else if (returnedData.responce == NEXTACK)
        responceToSend = NEXTBSY;
      else if (returnedData.responce == PREVACK)
        responceToSend = PREVBSY;
      else if (returnedData.responce == CENTREACK)
        responceToSend = CENTREBSY;    
      else if (returnedData.responce == LOADACK)
        responceToSend = LOADBSY;
      else if (returnedData.responce == GOPOSACK)
        responceToSend = GOPOSBSY;
      else
        responceToSend = GOTOBSY;
		}
   
		else if((returnedData.vib >=0) && (returnedData.responce == VIBACK))
		{
			vibEngine(returnedData.vib);
			responceToSend = VIBACK;
		}
       else if((returnedData.responce == VIBERR))
    {
      responceToSend = VIBERR;
    }
   
		else if((returnedData.responce == LOADACK))
		{
			TMC2130_LS.load();//Sets position to move to home to
			TMC2130_TT.load();
      newPosition = 100;
//      Serial.println("Got in here load");
			responceToSend = LOADACK;
		}
    else if(returnedData.responce == LOADTHERE)
    {
      responceToSend = LOADTHERE;
    }
    else if(returnedData.responce == CENTRETHERE)
    {
      responceToSend = CENTRETHERE;
    }
    else if(returnedData.responce == STARTTHERE)
    {
      responceToSend = STARTTHERE;
    }
    else if(returnedData.responce == GOPOSTHERE)
    {
      responceToSend = GOPOSTHERE;
    }
    else if((returnedData.responce == CENTREACK))
    {
      TMC2130_LS.centre();//Sets position to move to home to
      TMC2130_TT.centre();
      responceToSend = CENTREACK;
      newPosition = 200;
    }
    else if((returnedData.responce == POSACK))
    {
      responceToSend = POSACK;
    }
        else if((returnedData.responce == HB))
    {
      responceToSend = HB;
    }
    
     else if(returnedData.responce == COMMANDERROR)
    {
      responceToSend = COMMANDERROR;
    }
    else if(returnedData.responce == NORESPONCE)
    {
      responceToSend = NORESPONCE;
    }
    else if(returnedData.responce == READYACK)
    {
      responceToSend = READYACK;
    }
    else if(returnedData.responce == SNACK)
    {
      responceToSend = SNACK;
    }
    else if(returnedData.responce == HELP)
    {
      responceToSend = HELP;
    }
    else if(returnedData.responce == INFO)
    {
      responceToSend = INFO;
    }
    else if(returnedData.responce == SEQNUM)
    {
      responceToSend = SEQNUMACK;
    }
    else if(returnedData.responce == SEQTHERE)
    {
      responceToSend = SEQTHERE;  
    }
		else
		{
			responceToSend = COMMANDERROR;
		}
		
		inputString = "";
		returnedData.move = false;
		stringComplete = false;
	}
	
	if(responceToSend > 0)
	{
		serialResponder(responceToSend);//Sets what to send back to Eyepass.
		responceToSend = 0;
	}
	inmoveOld = inmove;
	
}

void serialEvent() {
	//	if(!inmove)// to accomidate timimg requirments of steppers.
	//	{
	while (Serial.available()) {
		// get the new byte:
		char inChar = (char)Serial.read();
		//	Serial.println("Here");
		
		// add it to the inputString:
		inputString+=inChar;
		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if ((inChar == '}') or (inputString =="help") or (inputString == "Help")) 
		{
			stringComplete = true;
      Serial.flush();// In order to fluxh out serial port in the event 
		}
		else
		{
			stringComplete = false;
		}
		
		
		if((!(inputString.startsWith("{")) && (!(inputString.startsWith("h")))))
		{
			inputString = "";
		}
		
	}
}

byte indicatorLight(byte colour)
{
	if(colour == GREEN)//Colours reversed  change back to GREEN
	{
		digitalWrite(GREEN_LED, HIGH);
		digitalWrite(RED_LED, LOW);
	}
	else if(colour == RED)//Colours Reversed change back to RED
	{
		digitalWrite(GREEN_LED, HIGH);
		digitalWrite(RED_LED, LOW);
	}
	
	else if(colour == ORANGE)
	{
		digitalWrite(GREEN_LED, HIGH);
		digitalWrite(RED_LED, HIGH);
	}
	else
	{
		digitalWrite(GREEN_LED, LOW);
		digitalWrite(RED_LED, LOW);
	}
	return colour;
	
}

byte vibEngine(byte vibMag)
{
	analogWrite(MTR_DRV, vibMag);
	return vibMag;
}

int findPosLS()
	{
		int required_pos = sampleArray[newPosition][0];
    returnedData.lin = required_pos;
//    Serial.println(required_pos);
		return required_pos;
	}
		
int findPosTT()
	{
		int required_pos = sampleArray[newPosition][1];
    returnedData.ang = required_pos;
//    Serial.println(required_pos);
		return required_pos;
	}
