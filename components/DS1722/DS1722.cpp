#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "DS1722_Driver.h"

//Pins:
short MOSI = 13;
short MISO = 12;
short sclk = 14;

#define CS GPIO_NUM_16
spi_device_handle_t spi;

DS1722_Driver Temp;

//Global resolution and Shutdown mode values
uint8_t res;
short res_int;
uint8_t SD;


void DS1722_Driver::init(int resolution, bool SD_mode)
{

	//SPI configuration
	spi_bus_config_t buscfg = {
		.mosi_io_num = MOSI,
		.miso_io_num = MISO,
		.sclk_io_num = sclk,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 32,
	};

	//DS1722 configuration 
	spi_device_interface_config_t devcfg = {
		.mode = 3,
		.clock_speed_hz = 1 * 1000 * 1000,
		//CS pin didn't work automaticly, you can change that if you want
		//.spics_io_num = 16,
		// CS pin
		.queue_size = 1,
	};

	//SPI intialisation
	ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, 0));
	ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));
	
	//CS pin initialisation
	gpio_reset_pin(CS);
	gpio_set_direction(CS, GPIO_MODE_OUTPUT);
	
	//Resolution select
	res_int = resolution;
	switch (resolution)
	{
	case 8:
		res = 0x00;
	case 9:
		res = 0x02;
	case 10: 
		res = 0x04;
	case 11: 
		res = 0x06;
	case 12:
		res = 0x08;
	default:
		res = 0x08;
	}
	
	//Shutdown mode
	if (SD_mode == true)
	{
		SD = 0x01;
	}
	else
	{
		SD = 0x00;
	}	
	
	//"Making" of the config byte
	uint8_t conf_byte = 0xE0 + 0 + res + SD;
	
	//Config write
	Temp.write_config(conf_byte, 8*2);
}

void DS1722_Driver::write_config(uint8_t conf_byte, uint16_t len) {
	
	uint8_t tx_buf[2];
	uint8_t rx_buf[2];
	
	//Configuration byte command
	tx_buf[0] = 0x80;
	tx_buf[1] = conf_byte;
	
	//SPI transaction settings
	spi_transaction_t t = {
		.length = len,
		.tx_buffer = tx_buf,
		.rx_buffer = rx_buf,
	};

	//SPI transaction
	gpio_set_level(CS, 1);
	ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
	gpio_set_level(CS, 0);
}

float DS1722_Driver::read(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len)
{
	//SPI transaction settings
	spi_transaction_t t = {
		.length = len,
   		.tx_buffer = tx_buf,
		.rx_buffer = rx_buf,
	};
 
	//SPI transaction and reading
	gpio_set_level(CS, 1);
	ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
	gpio_set_level(CS, 0);
	
	return rx_buf[1];  
}



uint8_t tx_buf[2];
uint8_t rx_buf[2];
uint8_t temp_buf[2];

float DS1722_Driver::get_temp()
{

	//Reading 2 temperature bytes
	tx_buf[0] = 0x01; 
	tx_buf[1] = 0x00;
	temp_buf[0] = Temp.read(tx_buf, rx_buf, 8 * sizeof(tx_buf));
		
	tx_buf[0] = 0x02;
	tx_buf[1] = 0x00;
	temp_buf[1] = Temp.read(tx_buf, rx_buf, 8 * sizeof(tx_buf));
	
	
	float temp;
	
	//Transformation selection depending on resolution
	switch (res_int)
	{
	case 8:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8));
	case 9:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.5;
	case 10: 
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.25;
	case 11: 
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.125;
	case 12:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.0625;
	default:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.0625;
	}
	return temp;
}


//Shutdown mode function
float DS1722_Driver::get_tempSD()
{
	
	//Transaction of 1SHOT reading in SD mode
	uint8_t conf_byte = 0xE0 + 0x10 + res + SD;
	Temp.write_config(conf_byte, 8 * 2);
	
	//Reading 2 temperature bytes
	tx_buf[0] = 0x01; 
	tx_buf[1] = 0x00;
	temp_buf[0] = Temp.read(tx_buf, rx_buf, 8 * sizeof(tx_buf));
		
	tx_buf[0] = 0x02;
	tx_buf[1] = 0x00;
	temp_buf[1] = Temp.read(tx_buf, rx_buf, 8 * sizeof(tx_buf));
		
	
	float temp;
	
	//Transformation selection depending on resolution
	switch (res_int)
	{
	case 8:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8));
	case 9:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.5;
	case 10: 
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.25;
	case 11: 
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.125;
	case 12:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.0625;
		//temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) / 0x100;
	default:
		temp = 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) * 0.0625;
	}
	return temp;
}