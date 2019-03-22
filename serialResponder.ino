// SerialResponder.ino
#include "globalAutostageVars.h"

int serialResponder(int responce)
{
	switch(responce)
	{
		case HB:
		{  //Done
			Serial.write("<HB>");
			Serial.write('\n');
      Serial.write("<HB [RSP]>");
      Serial.write('\n');
			break;
		}	
		case GOTOACK:
		{
			Serial.write("<btgoto [ACK]>");
			Serial.write('\n');
			break;
		}

   
		case GOTOSWERR:
		{
			Serial.write("<btgoto [ERR]>");//Error condtion from btgoto
			Serial.write('\n');
			break;
		}
		case READYACK: //Done
		{
			if(inmove == false)
			{
			  Serial.write("<btredy [ACK]>");//Device ready after setup
			  Serial.write('\n');
			}
     else
     {
        Serial.write("<btredy [BSY]>");//Device not after setup
        Serial.write('\n');
     }
        Serial.write("<btredy [RSP]>");
        Serial.write('\n');         
			break;
		}
		case READYSWERR:
		{
			Serial.write("<btredy [ERR]>");//btready error,  startup sequence not complete properly
			Serial.write('\n');
			break;
		}
		case SEQUENCEACK:
		{
			Serial.write("<btsequence [ACK]>");//Acknoledge sequence recived
			Serial.write('\n');
      break;
		}

    case SEQTHERE:
    {
      Serial.write("<btsequence [ACK]>");//Acknoledge sequence recived
      Serial.write('\n');
      Serial.write("<btmovecomplete [RSP]>");//Acknoledge sequence recived
      Serial.write('\n');
      break;
    }
    case LOADTHERE:
    {
      Serial.write("<btload [ACK]>");//Acknoledge sequence recived
      Serial.write('\n');
      Serial.write("<btmovecomplete [RSP]>");//Acknoledge sequence recived
      Serial.write('\n');
      break;
    }    
    case CENTRETHERE:
    {
      Serial.write("<btcentre [ACK]>");//Acknoledge sequence recived
      Serial.write('\n');
      Serial.write("<btmovecomplete [RSP]>");//Acknoledge sequence recived
      Serial.write('\n');
      break;
    }
    case NEXTACK:
    {
      Serial.write("<btnext [ACK]>");//next in sequence recived
      Serial.write('\n');
      break;
    }
    
    case PREVACK:
    {
      Serial.write("<btprev [ACK}>");//previous in sequence recived
      Serial.write('\n');
      break;
    }
     case GOPOSACK:
    {
      Serial.write("<gopos [ACK]>");//previous in sequence recived
      Serial.write('\n');
      break;
    }

     
		case MOVECOMPLETE:
		{
//			if ((returnedData.posnumber == returnedData.numberpos) && (returnedData.numberpos >0) && (goFlag ==true))
//			{
//				Serial.write("<btseqcomplete>");//Sequence complete 
//			}
//			else if ((returnedData.posnumber < returnedData.numberpos) && (returnedData.numberpos >0)&& (goFlag == true))
//			{
//				Serial.write("<btseq ");
//				if(returnedData.posnumber <10)//Shows <001/039> where 001 is the point in the sequence completed and 039 being the total number of sequences ie 0-39 (40 steps)
//					Serial.write("00");
//				else
//					Serial.write("0");
//				Serial.print(returnedData.posnumber);
//				Serial.write("/");
//				if(returnedData.numberpos <10)
//				  Serial.write("00");
//				else
//			    Serial.write("0");
//				Serial.print(returnedData.numberpos);
//				Serial.write(">");
//			}
//     else
//     {
      Serial.write("<btmovecomplete [RSP]>");
//      }
      goFlag = false;
			Serial.write('\n');
      break;
		}
		case LOADACK:
		{
			Serial.write("<btload of [ACK]>");//Acknologment for load sequence
			Serial.write('\n');
			break;
		}
    case CENTREACK:
    {
      Serial.write("<btcentre [ACK]>");//Acknologment for centre sequence
      Serial.write('\n');
      break;
    }  
//		case EXTENDACK:
//		{
//			Serial.write("<btextd [ACK]>");//Unused,  extend,  load replaced
//			Serial.write('\n');
//			break;
//		}
//		case EXTENDSWERR://Error from extend
//		{
//			Serial.write("<btextd swerr>");
//			Serial.write('\n');
//			break;
//		}
		case NORESPONCE_CLEAR:
		{
			Serial.write("<btnotcl>");//ie not clear...
			Serial.write('\n');
			break;
		}
		case SNACK ://Serial number written to EEPROM
		{
			Serial.write("<btwtsn [ACK]>");
			Serial.write('\n');
      Serial.write("<btwtsn [RSP]>");
      Serial.write('\n');
			break;
		}
		case HELP:
		{
			helpPrint();
			break;
		}
		case GOTOBSY://Stepper motors busy cannot move yet
		{
			Serial.write("<btgoto [ACK]>");
      Serial.write('\n');
			Serial.write("<btgoto [BSY]>");
			Serial.write('\n');
			break;
		}
    case SEQBSY://Stepper motors busy cannot move yet
    {
      Serial.write("<btsequence [ACK]>");
      Serial.write('\n');
      Serial.write("<btsequence [BSY]>");
      Serial.write('\n');
      break;
    }
    case NEXTBSY://Stepper motors busy cannot move yet
    {
      Serial.write("<btnext [ACK]>");
      Serial.write('\n');
      Serial.write("<btnext [BSY]>");
      Serial.write('\n');
      break;
    }
     case PREVBSY://Stepper motors busy cannot move yet
    {
      Serial.write("<btprev [ACK]>");
      Serial.write('\n');
      Serial.write("<btprev [BSY]>");
      Serial.write('\n');
      break;
    }
    case CENTREBSY://Stepper motors busy cannot move yet
    {
      Serial.write("<btcentre [ACK]>");
      Serial.write('\n');
      Serial.write("<btcentre [BSY]>");
      Serial.write('\n');
      break;
    }
    case LOADBSY://Stepper motors busy cannot move yet
    {
      Serial.write("<btload [ACK]>");
      Serial.write('\n');
      Serial.write("<btload [BSY]>");
      Serial.write('\n');
      break;
    }
    case GOPOSBSY://Stepper motors busy cannot move yet
    {
      Serial.write("<btgopos [ACK]>");
      Serial.write('\n');
      Serial.write("<btgopos [BSY]>");
      Serial.write('\n');
      break;
    }
    case SEQNUMACK://Stepper motors busy cannot move yet
    {
      Serial.write("<btseqnum [ACK]>");
      Serial.write('\n');
      Serial.write("<027 [RSP]>");  //Change if required ####
      Serial.write('\n');   
      break;
    }
    
		case INFO:
		{
			printInfo();
			break;
		}
		case COMMANDERROR://Error in command
		{
			Serial.write("<btcomd [ERR}>");
			Serial.write('\n');
			break;
		}
		case VIBACK:
		{
			Serial.write("<btvib [ACK]>");//vibrate ack
			Serial.write('\n');
      Serial.write("<btvibactivated [RSP]>");//vibrate ack
      Serial.write('\n');
			break;
		}
    case VIBERR:
    {
      Serial.write("<btvib [ERR]>");
      Serial.write('\n');
      break;
     }
   case POSACK://request for position  in formt <btpos lin:000 ang:000>
   {
      Serial.write("<btpos [ACK]>");
      Serial.write('\n');   
;
      Serial.write("<btpos ");
      if(newPosition <10)//Shows <001/039> where 001 is the point in the sequence completed and 039 being the total number of sequences ie 0-39 (40 steps)
        Serial.write("00");
      else
         Serial.write("0");
      Serial.print(newPosition);
      Serial.write(" [RSP]>");
//      if(returnedData.numberpos <10)
//        Serial.write("00");
//      else
//        Serial.write("0");
//      Serial.print(returnedData.numberpos);
      Serial.write(">");
      Serial.write('\n');
      break;
   }
		
		default :
		{
			Serial.write("<?>");// Esentually unknown command
			Serial.write('\n');
			break;
		}
		
	}
}


