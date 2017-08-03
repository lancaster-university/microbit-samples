#ifndef MICROBIT_FLASH_H_
#define MICROBIT_FLASH_H_

#include <mbed.h>

#define PAGE_SIZE 1024
#define CODE_SEGMENT_START_ADDR  			0x00000000
#define CODE_SEGMENT_END_ADDR      			0x00200000
#define CODE_SEGMENT							1
#define RAM_SEGMENT_START_ADDR				0x20000000
#define RAM_SEGMENT_END_ADDR				0x20200000
#define RAM_SEGMENT							2
#define ERR_NULL_POINTER_ADDR                                0
#define ERR_CROSSOVER_SEGMENT_ADDRESS 	-1
#define ERR_NOT_RAM_OR_CODE					-2



class MicroBitFlash
{
    private:

    /**
      * Check if an erase is required to write to a region in flash memory.
      * This is determined if, for any byte:
      * ~O & N != 0x00
      * Where O=Original byte, N = New byte.
      *
      * @param source to write from
      * @param flash_addr to write to
      * @param len number of uint8_t to check.
      * @return non-zero if erase required, zero otherwise.
      */
    int need_erase(uint8_t* source, uint8_t* flash_addr, int len);
 
    public:
    /**
      * Default constructor.
      */
    MicroBitFlash();

    /**
      * Writes the given number of bytes to the address in flash specified.
      * Neither address nor buffer need be word-aligned.
      * @param address location in flash to write to.
      * @param buffer location in memory to write from. 
      * @param length number of bytes to burn
      * @param scratch_addr if specified, scratch page to use. Use default 
      *                     otherwise.
      * @return non-zero on sucess, zero on error.
      * 
      * Example:
      * @code
      * MicroBitFlash flash;
      * uint32_t word = 0x01;
      * flash.flash_write((uint8_t*)0x38000, &word, sizeof(word))
      * @endcode
      */
    int flash_write(void* address, void* buffer, int length, 
                    void* scratch_addr = NULL);

    /**
      * Erase an entire page.
      * @param page_address address of first word of page.
      */
    void erase_page(uint32_t* page_address);

    /**
      * Write to flash memory, assuming that a write is valid
      * (using need_erase).
      * 
      * @param page_address address of memory to write to. 
      * 	Must be word aligned.
      * @param buffer address to write from, must be word-aligned.
      * @param len number of uint32_t words to write.
      */
    void flash_burn(uint32_t* page_address, uint32_t* buffer, int len);

    /**
      * Checks that the address given lies within either the "RAM" segment
      * or the "Code" segment of memory.
      * @param address location in flash it's attempting to write to.
      * @param length number of bytes it's attempting to burn
      * @return > 0 on sucess, < 1 on error.
      * 
      * Example:
      * @code
      * if(checkAddressType(address, sizeof(data)) == RAM_SEGMENT)
      *   printf("RAM Segment");
      * @endcode
      */
    int checkAddressType(uint32_t* address, int length); 

};

#endif
