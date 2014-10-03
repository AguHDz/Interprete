//  Brainfuck For Arduino is written by Luigi D'Andrea (www.xappsoftware.com)
//  It is distributed under the BSD license

//  Redistribution and use in source and binary forms, with or without modification, 
//  are permitted provided that the following conditions are met:
//  
//  Copyright (c) 2010, Luigi D’Andrea (www.xappsoftware.com)
//  All rights reserved.
//  
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright notice, 
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//  * Neither the name of the http://www.xappsoftware.com web site nor the names
//    of its contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY http://www.xappsoftware.com AND CONTRIBUTORS
//  “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
//  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
//  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#define P_SIZE 1024
#define M_SIZE 256

void bfInterpreter(void)
{
  char        program[P_SIZE];
  char        memory[M_SIZE];
  int         programSize;
  int         programPointer;
  int         memoryPointer;
  int         kl;

  programSize    =  0;
  memoryPointer  =  0;
  programPointer =  0;
  kl             =  0;
  
  memset(program, 0, sizeof(char)*P_SIZE);
  memset(memory, 0, sizeof(char)*M_SIZE);
  // Reading the program
  while(1)
  {
    if(Serial.available()>0)
    {
      program[programSize] = Serial.read();
      programSize++;
      if(programSize==P_SIZE-1)
      {
        break;
      }
      if(strstr(program, "<BFEND>")!=NULL)
      {
        break;
      }
    }
  }
  Serial.println(program);
  
  for(programPointer=0; programPointer<programSize; programPointer++)
  {
    switch(program[programPointer])
    {
      case '+':
        memory[memoryPointer]++;
        break;
      case '-':
        memory[memoryPointer]--;
        break;
      case '>':
        memoryPointer++;
        break;
      case '<':
        memoryPointer--;
        break;
      case '.':
        Serial.print(memory[memoryPointer], BYTE);
        break;
      case ',':
        while(1)
        {
          if(Serial.available()>0)
          {
            memory[memoryPointer]= Serial.read();
          }
          {
            delay(50);
          }
        }
        break;
      case '[':
	if (memory[memoryPointer] == 0)
        {
	  programPointer++;
	  while (kl > 0 || program[programPointer] != ']')
	  {
	    if (program[programPointer] == '[')
	      kl++;
	    if (program[programPointer] == ']')
	      kl--;
	    programPointer++;
	  }
	}
        break;
      case ']':
        if (memory[memoryPointer] != 0)
        {
          programPointer--;
          while (kl > 0 || program[programPointer] != '[')
          {
            if (program[programPointer] == '[')
              kl--;
	    if (program[programPointer] == ']')
              kl++;
            programPointer--;
          }
          programPointer--;
        }
        break;
      default:
        break;
    }
  }
  
  Serial.println("Waiting for a new program......");
  programSize=0;
}


void setup()
{
  Serial.begin(115200);
}

void loop()
{
  bfInterpreter();
}
