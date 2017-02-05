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
int NOPhilo;
int NOChop;
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
Dining(int nom)
{
	printf("\nThis is %s and we have %d chopsticks",
		currentThread->getName(),NOChop);
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
		NOChop = nop;
		int nom = atoi(shout); //nom: number of meals
		int ii = 1;
		char * which;
		if(nop && nom){
		//printf("%d %d",num1,shout1);
			for(int i = 1; i <= nop; i++){
				//printf("%s\n",a[i]);
				which = new char[100];
				sprintf(which, "Philosophers %d",ii);
				ii++;
		//		printf("%s\n",which);
				thread = new Thread(which);	
				thread->Fork(Dining,nom);
			}
		}
		else{
			printf("\nYou have entered invalid value for number of philosophers or/and number of meals for each philosophers");
		}	
	}
}
//End code changes by Hoang Pham
