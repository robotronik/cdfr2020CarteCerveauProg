//from the libopencm3 example: https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32-h107/flash_rw_example/flash_rw_example.c

#include "flash.h"

static uint32_t flash_program_data(uint32_t start_address, uint8_t *input_data, uint16_t num_elements)
{
	uint16_t iter;
	uint32_t current_address = start_address;
	uint32_t sector_address = start_address;
	uint32_t flash_status = 0;

	/*check if start_address is in proper range*/
	if((start_address - FLASH_OPERATION_ADDRESS) >= (FLASH_SECTOR_SIZE * (FLASH_SECTOR_NUM_MAX+1)))
		return 1;

	/*calculate current page address*/
	if(start_address % FLASH_SECTOR_SIZE)
		sector_address -= (start_address % FLASH_SECTOR_SIZE);

    /*check data does not exceed one sector wide*/
    if((start_address-sector_address)+num_elements>FLASH_SECTOR_SIZE){
        return -1;
    }

	flash_unlock();

	/*Erasing page*/
	flash_erase_sector(sector_address);
	flash_status = flash_get_status_flags();
	if(flash_status != FLASH_SR_EOP)
		return flash_status;

	/*programming flash memory*/
	for(iter=0; iter<num_elements; iter++)
	{
		/*programming word data*/
		flash_program_byte(current_address+iter, *(input_data + iter));
		flash_status = flash_get_status_flags();
		if(flash_status != FLASH_SR_EOP)
			return flash_status;

		/*verify if correct data is programmed*/
		if(*(uint8_t*)(current_address+iter) != *(input_data + iter))
			return -1;
	}

	return 0;
}

static void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *output_data)
{
	uint16_t iter;
	uint32_t *memory_ptr= (uint32_t*)start_address;

	for(iter=0; iter<num_elements; iter++)
	{
		*(uint8_t*)output_data = *(memory_ptr + iter);
		output_data += 4;
	}
}
