// //from the libopencm3 example: https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32-h107/flash_rw_example/flash_rw_example.c

#include "flash.h"

void setup_flash_rom(){
    //FIXME: impossible to set the register(CR with bit EOPIE) that should generate SR interrupt when operation are
    //over (the EOP bit on the SR reg)
    fprintf(stderr, "FLASH_CR=0x%x\n",FLASH_CR);
    FLASH_CR |= FLASH_CR_EOPIE;
    fprintf(stderr, "FLASH_CR=0x%x\n",FLASH_CR);
}

uint32_t flash_program_data(uint8_t sector, uint8_t *input_data, uint16_t num_elements)
{
    uint32_t SECTOR_TABLE_F4[] = {0x08000000, 0x08004000, 0x08008000, 0x0800C000, 
    0x08010000, 0x08020000, 0x8040000, 0x8060000};
	uint16_t iter;
	uint32_t current_address = SECTOR_TABLE_F4[sector];
	uint32_t sector_address = SECTOR_TABLE_F4[sector];
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
    /* fprintf(stderr, "flash unlocked\n"); */
	/*Erasing page*/
    /* fprintf(stderr, "FLASH_SR=%d, before erase sector\n", FLASH_SR); */
	flash_erase_sector(sector, FLASH_PROGRAM_SIZE);
    /* fprintf(stderr, "FLASH_SR=%d, before after erase sector\n", FLASH_SR); */
        // flash_get_status_flags();
        // this getter seems to not be in available for us 
        // instead we can read the status register (to check if it actually works)
        // other possibility, re implement get status flag like the version of stmF1
	//if(flash_status != FLASH_SR_EOP){
    //    fprintf(stderr, "SR bit=0x%x not set after erase sector\n", flash_status);
	//	return flash_status;
    //}

	/*programming flash memory*/
	for(iter=0; iter<num_elements; iter++)
	{
		/*programming word data*/
		flash_program_byte(current_address+iter, *(input_data + iter));
		/* if(flash_status != FLASH_SR_EOP) */
		/* 	return flash_status; */

		/*verify if correct data is programmed*/
        /* fprintf(stderr, "programmed data adress = %x\t programmed data = %x \n", (input_data+iter), *(input_data+iter)); */
		if(*(uint8_t*)(current_address+iter) != *(input_data + iter))
			return -1;
	}

	return 0;
}

void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *poutput_data)
{
	uint16_t iter;
	uint8_t *memory_ptr= (uint8_t*)start_address;
    /* fprintf(stderr, "start address=%x \n", memory_ptr); */

	for(iter=0; iter<num_elements; iter++)
	{
        /* fprintf(stderr, "iter=%x \t poutput_data=%x \t *poutputdata=%x\n",iter, poutput_data, *poutput_data ); */
		*poutput_data = *(memory_ptr + iter);
		poutput_data ++;
	}
    /* fprintf(stderr, "all data has been read\n"); */
}