void helpPrint()
{
	Serial.println(F("Command list"));// Define list of comands to be printed.......
  Serial.println(F("{HB},             Heartbeat."));
  Serial.println(F("{btinfo},         Returns device info."));
  Serial.println(F("{btgoto LLLAAA},  Command to move"));
  Serial.println(F("{btsequence},     Start sequence."));
  Serial.println(F("{btnext},         Move Next."));
  Serial.println(F("{btprev},         Move Previous."));
  Serial.println(F("{btpos},          Show current pos"));
  Serial.println(F("{btwtsn ****},    Write sn"));
  Serial.println(F("{btvib ***},      Set vib level"));
  Serial.println(F("{btredy},        Ready?"));
  Serial.print(F("help,             Help sheet"));
	Serial.write('\n');
}

void printInfo()
{  //Done
  Serial.print(F("<btinfo [ACK]"));
  Serial.write('\n');
	Serial.print(F("<bt info "));
	Serial.print(F("hw:xxxx "));
	Serial.print(F("fw:0.0.1 "));
	Serial.print(F("sn "));
	Serial.write((char)EEPROM.read(0));
	Serial.write((char)EEPROM.read(1));
	Serial.write((char)EEPROM.read(2));
	Serial.write((char)EEPROM.read(3));
	Serial.write((char)EEPROM.read(4));
	Serial.write((char)EEPROM.read(5));
	Serial.print(F(" wa:0650"));
	Serial.print(F(" sc:0000"));  //Error code storage to be defined.
	Serial.print(F(">"));
	Serial.write('\n');
}
