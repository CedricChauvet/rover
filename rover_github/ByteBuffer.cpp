/*
  ByteBuffer.cpp - A circular buffer implementation for Arduino
  Created by Sigurdur Orn, July 19, 2010.
  siggi@mit.edu
  Updated by GreyGnome (aka Mike Schwager) Mon Apr  8 21:11:15 CDT 2013
  	Fixed putString() so it reenables inputs correctly. In the if (length == capacity) section,
	it was missing this line:
			SREG = oldSREG; // Restore register; reenables interrupts
  Updated by GreyGnome (aka Mike Schwager) Thu Feb 23 17:25:14 CST 2012
  	added the putString() method and the fillError variable.
	added the checkError() and resetError() methods.  The checkError() method resets the fillError variable
	to false as a side effect.
	added the ByteBuffer(unsigned int buf_size) constructor.
	added the init() method, and had the constructor call it automagically.
	Also made the capacity, position, length, and fillError variables volatile, for safe use by interrupts.
	Mon Dec  3 07:55:04 CST 2012
	Added the putHex() and putDec() methods.
 */
 
#include "ByteBuffer.h"
  byte* data;
  int capacity;
  int position;
  int length;
  bool fillError;

/*void init(){
	init(DEFAULTBUFSIZE);
}*/

void init(unsigned int buf_length){
	data = (byte*)malloc(sizeof(byte)*buf_length);
	capacity = buf_length;
	position = 0;
	length = 0;
	fillError=false;
}

// Arduino 1.0: free() doesn't free.  :-(  This is a no-op as of 11/2012.
void deAllocate(){
	free(data);
}

void clear(){
	position = 0;
	length = 0;
}

void resetError(){
	fillError=false;
}

boolean checkError(){
	/*
	if (fillError) {
		Serial.print("E: checkError: length ");
		Serial.println(length, DEC);
	}
	*/

	boolean result=fillError;
	fillError=false;
	return(result);
}

int getSize(){
	return length;
}

int getCapacity(){
	return capacity;
}

byte peek(unsigned int index){
	byte b = data[(position+index)%capacity];
	return b;
}

uint8_t put(byte in){
	if(length < capacity){
		// save data byte at end of buffer
		data[(position+length) % capacity] = in;
		// increment the length
		length++;
		return 1;
	}
	// return failure
	//Serial.print("E: put: ");
	//Serial.println(length, DEC);
	fillError=true;
	return 0;
}


uint8_t putString(const char *in) {
	return(putString((char *) in));
}

uint8_t putString(char *in){
	uint8_t count=0;
	char *inString;

	inString=in;
	uint8_t oldSREG = SREG; cli();
	while(length <= capacity){
		if (length == capacity) {
			fillError=true;
			SREG = oldSREG; // Restore register; reenables interrupts
			return count;
		}
		// save data byte at end of buffer
		data[(position+length) % capacity] = *inString;
		// increment the length
		length++;
		inString++;
		count++;
		if (*inString == 0) {
			if (count==0) fillError=true; // Serial.println("E: putString"); };
			SREG = oldSREG; // Restore register; reenables interrupts
			return count;
		}
	}
	SREG = oldSREG; // Restore register; reenables interrupts
	return count;
}

uint8_t putInFront(byte in){
	uint8_t oldSREG = SREG; cli();
	if(length < capacity){
			// save data byte at end of buffer
			if( position == 0 )
					position = capacity-1;
			else
					position = (position-1)%capacity;
			data[position] = in;
			// increment the length
			length++;
			SREG = oldSREG; // Restore register; reenables interrupts
			return 1;
	}
	// return failure
	//Serial.println("E: putInFront");
	fillError=true;
	SREG = oldSREG; // Restore register; reenables interrupts
	return 0;
}

// Returns 0 if length of data is 0.
byte getdata(){
	uint8_t oldSREG = SREG; cli();
	byte b = 0;

	if(length > 0){
		b = data[position];
		// move index down and decrement length
		position = (position+1)%capacity;
		length--;
	}
	SREG = oldSREG; // Restore register; reenables interrupts
	return b;
}

