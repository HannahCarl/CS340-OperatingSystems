// -----------------------------------
// CSCI 340 - Operating Systems
// Spring 2019
// memory.c header file
// 
// Homework 1
// Hannah Carl (Former: Posch)
// -----------------------------------

#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int readMemoryFile(char* file_name) {

  // -------------------------------------
  // Please do not modify this code
  // -------------------------------------

  int addr_cnt = 0;
  int addr_bits = 0;
  int r_flag;

  FILE* fhnd;

  fhnd = fopen(file_name, "r");

  if (fhnd != NULL) {

    phy_memory = malloc(sizeof(unsigned int)*MAX_SIZE);

    r_flag = fscanf(fhnd, "%x\n", &phy_memory[addr_cnt]);

    while (r_flag != EOF) {
		  
      addr_cnt++;

      r_flag = fscanf(fhnd, "%x\n", &phy_memory[addr_cnt]);

    }

    phy_memory[addr_cnt] = '\0';
        	
    
  } else addr_cnt = READ_ERROR;

  fclose(fhnd);

  if (MEM_DEBUG) {
    printf("Number of bytes read %d\n", addr_cnt);
  }

  if (addr_cnt != READ_ERROR)
    addr_bits = log2(addr_cnt);
  else
    addr_bits = addr_cnt;
  
  return addr_bits;

} // end readMemoryFile function



int numberOfBlocks(unsigned int addr_bits, unsigned int num_block_offset_bits)
{
 

	int numberOfBlocksValue;
	numberOfBlocksValue = (int) pow (2, (addr_bits-num_block_offset_bits));
  	

  
  return numberOfBlocksValue; 
} // end numberOfBlocks function



void initialzeBlockPointers(unsigned int num_of_blocks,
			     unsigned int num_block_offset_bits )
{

  int i;

  block_location = malloc(sizeof(unsigned int) * num_of_blocks);

  for (i=0; i<num_of_blocks; i++) {

    block_location[i] = i*exp2(num_block_offset_bits);

    if (MEM_DEBUG) {
      printf("starting block address (base-16) %02X (%d base-10)\n",
	     block_location[i], block_location[i]);
    }

  }
  
} // end initalizeBlockPointers function
