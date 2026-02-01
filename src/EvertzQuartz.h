/*
 *    EvertzQuartz.cpp - library for controlling routers using the Evertz Quartz protocol
 *    Copyright (c) 2025 Louis Icke
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef EvertzQuartz_h
#define EvertzQuartz_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class EvertzQuartz {
  public:
    /* 
    * Create a new controler 
    * Params :
    * stream		stream object for commands to be sent to
    * timeout		milliseconds to wait for command revertive
    * debugPtr  stream object for debug messages
    */
    EvertzQuartz(Stream& stream, int timeout = 5000, Stream* debugPtr = nullptr);
    
    /* 
    * Tests router connection
    * Returns :
    * bool		returns true router acknowledges
    */
    bool test();
    
    /* 
    * Set crosspoint on router, multiple levels can be specified
    * Params :
    * levels		router levels to set. Legel levels are V,A,B,C,D,E,F,G for 8 level systems
    *			and V,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O for 16
    * dest		destination number
    * source		source number
    * Returns :
    * bool		returns true if router accepts with revertive
    */
    bool setXPT(String levels, int dest, int source);
    
    /* 
    * Not currently implemented
    */
    bool mSetXPT();
    
    /* 
    * Locks destination, inhibiting changing source
    * Params :
    * dest		destination number
    * Returns :
    * bool		returns true if destination is locked
    */
    bool destLock(int dest);
    
    /* 
    * Unlocks destination, allowing changing source
    * Params :
    * dest		destination number
    * Returns :
    * bool		returns true if destination is locked
    */
    bool destUnlock(int dest);
    
    /* 
    * Gets destination lock, inhibiting changing source
    * Params :
    * dest		destination number
    * Returns :
    * bool		returns true if destination is locked
    */
    bool getDestLock(int dest);
    
    /* 
    * Fires system salvo
    * Params :
    * salvo		salvo number between 1 and 32
    * Returns :
    * bool		returns true acknowledged by router
    */
    bool fireSalvo(int salvo);
    
    /* 
    * Interrogates route.
    * Params :
    * level		router level. Legel levels are V,A,B,C,D,E,F,G for 8 level systems
    *			and V,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O for 16
    * dest		destination number
    * Returns :
    * int		source number of route
    */
    int getRoute(String level, int dest);

  private:
    /* Stream object used to send commands */
    Stream& serial;

    /* Stream pointer for debugging */
    Stream* debugPtr;
    
    /*
    * Sends commmand to router
    * Params :
    * String    Command to send
    * Returns :
    * int       Number of bytes written
    */
    int sendCommand(String command);

    /* Returns revertive from router */
    String getRevertive();

    /*
    * Converts int to String with padding
    * Params :
    * integer		int to be parsed
    * Returns :
    * String		String with padded zeros
    */
    String intToString(int integer);
};

#endif	//EvertzQuartz.h

