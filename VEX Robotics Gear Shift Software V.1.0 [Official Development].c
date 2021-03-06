#pragma config(Sensor, dgtl1,  powerButton,    sensorTouch)
#pragma config(Sensor, dgtl2,  gearShift,      sensorTouch)
#pragma config(Sensor, dgtl3,  gearoneSensor,  sensorTouch)
#pragma config(Sensor, dgtl4,  geartwoSensor,  sensorTouch)
#pragma config(Sensor, dgtl12, errorIndicator, sensorLEDtoVCC)
#pragma config(Motor,  port2,           mainMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           gearshiftMotor, tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*VEX ROBOTICS 2 SPEED GEAR SHIFT SOFTWARE version 1.0 [Official Developement]

DEVELOPER: Austin A. Daigle
DATE WRITTEN AND TESTED: 3-15-2017

NOTE: This program does have a analytics features for gear position. Uppon startup the position of the
gear are check to set the gear variable for changing gear.*/

//Global Variables
int powerbuttonStatus = 0;
int powerbuttonDisable = 0;
int powertoggle = 0;
int previousmainmotorState = 0;
int gearshiftStatus = 0;
int gearshiftDisable = 0;
int gear = 1;


//Clean LED's
void cleanLeds()
{
turnLEDOff(errorIndicator);
}

//Reset to prior motor state Function
void returnmotorpriorState()
{
	if(previousmainmotorState == 0)
	{
		stopMotor(mainMotor);
	}
	else
	{
		startMotor(mainMotor, 127);
	}
}

//Gear changing Function
void shiftgear()
{
	while(gear == 1 && (SensorValue(gearoneSensor) == 0))
	{
		startMotor(mainMotor, 127);
		startMotor(gearshiftMotor, 35);
		wait(0.5);
		stopMotor(mainMotor);
		stopMotor(gearshiftMotor);
	}
	while(gear == 2 && (SensorValue(geartwoSensor) == 0))
	{
		startMotor(mainMotor, 127);
		startMotor(gearshiftMotor, -35);
		wait(0.5);
	}
	returnmotorpriorState();
}


//Mechanical Error Code Function
void mechanicalerrorCode()
{
while(SensorValue(gearoneSensor) == 1 && (SensorValue(geartwoSensor) == 1))
{
turnLEDOn(errorIndicator);
wait(0.2);
turnLEDOff(errorIndicator);
}
}


//Gear Analytics
void analyzegear()
{
if(SensorValue(gearoneSensor) == 1 && (SensorValue(geartwoSensor) == 1))
	{
	//Error Found! Solution: Check system and reset Microcontroller.
	//Display Mechanical Error Code
	mechanicalerrorCode();
	}
else
	{
	if(SensorValue(gearoneSensor) == 0 && (SensorValue(geartwoSensor) == 0))
		{
		//Error Found! Solution: automated correction fuction.
		gear = 1;
		shiftgear();
		}
	else
		{
		if(SensorValue(gearoneSensor) == 1 && (SensorValue(geartwoSensor) == 0))
			{
			gear = 1;
			}
		if(SensorValue(gearoneSensor) == 0 && (SensorValue(geartwoSensor) == 1))
			{
			gear = 2;
			}
		}
}
}


//Gear variable button changer
void gearvarChange()
{
	if(gear == 1)
	{
		gear = 2;
	}
	else
	{
		if(gear == 2)
		{
			gear = 1;
		}
	}
	shiftgear();
}

//Gear shift button variable enable
void gearshiftClear()
{
	if(SensorValue(gearShift) == 0)
	{
		gearshiftDisable = 0;
	}
}

//Power button enable
void powerbuttonClear()
{
	if(SensorValue(powerButton) == 0)
	{
		powerbuttonDisable = 0;
	}
}

//Power toggle Variable reset
void powerstat()
{
	if(powertoggle > 1)
	{
		powertoggle = 0;
	}
}

//Activate the gear shift (button) function
void gearshiftFunction()
{
	if(SensorValue(gearShift) == 1 && gearshiftDisable == 0)
	{
		gearshiftStatus = 1;
	}
	if(gearshiftStatus > 0)
	{
		gearvarChange();
		wait1Msec(100);
		gearshiftStatus = 0;
		gearshiftDisable = 1;
	}
	else
	{
		stopMotor(gearshiftMotor);
	}
	gearshiftClear();
}

//Activate the power button function
void powerbuttonFunction()
{
	if(SensorValue(powerButton) == 1 && powerbuttonDisable == 0)
	{
		powerbuttonStatus = 1;
	}
	if(powerbuttonStatus > 0)
	{
		wait1Msec(250);
		powertoggle = powertoggle + 1;
		powerstat();
		powerbuttonStatus = 0;
		powerbuttonDisable = 1;
		if(powertoggle == 0)
		{
			previousmainmotorState = 0;
			stopMotor(mainMotor);
		}
		else
		{
			previousmainmotorState = 1;
			startMotor(mainMotor,127);
		}
	}
	powerbuttonClear();
}

//Core Execution Script
task main()
{
	cleanLeds();
	analyzegear();
	powerstat();
	while(true)
	{
		powerbuttonFunction();
		gearshiftFunction();
	}
}
