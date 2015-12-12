/*
 * P-Track Arduino control software
 * Written by J. Buttars
 * 3 DEC 2015
*/ 

#include <Servo.h> 
 
Servo              myservoVert;      // Vertical Axis 
Servo              myservoHorz;      // Horizontal Axis

//////////////////////USER EDITABLE VARIABLES/////////////////////////////////////////////////////////

static const int   VHOME =  85;      // Vertical home position usually found by calibrating
static const int   HHOME =  90;      // Horizontal home location usually found by calibrating
int                Hpos  =  HHOME;   // Variable to store the servo horizontal position 
int                Vpos  =  VHOME;   // Variable to store the servo vertical positon
const int          Vmin  =  25;      // Min Horizontal value, set to '0' when calibrating. 
const int          Vmax  = 115;      // Max Horizontal value, set to '180' when calibration.
const int          Hmin  =  20;      // Min Horizontal value, set to '0' when calibrating.
const int          Hmax  = 150;      // Max Horizontal value, set to '180' when calibration.
const int  Horz_Preset_1 = 120;      // Preset 1, we asume we want same verical postion
const int  Horz_Preset_2 =  90;      // Preset 2, we asume we want same verical postion
const int  Horz_Preset_3 =  57;      // Preset 3, we asume we want same verical postion
bool          invertVert =   1;      // If servo move opposite of desired flip value from '0' to '1'
bool          invertHorz =   0;      // If servo move opposite of desired flip value from '0' to '1'
const int      DELAY_VAL =   1;      // Used to make movement smooth. Adjust if movement is jerky
const int       stepSize =   1;      // Used to increase speed of GoTo function

//////////////////////END USER EDITABLE VARIABLES/////////////////////////////////////////////////////

void setup() 
{ 
  myservoVert.attach(11);  //Attach Vertical Movement Servo
  myservoHorz.attach(10);  //Attach Horizontal Movement Servo
  delay(2000);            //Give the device 2 seconds to aid in programing before we use the serial or connect to bluetooth
  myservoVert.write(VHOME);
  myservoHorz.write(HHOME);
  Serial.begin(9600);
  Serial.println("Welcome To P-TRACK, Device is now ready to use.");
} 
 
void loop() 
{ 
  while (Serial.available() > 0) 
  {
    int move = Serial.read();
    
    if(move == 'a')
    {
      HmoveN();
    }
    
    if(move == 'd')
    {
      HmoveP();
    }
    
    if(move == 'w')
    {
      VmoveP();
    }
    
    if(move == 's')
    {
      VmoveN();
    }
    
    if(move == '1')
    {
      Go_To_Pos(Horz_Preset_1,Vpos);
    }

    if(move == '2') 
    {
       Go_To_Pos(Horz_Preset_2,Vpos);
    }
    
    if(move == '3') 
    {
       Go_To_Pos(Horz_Preset_3,Vpos);
    }
        
    if(move == 'h') //Go Home
    {
      MoveHome();
    }
    
    if(move == 'p') // Stop Serial, 'q' is too close to the other commands
    {
      //EXIT
      exit(0);
    }
  }//End while loop serial
}//End while true loop

void MoveHome()
{
  Go_To_Pos(HHOME,VHOME);
}

void HmoveN()
{
  if(invertHorz == 0)
  {
    Hpos +=stepSize;
    if(Hpos>Hmax)
    {
    Hpos=Hmax;//Don't want to go over
    }
  }
  else
  {
    Hpos -=stepSize;
    if(Hpos<Hmin)
    {
    Hpos=Hmin;//Don't want to go over
    }
  }
  myservoHorz.write(Hpos);
  Serial.print("Horiz Pos: ");
  Serial.println(Hpos, DEC);
  delay(DELAY_VAL);
}//End HmoveN

void HmoveP()
{
    if(invertHorz == 0)
  {
    Hpos -=stepSize;
    if(Hpos<Hmin)
    {
    Hpos=Hmin;//Don't want to go over
    }
  }
  else
  {
    Hpos +=stepSize;
    if(Hpos>Hmax)
    {
    Hpos=Hmax;//Don't want to go over
    }
  }
  myservoHorz.write(Hpos);
  Serial.print("Horiz Pos: ");
  Serial.println(Hpos, DEC);
  delay(DELAY_VAL);
}//End HmoveP

void VmoveP()
{
  if(invertVert == 0)
  {
    Vpos +=stepSize;
    if(Vpos>Vmax)
    {
      Vpos=Vmax;//Don't want to go over
    }
  }
   else
  {
    Vpos -=stepSize;
    if(Vpos<Vmin)
    {
    Vpos=Vmin;//Don't want to go over
    }
  }
  
  myservoVert.write(Vpos);
   Serial.print("Vert Pos: ");
  Serial.println(Vpos, DEC);
  delay(DELAY_VAL);
}//End VmoveP

void VmoveN()
{
  if(invertVert == 0)
  {
    Vpos -=stepSize;
    if(Vpos<Vmin)
    {
    Vpos=Vmin;//Don't want to go over
    }
  }
  else
  {
    Vpos +=stepSize;
    if(Vpos>Vmax)
    {
      Vpos=Vmax;//Don't want to go over
    }
  }
   myservoVert.write(Vpos);
    Serial.print("Vert Pos: ");
   Serial.println(Vpos, DEC);
   delay(DELAY_VAL);
}//end VmoveN

void Go_To_Pos(int Desired_Hpos, int Desired_Vpos)
{
  int tries = 180; //If we got something wrong stop so we can fix it
  while(Desired_Hpos != Hpos && tries >0)
  {
    tries--;
    if(Hpos<Desired_Hpos)
    {
      if(invertHorz == 0)
      {
        HmoveN();
      }
      else
      {
        HmoveP();
      }
    }
    else if(Hpos>Desired_Hpos)
    {
            if(invertHorz == 0)
      {
        HmoveP();
      }
      else
      {
        HmoveN();
      }
    }
    
  }//End while loop Horz 
  tries = 180;
  while(Desired_Vpos != Vpos && tries > 0)
  {
    tries --;
    if(Vpos>Desired_Vpos)
    {
      if(invertVert == 0)
      {
        VmoveN();
      }
      else
      {
        VmoveP();
      }
    }
    else if(Vpos<Desired_Vpos)
    {
            if(invertVert == 0)
      {
        VmoveP();
      }
      else
      {
        VmoveN();
      }
    } 
  }//end while
  return;
}
