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

#include "EvertzQuartz.h"

EvertzQuartz::EvertzQuartz(Stream& stream, int timeout):serial(stream) {
  serial.setTimeout(timeout);
}

String EvertzQuartz::getRevertive() {
  return serial.readStringUntil('\r');
}

String EvertzQuartz::intToString(int integer) {
  String stringValue = String(integer);
  if (stringValue.length() == 1) {
    return "00" + stringValue;
  } else if (stringValue.length() == 2) {
    return "0" + stringValue;
  } else {
    return stringValue;
  }
}

bool EvertzQuartz::test() {
  // Send test command
  serial.print(".#01\r");
  
  String revertive = getRevertive();
  
  if (revertive == ".A") {
    return true;
  } else {
    return false;
  }
}

bool EvertzQuartz::setXPT(String levels, int dest, int source) {
  // Construct command
  String command = ".S" + levels + dest + "," + source + "\r";

  // Send xput command
  serial.print(command);
  
  // Get revertive
  String revertive = getRevertive();

    if (revertive == ".U" + levels + intToString(dest) + "," + intToString(source)) {
    return true;
  } else {
    return false;
  }
}

bool EvertzQuartz::destLock(int dest) {
  // Construct command
  String command = ".BL" + dest + "\r";

  // Send command
  serial.print(command);

  // Get revertive
  String revertive = getRevertive();

  if (revertive == ".BA" + intToString(dest) + ",0") {
    return false;
  } else {
    return true;
  }
}

bool EvertzQuartz::destUnlock(int dest) {
  // Construct command
  String command = ".BU" + dest + "\r";

  // Send command
  serial.print(command);

  // Get revertive
  String revertive = getRevertive();

  if (revertive == ".BA" + intToString(dest) + ",0") {
    return false;
  } else {
    return true;
  }
}

bool EvertzQuartz::getDestLock(int dest) {
  // Construct command
  String command = ".BI" + dest + "\r";

  // Send command
  serial.print(command);

  // Get revertive
  String revertive = getRevertive();

  if (revertive == ".BA" + intToString(dest) + ",0") {
    return false;
  } else {
    return true;
  }
}

bool EvertzQuartz::fireSalvo(int salvo) {
  // Construct command
  String command = ".F" + salvo + "\r";

  // Send command
  serial.print(command);

  // Get revertive
  String revertive = getRevertive();

  if (revertive == ".A") {
    return true;
  } else {
    return false;
  }
}

int getRoute(char level, int dest) {
  // Construct command
  String command = ".I" + level + dest + "\r";

  // Send command
  serial.print(command);

  // Get revertive
  String revertive = getRevertive();

  return revertive.substr(7, revertive.length()).toInt();
}