byte getFromBack(){
	byte b = 0;
	if(length > 0){
		uint8_t oldSREG = SREG; cli();
		b = data[(position+length-1)%capacity];
		length--;
		SREG = oldSREG; // Restore register; reenables interrupts
	}

	return b;
}

//
// Ints
//

void putIntInFront(int in){
    byte *pointer = (byte *)&in;
	putInFront(pointer[0]);	
	putInFront(pointer[1]);	
}

void putInt(int in){
    byte *pointer = (byte *)&in;
	put(pointer[1]);	
	put(pointer[0]);	
}


int getInt(){
	int ret;
    byte *pointer = (byte *)&ret;
	pointer[1] = getdata();
	pointer[0] = getdata();
	return ret;
}

int getIntFromBack(){
	int ret;
    byte *pointer = (byte *)&ret;
	pointer[0] = getFromBack();
	pointer[1] = getFromBack();
	return ret;
}

void putHex(uint8_t theByte) {
	put('0'); put('x');
    uint8_t hinybble=theByte>>4;
    uint8_t lonybble=theByte & 0x0F;
	uint8_t addend=0;
	if (hinybble >= 0x0a) addend=7;
	put(hinybble+48+addend);
	if (lonybble >= 0x0a) addend=7;
	else addend=0;
	put(lonybble+48+addend);
}

void putDec(uint8_t number) {
  uint8_t hundreds=0;
  uint8_t tens=0;
  uint8_t ones=0;
  uint8_t tmp=number;

  while (tmp >= 100 ) {
    hundreds++;
    tmp-=100;
  }
  while (tmp >= 10 ) {
    tens++;
    tmp-=10;
  }
  ones=tmp;
  hundreds+=48; tens+=48; ones+=48;
  if (number >= 100) { put(hundreds); }
  if (number >= 10) {  put(tens); }
  put(ones);
}

void putDec(int8_t number) {
	uint8_t absNumber=abs(number);
	if (number < 0) put('-');
	putDec(absNumber);
}

//
// Longs
//

void putLongInFront(long in){
    byte *pointer = (byte *)&in;
	putInFront(pointer[0]);	
	putInFront(pointer[1]);	
	putInFront(pointer[2]);	
	putInFront(pointer[3]);	
}

void putLong(long in){
    byte *pointer = (byte *)&in;
	put(pointer[3]);	
	put(pointer[2]);	
	put(pointer[1]);	
	put(pointer[0]);	
}


long getLong(){
	long ret;
    byte *pointer = (byte *)&ret;
	pointer[3] = getdata();
	pointer[2] = getdata();
	pointer[1] = getdata();
	pointer[0] = getdata();
	return ret;
}

long getLongFromBack(){
	long ret;
    byte *pointer = (byte *)&ret;
	pointer[0] = getFromBack();
	pointer[1] = getFromBack();
	pointer[2] = getFromBack();
	pointer[3] = getFromBack();
	return ret;
}


//
// Floats
//

void putFloatInFront(float in){
    byte *pointer = (byte *)&in;
	putInFront(pointer[0]);	
	putInFront(pointer[1]);	
	putInFront(pointer[2]);	
	putInFront(pointer[3]);	
}

void putFloat(float in){
    byte *pointer = (byte *)&in;
	put(pointer[3]);	
	put(pointer[2]);	
	put(pointer[1]);	
	put(pointer[0]);	
}

float getFloat(){
	float ret;
    byte *pointer = (byte *)&ret;
	pointer[3] = getdata();
	pointer[2] = getdata();
	pointer[1] = getdata();
	pointer[0] = getdata();
	return ret;
}

float getFloatFromBack(){
	float ret;
    byte *pointer = (byte *)&ret;
	pointer[0] = getFromBack();
	pointer[1] = getFromBack();
	pointer[2] = getFromBack();
	pointer[3] = getFromBack();
	return ret;
}


