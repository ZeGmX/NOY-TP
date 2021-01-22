/*! \file msgerror.h

    \brief Defines the data structure to store error messages during

           a syscall process

  

    Copyright (c) 1999-2000 INSA de Rennes.

    All rights reserved.  

    See copyright_insa.h for copyright notice and limitation 

    of liability and disclaimer of warranty provisions.

*/



#ifndef MSGERROR_H

#define MSGERROR_H



// Forward declarations

class SyscallError;

class DriverConsole;



#define ERROR -1



//! List of error message numbers

//! (see their meaning in msgerror.cc)

enum

{

  NO_ERROR = 0,

  INC_ERROR,

  OPENFILE_ERROR,

  EXEC_FILE_FORMAT_ERROR,

  OUT_OF_MEMORY,



  OUT_OF_DISK,

  ALREADY_IN_DIRECTORY,

  INEXIST_FILE_ERROR,

  INEXIST_DIRECTORY_ERROR,

  NOSPACE_IN_DIRECTORY,

  NOT_A_FILE,

  NOT_A_DIRECTORY,

  DIRECTORY_NOT_EMPTY,

  INVALID_COUNTER,



  /* Invalid typeId fields: */

  INVALID_SEMAPHORE_ID,

  INVALID_LOCK_ID,

  INVALID_CONDITION_ID,

  INVALID_FILE_ID,

  INVALID_THREAD_ID,



  NO_ACIA,



  NUMMSGERROR /* Must always be last */

};





/*! \brief Defines a structure to store syscall error messages

//

//

//  When an error occurs during a system call, a negative value

//  is return to the user program and then an error message

//  can be printed using the system call PError(). This structure

//  contains the last error message with the corresponding context

//  string (eg. the name of an unknow file).

*/

class SyscallError {

 public:

  SyscallError();     // Initialize the structure

  ~SyscallError();    // De-allocate the structure

  

  void SetMsg(char *about,int num);   

                      //!< Set the current error message



  void PrintLastMsg(DriverConsole *cons,char *ch);

                      //!< Print the error message with a user defined

                      //!< string



  //! Get the error format string (with a %s) associated with error num

  const char * GetFormat(int num);

  

 private:

  int lastError;           //!< last error's ident

  char *errorAbout;        //!< context string

  char *msgs[NUMMSGERROR]; //!< The array of strings for the error messages

};



#endif // MSGERROR_H

