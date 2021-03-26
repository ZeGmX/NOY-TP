/*! \file pagefaultmanager.cc
Routines for the page fault managerPage Fault Manager
*/
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.
//  See copyright_insa.h for copyright notice and limitation
//  of liability and disclaimer of warranty provisions.
//

#include "kernel/thread.h"
#include "vm/swapManager.h"
#include "vm/physMem.h"
#include "vm/pagefaultmanager.h"

PageFaultManager::PageFaultManager()
{
}

// PageFaultManager::~PageFaultManager()
/*! Nothing for now
*/
PageFaultManager::~PageFaultManager()
{
}

// ExceptionType PageFault(uint32_t virtualPage)
/*! 	
//	This method is called by the Memory Management Unit when there is a 
//      page fault. This method loads the page from :
//      - read-only sections (text,rodata) $\Rightarrow$ executive
//        file
//      - read/write sections (data,...) $\Rightarrow$ executive
//        file (1st time only), or swap file
//      - anonymous mappings (stack/bss) $\Rightarrow$ new
//        page from the MemoryManager (1st time only), or swap file
//
//	\param virtualPage the virtual page subject to the page fault
//	  (supposed to be between 0 and the
//        size of the address space, and supposed to correspond to a
//        page mapped to something [code/data/bss/...])
//	\return the exception (generally the NO_EXCEPTION constant)
*/
#define ETUDIANTS_TP //TODO: remove this line
#ifndef ETUDIANTS_TP
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage)
{
  printf("**** Warning: page fault manager is not implemented yet\n");
  exit(-1);
  return ((ExceptionType)0);
}
#endif
#ifdef ETUDIANTS_TP
// TODO: deal with read/write rights
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage)
{
  printf("IN THE PAGE FAULT ROUTINE\n");
  // virtual page table
  TranslationTable* tpv = g_current_thread->GetProcessOwner()->addrspace->translationTable;
  int numPage = tpv->getAddrDisk(virtualPage);
  printf("swap: %d, addrdisk: %d\n", tpv->getBitSwap(virtualPage), numPage);

  // Checking the swap bit to know the content
  if (tpv->getBitSwap(virtualPage) == 1) {  // The page is in swap

    // page being used by the replacement algorithm
    while (numPage == -1) numPage = tpv->getAddrDisk(virtualPage);

    // Loading the page into a temporary location
    char tempPage[g_cfg->PageSize];
    g_swap_manager->GetPageSwap(numPage, tempPage);

    int numPage_phys = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, numPage);
    int* phys_addr = ((int*)g_machine->mainMemory) + numPage_phys * g_cfg->PageSize;
    memcpy(phys_addr, tempPage, g_cfg->PageSize);

    g_physical_mem_manager->UnlockPage(numPage_phys);
  }

  else if (numPage == -1) {  // the page doesn't exist in memory, anonymous page
    // Loading the page into a temporary location
    char tempPage[g_cfg->PageSize];
    memset(tempPage, 0, g_cfg->PageSize); // filling the page with 0

    int numPage_phys = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, numPage);
    int* phys_addr = ((int*)g_machine->mainMemory) + numPage_phys * g_cfg->PageSize;
    memcpy(phys_addr, tempPage, g_cfg->PageSize);

    g_physical_mem_manager->UnlockPage(numPage_phys);
  }

  else { // load the page in the executable
    // Loading the page into a temporary location
    char tempPage[g_cfg->PageSize];
    g_current_thread->GetProcessOwner()->exec_file->ReadAt(tempPage, g_cfg->PageSize, numPage);

    int numPage_phys = g_physical_mem_manager->AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, numPage);
    int* phys_addr = ((int*)g_machine->mainMemory) + numPage_phys * g_cfg->PageSize;
    memcpy(phys_addr, tempPage, g_cfg->PageSize);

    g_physical_mem_manager->UnlockPage(numPage_phys);
  }
  tpv->setBitValid(virtualPage);
  printf("end routine\n");
  return NO_EXCEPTION;
}
#endif


