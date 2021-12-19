/*
 ============================================================================
 Name        : c_project_trial.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define WITH_ENGINE_TEMP_CONTROLLER 1
enum mode{off,on};
enum speed_level{high=100,low=30,stop=0};
void (*ptr_func)(void) = NULL; //pointer to function

typedef struct vehicle{
	enum mode engine;
	enum mode Ac;
	enum mode EngineTempController;
	enum speed_level speed;
	char trafficColor;
	float roomTemprature;
	float engineTemprature;
}vehicle;
vehicle car;
char askUserWishes(void)	//func asks the user to turn on/off/or quit and returns the input
{
	char input;
	printf("choose one of the following:\n");
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n\n");
	scanf(" %c",&input);

	return input;
}

char SensorsSetMenu(void)	//func displays the sensors set menu and sets the vehicle "car" members "room tempr-engine on/off ext.."
{
	char answer;
	printf("Sensors set menu:\n");
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature (Temperature Sensor)\n");
#if WITH_ENGINE_TEMP_CONTROLLER==1	//if ==0 this part wont be seen by the compiler
	printf("d. Set the engine temperature (Engine Temperature Sensor)\n");
#endif
	scanf(" %c",&answer);
	switch(answer){
	case 'a':
		printf("Vehicle engine turned off\n");
		car.engine=off;
		break;
	case 'b':
		printf("Enter traffic light color:\n");
		scanf(" %c",&car.trafficColor);
		break;
	case 'c':
		printf("Enter the room temperature:\n");
		scanf(" %f",&car.roomTemprature);
		break;
	case 'd':
		printf("Enter the engine temperature:\n");
		scanf(" %f",&car.engineTemprature);
		break;
	}
	return answer;
}
void setSpeed(vehicle* car){	//used call by refrence to change into the car struct itself in memory
	if ((car->trafficColor=='G')||(car->trafficColor=='g')){		//if green set speed=100
			car->speed=high;

		}
	 if ((car->trafficColor=='O')||(car->trafficColor=='o')){		//if orange set speed=30
			car->speed=low;
			car->Ac=on;
		    car->roomTemprature=car->roomTemprature*(1.25)+1;		//if the speed=30 turn on Ac and engine temp controll and calulations
			car->EngineTempController=on;
			car->engineTemprature=car->engineTemprature*(1.25)+1;
		}
	 if ((car->trafficColor=='R')||(car->trafficColor=='r')){		//if red stop the car
			car->speed=stop;
		}

	else printf("only the following charachters are valid:G-g-O-o-R-r\n");

}
void setRoomTemp(vehicle* car)
{

	if ((car->roomTemprature==20)&&(car->Ac==on)){}		//do nothing
	else if((car->roomTemprature<10)||(car->roomTemprature>30)){
			car->roomTemprature=20;
			car->Ac=on;
		}
	else if((car->roomTemprature>=10)&&(car->roomTemprature<=30)){
				car->Ac=off;
			}
	if((car->trafficColor=='O')||(car->trafficColor=='o'))
		{
			car->Ac=on;
			car->roomTemprature=car->roomTemprature*(1.25)+1;
		}


}
#if WITH_ENGINE_TEMP_CONTROLLER==1
void setEngineTemp(vehicle* car)
{

	if((car->engineTemprature==125)&&(car->EngineTempController==on)){}	//do nothing
	else if((car->engineTemprature>=100)&&(car->engineTemprature<=150)){
		car->EngineTempController=off;
	}
	else if((car->engineTemprature<100)||(car->engineTemprature>150))
	{
		car->EngineTempController=on;
		car->engineTemprature=125;
	}
	if((car->trafficColor=='O')||(car->trafficColor=='o')){
			car->EngineTempController=on;
			car->engineTemprature=car->engineTemprature*(1.25)+1;
		}


}
#endif
void display(vehicle* car){
	switch(car->engine){
	case on:
		printf("Engine state: on\n");
		break;
	case off:
			printf("Engine state: off\n");
			break;
	}
	switch(car->Ac){
		case on:
			printf("AC: on\n");
			break;
		case off:
				printf("AC: off\n");
				break;
		}
	printf("Vehicle Speed:%d km/hour\n",car->speed);
	printf("Room Temperature:%f c\n",car->roomTemprature);
#if WITH_ENGINE_TEMP_CONTROLLER==1	//if ==0 this part wont be seen by the compiler
	switch(car->EngineTempController){
			case on:
				printf("Engine Temperature Controller State: on\n");
				break;
			case off:
					printf("Engine Temperature Controller State: off\n");
					break;
			}

	printf("Engine Temperature:%f c\n\n",car->engineTemprature);
#endif

}
void setInitialValues(vehicle *car){	//all the struct vehicle car members are given initial values here
	car->Ac=off;
	car->EngineTempController=off;
	car->engine=off;
	car->engineTemprature=105;
	car->roomTemprature=12;
	car->speed=stop;
	car->trafficColor='G';
}


int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	char input;		//holds the user choice "whether to turn the vehicle on/off/quit"
    void (*ptr_func)(vehicle*) = NULL;		 //pointer to function to choose function
	setInitialValues(&car);	//values initialization
	while(1)	/*infinte loop so in case the user turned the engine off,
				they get asked again to turn it on/off/quit */
	{
		input=askUserWishes();	//the user is asked to turn on/off/quit
		switch(input){
				case 'a':	//turn on is chosen
					printf("Vehicle engine turned on\n");
					car.engine=on;
					for(;;){	/* infinite loop so the sensor set menu
					 	 	 	 appears again after the user chooses to set a value and set that value*/
						int sensor_choice=SensorsSetMenu();
						if(sensor_choice=='a')break;//if user choose turn off engine we go out of this loop so we can ask again on/off/quit
						else if(sensor_choice=='b'){ptr_func=setSpeed;}
						else if(sensor_choice=='c'){ptr_func=setRoomTemp;}
#if WITH_ENGINE_TEMP_CONTROLLER==1	//if ==0 this part wont be seen by the compiler
						else if(sensor_choice=='d'){ptr_func=setEngineTemp;}
#endif
						(*ptr_func)(&car); /* call the chosen function using pointer to function
												and notice parameter call by refrence*/
						//setSpeed(&car);
						//setRoomTemp(&car);
#if WITH_ENGINE_TEMP_CONTROLLER==1	//if ==0 this part wont be seen by the compiler
						//setEngineTemp(&car);
#endif
						display(&car);
					}
					break;
				case 'b':
					printf("Vehicle engine turned off\n");
					car.engine=off;
					break;
				case 'c':
					printf("Quit\n");
					exit(0);	//quits the program and you have to run again
					break;	//the program will loop again unless quit is entered
				}
	}

	return 0;
}
