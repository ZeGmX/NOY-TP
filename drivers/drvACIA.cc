/* \file drvACIA.cc
   \brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return 
//      immediately, and an interrupt happens later on).  
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.  
//  See copyright_insa.h for copyright notice and limitation 
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h" // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
  Initialize the ACIA driver. 
  In the ACIA Interrupt mode, 
  initialize the reception index and semaphores and allow 
  reception interrupts. 
  In the ACIA Busy Waiting mode, simply inittialize the ACIA 
  working mode and create the semaphore.
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
DriverACIA::DriverACIA()
{
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
}
#endif
#ifdef ETUDIANTS_TP
DriverACIA::DriverACIA()
{
  switch(g_cfg->ACIA) {  // Actions depending on the ACIA mode
  case ACIA_BUSY_WAITING: {
    DEBUG('d', (char*)"Creating the ACIA driver with mode BusyWaiting\n");

    g_machine->acia->SetWorkingMode(BUSY_WAITING);

    // Mutex to protect the emission/reception functions
    send_sema = new Semaphore((char*)"DriverAcia_send_sema", 1);
    receive_sema = new Semaphore((char*)"DriverAcia_receive_sema", 1);
    break;
  }
  case ACIA_INTERRUPT: {
    DEBUG('d', (char*)"Creating the ACIA driver with mode Interrupt\n");

    // At first we accept both interruptions
    g_machine->acia->SetWorkingMode(SEND_INTERRUPT | REC_INTERRUPT);

    // We can already send a string
    send_sema = new Semaphore((char*)"DriverAcia_send_sema", 1);
    // But we can't receive one until something has been sent
    receive_sema = new Semaphore((char*)"DriverAcia_receive_sema", 0);

    ind_rec = 0;
    ind_send = 0;
    break;
  }
  case ACIA_NONE: {
    break;
  }
  }
}
#endif


//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
int DriverACIA::TtySend(char *buff)
{
  printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
}
#endif
#ifdef ETUDIANTS_TP
int DriverACIA::TtySend(char *buff)
{
  DEBUG('d', (char*)"Call to TtySend\n");

  send_sema->P();

  switch (g_machine->acia->GetWorkingMode()) { // Actions depending on the current ACIA mode
  case BUSY_WAITING: {
    int i = -1;

    do {
      // We wait for the last character to be sent so that we can put a new one
      while (g_machine->acia->GetOutputStateReg() == FULL) {;}

      i++;
      g_machine->acia->PutChar(buff[i]);
    } while (buff[i] != '\0');

    send_sema->V();

    return i + 1;
  }
  case REC_INTERRUPT | SEND_INTERRUPT: {
    // We copy the buffer into send_buffer (to be used in the interruption routine
    for (int i = 0 ; i < strlen(buff) + 1 ; i++)
      send_buffer[i] = buff[i];

    ind_send = 0;
    // We put the first character, which will raise the exception and call the routine
    g_machine->acia->PutChar(buff[0]);
    ind_send = 1;

    return strlen(buff) + 1;
  }
  }
  return 0;
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA 
//  (Busy Waiting and Interrupt mode).
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
int DriverACIA::TtyReceive(char *buff, int lg)
{
  printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
}
#endif
#ifdef ETUDIANTS_TP
int DriverACIA::TtyReceive(char *buff, int lg)
{
  DEBUG('d', (char*)"Call to TtyReceive\n");

  receive_sema->P();

  switch (g_machine->acia->GetWorkingMode()) {  // Actions depending on the current ACIA mode
  case BUSY_WAITING: {
    int i = 0;

    do {
      // We wait for a character to be received
      while (g_machine->acia->GetInputStateReg() == EMPTY) {;}

      // we copy it into the result string
      buff[i] = g_machine->acia->GetChar();
      i++;
    } while (i < lg && buff[i - 1] != '\0');

      buff[i - 1] = '\0';

    receive_sema->V();

    return i - 1;
  }
  case SEND_INTERRUPT: {
    int bound = std::min(ind_rec + 1, lg);

    // We copy the received buffer into the result string
    for (int i = 0 ; i < bound ; i++)
      buff[i] = receive_buffer[i];

    buff[bound - 1] = '\0';
    ind_rec = 0;

    // Now that the buffer has been processed, we can receive a new string
    g_machine->acia->SetWorkingMode(REC_INTERRUPT | SEND_INTERRUPT);

    return bound;
  }
  }
  return 0;
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
  Used in the ACIA Interrupt mode only. 
  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
void DriverACIA::InterruptSend()
{
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
}
#endif
#ifdef ETUDIANTS_TP
void DriverACIA::InterruptSend()
{
   // Interruption routine, called when a character is put into the output buffer

  DEBUG('d', (char*)"Interruption routine - send\n");

  if (send_buffer[ind_send - 1] != '\0') {
    g_machine->acia->PutChar(send_buffer[ind_send]);
    ind_send++;
  }
  else { // Receiving the last character
    send_sema->V();
  }
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
  Used in the ACIA Interrupt mode only. Receives a character through the ACIA.
  Releases the receive_sema semaphore and disables reception 
  interrupts when the last character of the message is received 
  (character '\0').
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
void DriverACIA::InterruptReceive()
{
  printf("**** Warning: receive interrupt handler not implemented yet\n");
  exit(-1);
}
#endif
#ifdef ETUDIANTS_TP
void DriverACIA::InterruptReceive()
{
  DEBUG('d', (char*)"Interruption routine - receive\n");

  char character = g_machine->acia->GetChar();

  if (character == '\0') {  // Receiving the last character of the sring
    receive_buffer[ind_rec] = character;
    // Blocking the interrupions until TtyReceive is called
    g_machine->acia->SetWorkingMode(g_machine->acia->GetWorkingMode() ^ REC_INTERRUPT);
    receive_sema->V();
  }
  else {
    receive_buffer[ind_rec] = character;
    ind_rec++;
  }
}
#endif
