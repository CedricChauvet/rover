/*
  ByteBuffer.h - A circular buffer implementation for Arduino
  Created by Sigurdur Orn, July 19, 2010.  siggi@mit.edu
  Updated by GreyGnome (aka Mike Schwager) Thu Feb 23 17:25:14 CST 2012
  	added the putString() method and the fillError variable.
	added the checkError() and resetError() methods.  The checkError() method resets the fillError variable
	to false as a side effect.
	added the ByteBuffer(unsigned int buf_size) constructor.
	added the init() method, and had the constructor call it automagically.
	protected certain sections of the code with cli()/sei() calls, for safe use by interrupts.
	Also made the capacity, position, length, and fillError variables volatile, for safe use by interrupts.
 */
 
#ifndef ByteBuffer_h
#define ByteBuffer_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

//#include <util/atomic.h>
void init(unsigned int buf_size);
	byte getdata();
	// This method initializes the datastore of the buffer to the default size.
	//void init();

	// This method resets the buffer into an original state (with no data)	
	void clear();

	// This method resets the fillError variable to false.
	void resetError();

	// This method tells you if your buffer overflowed at some time since the last
	// check.  The error state will be reset to false.
	boolean checkError();

	// This releases resources for this buffer, after this has been called the buffer should NOT be used
	void deAllocate();

	// Returns how much space is used in the buffer
	int getSize();
	
	// Returns the maximum capacity of the buffer
	int getCapacity();

	// This method returns the byte that is located at index in the buffer but doesn't modify the buffer like the get methods (doesn't remove the retured byte from the buffer)
	byte peek(unsigned int index);

	//
	// Put methods, either a regular put in back or put in front
	// 
	uint8_t putInFront(byte in);
	uint8_t put(byte in);
	uint8_t putString(char *in);

	void putIntInFront(int in);
	void putInt(int in);

	void putLongInFront(long in);
	void putLong(long in);

	void putFloatInFront(float in);
	void putFloat(float in);
#define DEFAULTBUFSIZE 32



#endif

