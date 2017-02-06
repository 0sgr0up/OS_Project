// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include <string.h>
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//Begin code changes by Hoang Pham
void
CheckInput(int which)
{
	char input[100];
	printf("\nPlease enter your input: ");
	gets(input);
	printf("\nYou just entered: %s\n", input);
	bool isDigit = true;
	bool isNeg = false;
	bool hasDot = false;
	bool isChar = false;
	
	for(int i = 1 ; input[i] != '\0'; i++){
		char temps[1];
		temps[0] = input[i];
		//printf("\n%d",atoi(temps));
		int temp = atoi(temps);

		if(temp==0){
			if(input[i] == '.' && !hasDot)
				hasDot = true;
			else if(input[i] == '0')
				continue;
			else{
				isDigit = false;
				isChar = true;
				break;
			} 
		}
		else {
			//isDigit = true;
			continue;
		}

	}
	char *sign;
	char atemp[1];
	atemp[0] = input[0];
	if(input[0] == '-'){
		sign = "negative";
	}
	else if(atoi(atemp) != 0 || input[0] == '0'){
		sign = "positive";
		isDigit = true;	
	}
	else
		isChar = true;

	if(isChar)
		printf("\nYour input is just characters");
	else if(isDigit){
		if(!hasDot)
			printf("\nYour input is an %s integer", sign);
		else 
			printf("\nYour input is a %s decimal", sign);
	}	
	 
	printf("\n");
}

void 
ShoutOutLoud(int which)
{
	const char *a[8];
	a[0] = "We are watching the Packers and Falcons game";
	a[1] = "We are drinking rasperry sprite";
	a[2] = "This is a shouting";
	a[3] = "I like to talk to you";
	a[4] = "This girl likes my dog";
	a[5] = "Such a inspiration";
	a[6] = "He is a billionaire nerd";
	a[7] = "I will drive to visit you";
	
	int i = 0;
	int time = 1;
	
	while(i < which){
		int index = Random()%8;
		printf("%s said %s, and has shouted %d time(s)\n",currentThread->getName(),a[index],time);
		for(int j = 0; j < (Random()%5+2); j++)
			currentThread->Yield();	
		time++;
		i++;
	}
}	

void 
pickleft(int id){
	while(chopstick[id]){
		//waiting
		currentThread->Yield();
	}
	if(!chopstick[id]){
		printf("\nPhilosopher %d picks up left chopstick", id);
		chopstick[id] = true;	
	}
}

void 
pickright(int id){
	int idc = (id + 1)%NOPhilo;
	while(chopstick[idc]){
		//waiting
		currentThread->Yield();
	}
	if(!chopstick[idc]){
		printf("\nPhilosopher %d picks up right chopstick", id);
		chopstick[idc] = true;	
	}
}

void 
eat(int id, int meal){
	printf("\nPhilosopher %d is eating meal#%d",id, meal);
	for(int i = 0; i < Random()%5+2; i ++){
		currentThread->Yield();
	}
}

void
putleft(int id){
	printf("\nPhilosopher %d puts down left chopstick", id);
	chopstick[id] = false;
}

void
putright(int id){
	int idc = (id + 1)%NOPhilo;
	printf("\nPhilosopher %d puts down right chopstick", id);
	chopstick[idc] = false;
}

void 
think(int id){
	printf("\nPhilosopher %d begin thinking",id);
	for(int i = 0; i < Random()%5+2; i ++){
		currentThread->Yield();
	}	
}

void 
Dining(int id)
{
	int currentMeal = 0;
	printf("\n%s said: Sitting down.",
		currentThread->getName());
	currentThread->Yield();
	while(currentMeal < NOMeal){
		pickleft(id);
		pickright(id);
		eat(id, currentMeal);
		putleft(id);
		putright(id);
		think(id);
		currentMeal++;
	}
	doneDining++;
	// printf("\nThis is doneDining number: %d",doneDining);
	if(doneDining == NOPhilo)
		for(int i =0 ;i <NOPhilo; i++)
			printf("\nPhilosopher %d is leaving the table",i);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
	
    DEBUG('t', "Entering ThreadTest");
	if(CMD == 1){ 
		//printf("hola and CMD = %d", CMD);
		Thread *t = new Thread("forked thread");
    		t->Fork(CheckInput,1);
	}
	else if(CMD ==2){
		//printf("Bonjour and CMD = %d",CMD);
		printf("\nPlease enter number of threads: ");
		char num[10];
		gets(num);
		while(atoi(num)==0){
			printf("\nIncorrect Input\nPlease enter number of threads: ");
			gets(num);
		}
		printf("\nPlease enter number of shout for each thread: ");
		char shout[10];
		gets(shout);
		while(atoi(shout)==0){
			printf("\nIncorrect Input\nPlease enter number of shout for each thread: ");
			gets(shout);
		}
		Thread *thread;// = new Thread("shouted thread");
	
		int num1 = atoi(num);
		int shout1 = atoi(shout);
		int ii = 1;
		char * which;
		if(num1 && shout1){
		//printf("%d %d",num1,shout1);
			for(int i = 1; i <= num1; i++){
				//printf("%s\n",a[i]);
				which = new char[10];
				sprintf(which, "Thread %d",ii);
				ii++;
		//		printf("%s\n",which);
				thread = new Thread(which);	
				thread->Fork(ShoutOutLoud,shout1);
			}
		}
		else{
			printf("\nYou have entered invalid value for number of thread or/and number of shouting each thread");
		}	
	}
	else if(CMD == 3){
		printf("\nPlease enter number of philosophers: ");
		char num[10];
		gets(num);
		while(atoi(num)==0){
			printf("\nIncorrect Input\nPlease enter number of philosophers: ");
			gets(num);
		}
		printf("\nPlease enter number of meals for each philosophers: ");
		char shout[10];
		gets(shout);
		while(atoi(shout)==0){
			printf("\nIncorrect Input\nPlease enter number of meals for each philosophers: ");
			gets(shout);
		}
		Thread *thread;// = new Thread("shouted thread");
	
		int nop = atoi(num); //nop: number of philosophers
		NOPhilo = nop;
		chopstick = new bool[nop];

		//Populate chopstick slot: all false - chopstick is not in use
		for(int i=0; i < nop ;i++){
			chopstick[i] = false;
		}

		int nom = atoi(shout); //nom: number of meals
		NOMeal = nom;
		int ii = 1;
		char * which;
		if(nop && nom){
			for(int i = 0; i < nop; i++){
				// printf("Philosopher %d sits down\n",);
				which = new char[100];
				sprintf(which, "Philosophers %d",i);
				thread = new Thread(which);	
				if(CMD == 2)
					thread->Fork(Dining,i);
				else if(CMD == 3)
					thread->Fork(DiningS,i);
			}
		}
		else{
			printf("\nYou have entered invalid value for number of philosophers or/and number of meals for each philosophers");
		}

		printf("\n");
	}
}
//End code changes by Hoang Pham
