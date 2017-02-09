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
#include "synch.h"
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
int NOMeal;
bool * chopstick;
bool allEntered;
bool allEated;
int doneDining;
int currentMeal;

int NOPeople;
int MSize;
int NOMail;
int totalSent;

int ** mailbox;
const char * mailContent[10];

Semaphore ** chopstickS;

void SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//Begin code changes by Hoang Pham
void CheckInput(int which)
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

void ShoutOutLoud(int which)
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
		for(int j = 0; j < (Random()%4+2); j++)
			currentThread->Yield();	
		time++;
		i++;
	}
}	

void pickleft(int id){
	while(chopstick[id]){
		//waiting
		currentThread->Yield();
	}
	if(!chopstick[id]){
		printf("\nPhilosopher %d picks up left chopstick", id);
		chopstick[id] = true;
	}
}

void pickleftS(int id){
	printf("\nPhilosopher %d picks up left chopstick", id);
	chopstickS[id]->P(); 
}

void pickright(int id){
	int idc = (id + 1)%NOPhilo;
	if(NOPhilo == 1)
		idc = 1;
	while(chopstick[idc]){
		//waiting
		currentThread->Yield();
	}
	if(!chopstick[idc]){
		printf("\nPhilosopher %d picks up right chopstick", id);
		chopstick[idc] = true;	
	}
}

void pickrightS(int id){
	printf("\nPhilosopher %d picks up right chopstick", id);
	int idc = (id+1)%NOPhilo;
	chopstickS[idc]->P(); 
}

void eat(int id, int meal){
	printf("\nPhilosopher %d is eating meal#%d",id, meal);
	currentMeal++;
	for(int i = 0; i < Random()%4+2; i ++){
		currentThread->Yield();
	}	
}

void putleft(int id){
	printf("\nPhilosopher %d puts down left chopstick", id);
	chopstick[id] = false;
}

void 
putleftS(int id){
	printf("\nPhilosopher %d puts down left chopstick", id);
	chopstickS[id]->V();
}

void putright(int id){
	int idc = (id + 1)%NOPhilo;
	if(NOPhilo == 1)
		idc = 1;
	printf("\nPhilosopher %d puts down right chopstick", id);
	chopstick[idc] = false;
}

void putrightS(int id){
	printf("\nPhilosopher %d puts down right chopstick", id);
	int idc = (id + 1)%NOPhilo;
	chopstickS[idc]->V();
}

void think(int id){
	printf("\nPhilosopher %d begin thinking",id);
	for(int i = 0; i < Random()%4+2; i ++){
		currentThread->Yield();
	}	
}

void Dining(int id)
{
	printf("\n%s said: Sitting down.",
		currentThread->getName());
	currentThread->Yield();
	while(currentMeal <= NOMeal){
		pickleft(id);
		pickright(id);
		if((currentMeal-1) == NOMeal){
			putleft(id);
			putright(id);
			break;
		}
		eat(id, currentMeal);
		putleft(id);
		putright(id);
		if((currentMeal-1) == NOMeal){
			break;
		}
		think(id);
	}
	doneDining++;
	while(doneDining<NOPhilo)
		currentThread->Yield();
	printf("\n%s is leaving the table",currentThread->getName());
	currentThread->Finish();
}

void DiningS(int id){
	printf("\n%s said: Sitting down.",
		 currentThread->getName());
	currentThread->Yield();
	while(currentMeal <= NOMeal){
		pickleftS(id);
		pickrightS(id);
		printf("\nCurrent meal %d",currentMeal);
		if((currentMeal-1) == NOMeal){
			putleftS(id);
			putrightS(id);
			break;
		}
		eat(id, currentMeal);
		putleftS(id);
		putrightS(id);
		if((currentMeal-1) == NOMeal)
			break;
		think(id);
	}
	doneDining++;
	while(doneDining < NOPhilo)
		currentThread->Yield();
	printf("\n%s is leaving the table",currentThread->getName());
	currentThread->Finish();		
}

bool 
checkInt(char * var){
	for(int i = 0; var[i] != '\0'; i++){
		if(var[i] < '0' || var[i] > '9')
			return true;
	}
	return false;
}

bool hasMail(int id){
	for(int i = 0; i < MSize; i++){
		if(mailbox[id][i] != 9999)
			return true;
	}
	return false;
}

void read(int id){
	for (int i = 0; i <MSize; i++){
		if(mailbox[id][i] != 9999){
			printf("\n%s received an email with content: \"%s\"",
				currentThread->getName(),mailContent[mailbox[id][i]]);
			mailbox[id][i] = 9999;
			break;
		}
	}
}

void wait(int id){
	printf("\n%s begin waiting",currentThread->getName());
	for(int i = 0; i < Random()%4+2; i ++){
		currentThread->Yield();
	}	
}

bool isFull(int id){
	for(int i = 0; i < MSize; i++){
		if(mailbox[id][i] == 9999)
			return false;
	}
	return true;
}

