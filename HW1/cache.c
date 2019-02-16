// -----------------------------------
// CSCI 340 - Operating Systems
// Spring 2019
// cache.c file
// 
// Homework 1
// Hannah Carl (Former: Posch)
// -----------------------------------

#include "cache.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int initializeCache(unsigned int number_of_lines)
{
  int line, retVal;

  retVal = OK;

  cache = malloc(sizeof(cache_line*) * number_of_lines);

  if (cache != NULL) {

    for (line=0; line<number_of_lines; line++) {

      cache[line] = (cache_line*) malloc(sizeof(cache_line));
      cache[line]->tag = UNK;
      cache[line]->hit_count = ZERO;
      
    }
    
  } else
    retVal = FAIL;

  return retVal;

} // end initializeCache function


int cread(unsigned int cmf, unsigned int* hex_addr, unsigned int* found,
	  unsigned int* replace)
{
	//Direct Mapping Variables
	int shiftTagDM = addr_bits - NUM_OF_TAG_BITS_DM;
	//int modTagDM = (int) pow (2, (addr_bits-NUM_OF_LINE_BITS-NUM_BLOCK_OFFSET_BITS)); 
	int modLineDM = (int) pow (2, (addr_bits-NUM_OF_TAG_BITS_DM-NUM_BLOCK_OFFSET_BITS)); 
	int blockOffsetShift = (int) pow (2, (NUM_BLOCK_OFFSET_BITS));

	int tagDM = (((*hex_addr)>>shiftTagDM));
	int lineDM = (((*hex_addr)>>NUM_BLOCK_OFFSET_BITS)%modLineDM);
	int blockOffsetDM = (*hex_addr)%blockOffsetShift;
	int blockNumDM = (*hex_addr)>>NUM_BLOCK_OFFSET_BITS;
	
	//Set Associative Variables
	int numOfSetBits = addr_bits-NUM_OF_TAG_BITS_SA-NUM_BLOCK_OFFSET_BITS;
	int numOfLinesPerSet = addr_bits/((int) pow (2, (numOfSetBits)));
	int shiftTagSA = addr_bits - NUM_OF_TAG_BITS_SA;
	int modSetSA = (int) pow (2, (numOfSetBits));
	//int modTagSA = (int) pow (2, (addr_bits-NUM_OF_TAG_BITS_SA));	
	

    	int setSA = ((*hex_addr)>>NUM_BLOCK_OFFSET_BITS)%modSetSA;
	int tagSA = ((*hex_addr)>>shiftTagSA);
    	int lineToCheck;
	int smallestHitCount = cache[0]->hit_count;
	int locOfSmallHitCount = 0;
	int byte = phy_memory[block_location[blockNumDM]+blockOffsetDM];

	//printf("tag = %d\n line = %d\n  blockOffset= %d\n   blockNum = %d\n  tagSA = %d\n setNum = %d\n",  tagDM, lineDM, blockOffsetDM, blockNumDM, tagSA, setSA);

	//Reset found and replace
	*found = 0;
	*replace = 0;
  
	//Direct Mapping
	if(cmf == 1){
		
		if((cache[lineDM]->tag) == (tagDM)){
			*found = 1;
			cache[lineDM]->hit_count +=1;
			*replace = 0;
		}
		else{
			*found =0;
			*replace = 1;
			cache[lineDM]->tag = (tagDM);
			cache[lineDM]->hit_count = 0;
		}

	}
	
	//2 Way Set Associative
	else{
		
		lineToCheck = numOfLinesPerSet*setSA;
        	for(int i=0; i<numOfLinesPerSet; i++){
			if(*found == 0){
				if(cache[lineToCheck+i]->tag == tagSA){
			    		*found = 1;
			    		*replace = 0;
			    		cache[lineToCheck+i]->hit_count +=1;
				}
			}

		}
            
		
        
		
		if(*found == 0){
			smallestHitCount = cache[lineToCheck]->hit_count;
			locOfSmallHitCount = lineToCheck;
			for(int i=1; i<numOfLinesPerSet; i++){
				if(*found == 0){
					if(smallestHitCount > (cache[lineToCheck+i]->hit_count)){
						smallestHitCount = cache[lineToCheck+i]->hit_count;
						locOfSmallHitCount = lineToCheck+i;
					}
				}

			}
			
			cache[locOfSmallHitCount]->tag = (tagSA);
			cache[locOfSmallHitCount]->hit_count =0;
			*found =0;
			*replace = 1;
			
		}

	}
	//cprint();


  return byte;

} // end cread function



void cprint()
{
  unsigned int line;

  printf("\n---------------------------------------------\n");
  printf("line\ttag\tnum of hits\n");
  printf("---------------------------------------------\n");

  for (line=0; line<NUM_OF_LINES; line++) { 

    if (cache[line]->tag == UNK) {
			
      printf("%d\t%d\t%d\n", line, cache[line]->tag, cache[line]->hit_count);

    } else {

      printf("%d\t%02X\t%d\n", line, cache[line]->tag, cache[line]->hit_count);

    }

  }

} // end cprint function
