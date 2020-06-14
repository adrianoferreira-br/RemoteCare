#include <Preferences.h>


Preferences preferences;


#define MAX_REPORTS 100

String boxDate[NUMBER_OF_BOXES];
String boxTime[NUMBER_OF_BOXES];

/*
 box : [0 to NUMBER_OF_BOXES-1]
 date : DD/MM 
 time : HH:MM
 tipo : N : nao tomou, E : tomou de outro box, A tomou atrasado, C : tomou corretamente.  
*/
void saveReport(int box, char * date, char * time, char tipo)
{
  String record = "";
  record += box;
  record += ",";
  record += date;
  record += ",";
  record += time;
  record += ",";
  record += tipo;
  preferences.begin("preferences", false); 
  
  int position;
  position = preferences.getShort("repPos",-1);

  if(position++ >= MAX_REPORTS)
    position = 0;
  String field = "report" + position; 
  preferences.putString(field.c_str(),record.c_str());
  preferences.putShort("repPos",position);
  preferences.end();
}


//return json
String getPesistedReport()
{
	String result = "";

	result = "{\"report\"[";
	
	preferences.begin("preferences", false); 
  
  short currentPosition;

  currentPosition = preferences.getShort("repPos",-1);
	bool endFound = false;
	if(currentPosition > -1)
	{
		String field;
		String value;
		bool firstItem = true;
		short itemCounter = 1;
		for(short counter = currentPosition+1; counter < MAX_REPORTS; counter++)
		{
			field = "report" + counter;
		 	value = preferences.getString(field.c_str());
			if(value.length() > 0)
			{
				if(firstItem)
				{
					firstItem = false;
				}
				else
				{
					result += ",";	
				}
				result += "{\"item\":\"";
				result += itemCounter++;
				result += "\",\"value\":\"";
				result += value;
				result += "\"}";
			}
			else
			{
				endFound = true;
				break;
			}
		}
	
		if(!endFound)
		{
			for(short counter = 0; counter <= currentPosition; counter++)
			{
				field = "report" + counter;
		 		value = preferences.getString(field.c_str());
				if(value.length() > 0)
				{
					if(firstItem)
					{
						firstItem = false;
					}
					else
					{
						result += ",";	
					}
					result += "{\"item\":\"";
					result += itemCounter++;
					result += "\",\"value\":\"";
					result += value;
					result += "\"}";
				}
				else
				{
					endFound = true;
					break;
				}
			}
		}
	}
	result+="]}";
	preferences.end();

	return result;

}


void initPersistence()
{
  preferences.begin("preferences", false); 
  for(int counter  = 0; counter < NUMBER_OF_BOXES; counter++)
  { 
    String field = "BoxDate" + counter;
    boxDate[counter] = preferences.getString(field.c_str());
    field = "BoxTime" + counter;
    boxTime[counter] = preferences.getString(field.c_str());
  }
  preferences.end();

}


void persistScheduleBoxes()
{
  preferences.begin("preferences", false); 

  for(int counter  = 0; counter < NUMBER_OF_BOXES; counter++)
  { 
    String field = "BoxDate" + counter;
    preferences.putString(field.c_str(), boxDate[counter].c_str());
    field = "BoxTime" + counter;
    preferences.putString(field.c_str(), boxTime[counter].c_str());
  }
  preferences.end();
}
