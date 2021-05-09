// //from the libopencm3 example: https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32-h107/flash_rw_example/flash_rw_example.c

#include "flash.h"

uint32_t flash_program_data(uint8_t sector, uint8_t *input_data, uint16_t num_elements)
{
    uint32_t SECTOR_TABLE_F4[] = {0x08000000, 0x08004000, 0x08008000, 0x0800C000, 
    0x08010000, 0x08020000, 0x8040000, 0x8060000};
	uint16_t iter;
	uint32_t current_address = SECTOR_TABLE_F4[sector];
	uint32_t sector_address = SECTOR_TABLE_F4[sector];
	uint32_t flash_status = 0;
    // too many check because we work with sector but this structure is useful for pages
	/*check if start_address is in proper range*/
	if((sector_address - FLASH_OPERATION_ADDRESS) >= (FLASH_SECTOR_SIZE * (FLASH_SECTOR_NUM_MAX+1)))
		return 1;

	/*calculate current page address*/
	if(sector_address % FLASH_SECTOR_SIZE)
		sector_address -= (sector_address % FLASH_SECTOR_SIZE);

    /*check data does not exceed one sector wide*/
    if((sector_address-sector_address)+num_elements>FLASH_SECTOR_SIZE){
        return -1;
    }

	flash_unlock();
    fprintf(stderr, "flash unlocked\n");

	/*Erasing page*/
	flash_erase_sector(sector, FLASH_PROGRAM_SIZE);
	flash_status = FLASH_SR;
    fprintf(stderr, "flash_status=%d, before test with SR_EOP\n", flash_status);
        // flash_get_status_flags();
        // this getter seems to not be in available for us 
        // instead we can read the status register (to check if it actually works)
        // other possibility, re implement get status flag like the version of stmF1
	if(flash_status != FLASH_SR_EOP)
		return flash_status;

	/*programming flash memory*/
	for(iter=0; iter<num_elements; iter++)
	{
		/*programming word data*/
		flash_program_byte(current_address+iter, *(input_data + iter));
		flash_status = FLASH_SR;
		if(flash_status != FLASH_SR_EOP)
			return flash_status;

		/*verify if correct data is programmed*/
		if(*(uint8_t*)(current_address+iter) != *(input_data + iter))
			return -1;
	}

	return 0;
}

void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *output_data)
{
	uint16_t iter;
	uint32_t *memory_ptr= (uint8_t*)start_address;
    fprintf(stderr, )

	for(iter=0; iter<num_elements; iter++)
	{
		*output_data = *(memory_ptr + iter);
		output_data += 4;
	}
}