void composeMail(int id){
	if(totalSent >= NOMail)
		return;
	else{
		int mailTo = Random()%NOPeople;
		while(mailTo == id){
			mailTo = Random()%NOPeople;
		}
		while(isFull(mailTo)){
			currentThread->Yield();
		}
		for(int i = 0; i<MSize;i++){
			if(mailbox[mailTo][i] == 9999){
				mailbox[mailTo][i] = Random()%10;
				printf("\n%s just sent a mail to Person #%d",
					currentThread->getName(), mailTo);
				break;
			}
		}
	}
}

void PostOffice(int id){
	while(totalSent < NOMail){
		printf("\n%s enters the post office"
				,currentThread->getName());
		while(hasMail(id)){
			read(id);
			currentThread->Yield();
		}
		composeMail(id);
		totalSent++;
		printf("\n%s leaves the post office",currentThread->getName());
		wait(id);
	}
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
	else if(CMD == 3 | CMD == 4){
		printf("\nPlease enter number of philosophers: ");
		char * num = new char[100];
		gets(num);
		//char * num1 = num;
		while(atoi(num)==0  || checkInt(num)){
			printf("\nIncorrect Input\nPlease enter number of philosophers: ");
			gets(num);
		}
		printf("\nPlease enter total number of meals for philosophers: ");
		char * shout = new char[100];
		gets(shout);
		// char * shout1 = shout;
		while(atoi(shout)==0 || checkInt(shout)){
			printf("\nIncorrect Input\nPlease enter total number of meals for philosophers: ");
			gets(shout);
		}
		Thread *thread;// = new Thread("shouted thread");
	
		int nop = atoi(num); //nop: number of philosophers
		NOPhilo = nop;
		if(NOPhilo==1){
			printf("\nSpecial case");
			nop =2;
		}
		chopstick = new bool[nop];
		chopstickS = new Semaphore * [nop];
		currentMeal=1;
		doneDining=0;

		//Populate chopstick slot: all false - chopstick is not in use
		for(int i=0; i < nop ;i++){
			chopstick[i] = false;
		}

		char * nameS;
		//Populate value for chopstick semaphore
		for(int i=0; i < nop ;i++){
			nameS = new char[100];
			sprintf(nameS, "Chopstick %d",i);
			chopstickS[i] = new Semaphore(nameS,1);
		}
		int nom = atoi(shout); //nom: number of meals
		NOMeal = nom;
		int ii = 1;
		char * which;
		if(NOPhilo && NOMeal){
			for(int i = 0; i < NOPhilo; i++){
				// printf("Philosopher %d sits down\n",);
				which = new char[100];
				sprintf(which, "Philosopher %d",i);
				thread = new Thread(which);	
				if(CMD == 3)
					thread->Fork(Dining,i);
				else if(CMD == 4)
					thread->Fork(DiningS,i);
			}
		}
		else{
			printf("\nYou have entered invalid value for number of philosophers or/and number of meals for each philosophers");
		}
	}
	else if(CMD == 5 || CMD == 6){

		mailContent[0] = "Hello there, Im about to graduate";
		mailContent[1] = "Hey! It is OS time";
		mailContent[2] = "Im hungry. Lets do Dining Philosopher";
		mailContent[3] = "No, sorry. Can not dine right now";
		mailContent[4] = "I have to go to the Post office";
		mailContent[5] = "But for what ?";
		mailContent[6] = "To wait in the busy line ?";
		mailContent[7] = "Nah not really. I have Semaphore";
		mailContent[8] = "Sounds fun. Good luck";
		mailContent[9] = "Thanks! You know I will";

		printf("\nPlease enter number of people go to the post office: ");
		char * numpeople = new char[100];
		gets(numpeople);
		//char * num1 = num;
		while(atoi(numpeople)==0  || checkInt(numpeople)){
			printf("\nIncorrect Input\nPlease enter number of people go to the post office: ");
			gets(numpeople);
		}
		printf("\nPlease enter the size of each mail box: ");
		char * mailsize = new char[100];
		gets(mailsize);
		// char * shout1 = shout;
		while(atoi(mailsize)==0 || checkInt(mailsize)){
			printf("\nIncorrect Input\nPlease enter the size of each mail box: ");
			gets(mailsize);
		}
		printf("\nPlease enter the total number of messages to be sent: ");
		char * nummail = new char[100];
		gets(nummail);
		// char * shout1 = shout;
		while(atoi(nummail)==0 || checkInt(nummail)){
			printf("\nIncorrect Input\nPlease enter the size of each mail box: ");
			gets(nummail);
		}

		NOPeople = atoi(numpeople);
		MSize = atoi(mailsize);
		NOMail = atoi(nummail);
		totalSent = 0;

		mailbox = new int*[NOPeople];

		Thread * thread;

		for(int i = 0; i < NOPeople; i++){
			mailbox[i] = new int[MSize];
			for(int j = 0; j < MSize; j++){
				//Initialize mail box data
				mailbox[i][j] = 9999;
			}
			char * peoplename = new char [100]	;
			sprintf(peoplename, "Person #%d", i );
			thread = new Thread(peoplename);
			thread->Fork(PostOffice,i);
		}
	}
}
//End code changes by Hoang Pham
