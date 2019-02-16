#include <stdio.h>	// IO functions
#include <stdlib.h> 	// std lib function
#include <unistd.h>	// close function
#include "memory.h"	// our code that represents physical memory
#include "cache.h"	// our code that represents cache


/******************************************************************************

HW1: CSCI 340 Operating Systems - Spring 2019

Develop a cache simulator that implements the two cache mapping functions (CMF):
1) direct mapping (DM)
2) set associative (SA) with set size of 2

This will be an interactive simulator that allows the user to:
1) Select the CMF
2) Enter hex memory address locations
3) For the CMF selected in (1) determine if it's a cache-hit or -miss (and display to the user)
4) if a cache-miss, determine the cache line to be replaced for the choosen CMF.
   Note: for SA CMF, the replacement algorithm is least recently used (LRU) that is based on 
	 minimum number of cache hits.

*******************************************************************************/

#define RUNFOEVA 1

char* CMFS[] = { "Direct Mapping", "Set Associative" };
char* OP[] = { "MISS", "HIT" };
char* ROP[] = { "No Replacement", "Replacement" };

int main(int argc, char *argv[])
{

  unsigned int num_blocks;
  unsigned int cmf;
  unsigned int hex_addr;
	
  int byte;
  unsigned int found, replace;

  addr_bits = readMemoryFile(argv[1]);

  if (addr_bits != READ_ERROR) {

    /*
      ---------------------------------------------------
      STEP 1:
      ---------------------------------------------------
      Calculate the number of blocks then display to the
      user the number of physical memory addressable bits 
      and the total number of physical memory blocks.
    */

    printf("\n------------------------\n");
    printf("[STEP 1] Setting up physical memory\n");
    printf("------------------------\n");
		
    num_blocks = numberOfBlocks(addr_bits, NUM_BLOCK_OFFSET_BITS);

    printf("Physical memory addressable bits = %d, total number of blocks = %d\n", addr_bits, num_blocks);

    /*
      ---------------------------------------------------
      STEP 2:
      ---------------------------------------------------
      Initialize the block points, i.e. determine the physical
      memory starting address for each block.
      
    */

    printf("\n------------------------\n");
    printf("[STEP 2] Determining physical memory block locations\n");
    printf("------------------------\n");

    initialzeBlockPointers(num_blocks, NUM_BLOCK_OFFSET_BITS);

    /*
      ---------------------------------------------------
      STEP 3:
      ---------------------------------------------------
      Ask the user to choose the cache mapping function (CMF)
      
    */

    printf("\n------------------------\n");
    printf("[STEP 3] Select cache mapping function (CMF)\n");
    printf("------------------------\n");
    printf("1 = Direct mapping\n");
    printf("2 = Set associative\n");
    printf("------------------------\n");
    printf("Please enter 1 or 2: ");

    scanf("%d", &cmf);

    if (cmf != DM && cmf != SA) 
      printf("You're kill'n me ... unknown choice (%d) ... exiting\n", cmf);
    else {

      /*
	---------------------------------------------------
	STEP 4:
	---------------------------------------------------
	Initialize cache, that is, all the cache lines should 
	be empty (i.e. no block of physical memory is loaded 
	into any cache line).
	
      */

      printf("\n------------------------\n");
      printf("[STEP 4] initializing cache\n");
      printf("------------------------\n");

      initializeCache(NUM_OF_LINES);
      
      /*
	---------------------------------------------------
	STEP 5:
	---------------------------------------------------
	Ask the user to enter physical a memory address location
	in hexidecimal format (e.g. SA), then get and display
	physical memory value from cache and display to user.
	
      */

      printf("\n------------------------\n");
      printf("[STEP 5] Starting simulation\n");
      printf("------------------------\n");
      printf("CMF is %s\n", CMFS[cmf-1]);
      printf("To exit simulation press the 'Ctrl C' keys on your keyboard\n");

      
      while (RUNFOEVA) {

	printf("\nPlease enter %d-bit hexidecimal address: ", addr_bits);
	scanf("%x", &hex_addr);

	printf("Entered Hexidecimal (Base-16) address %02X (Base-10 value %d)\n", hex_addr, hex_addr);
	
	byte = cread(cmf, &hex_addr, &found, &replace);
	
	if (byte != FAIL) {

	  printf("[%s:%s] The byte value at memory address %02X is %02X\n",
		 OP[found], ROP[replace], hex_addr, byte);

	} else {

	  printf("Failed to read cache for memory location %02X\n", hex_addr);

	}
	
      }
      
    }

  } else {

    printf("Unable to read the memory file (%s) ... exiting\n", argv[1]);

  }

  return OK;
     
} // end main function

