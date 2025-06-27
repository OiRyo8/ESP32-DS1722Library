#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "DS1722_Driver.h"

#define CS GPIO_NUM_16
spi_device_handle_t spi;

DS1722_Driver Temp;

void DS1722_Driver::init()
{
	// Конфигурация шины SPI (HSPI)
	spi_bus_config_t buscfg = {
		.mosi_io_num = 13,
		.miso_io_num = 12,
		.sclk_io_num = 14,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 32,
	};

	// Конфигурация устройства DS1722
	spi_device_interface_config_t devcfg = {
		.mode = 3,
		// SPI Mode 3
		.clock_speed_hz = 1 * 1000 * 1000,
		// 1 MHz (макс. 2.5 MHz для DS1722)
		//.spics_io_num = 16,
		// CS pin
		.queue_size = 1,
	};

	// Инициализация SPI
	ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, 0));
	ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));
	
	gpio_reset_pin(CS);
	gpio_set_direction(CS, GPIO_MODE_OUTPUT);
}

void DS1722_Driver::config(uint8_t *tx_buf, uint8_t* rx_buf, uint16_t len) {
	
	spi_transaction_t t = {
		.length = len,
		// 16 бит (2 байта)
		.tx_buffer = tx_buf,
		.rx_buffer = rx_buf,
	};

	// Отправка команды и чтение данных
	gpio_set_level(CS, 1);
	ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
	gpio_set_level(CS, 0);
}

float DS1722_Driver::read(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len)
{
	spi_transaction_t t = {
		.length = len,
		// 16 бит (2 байта)
   		.tx_buffer = tx_buf,
		.rx_buffer = rx_buf,
	};

	// Отправка команды и чтение данных
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
	tx_buf[0] = 0x01; 
	tx_buf[1] = 0x00;
	temp_buf[0] = Temp.read(tx_buf, rx_buf, 8 * sizeof(tx_buf));
		
	tx_buf[0] = 0x02;
	tx_buf[1] = 0x00;
	temp_buf[1] = Temp.read(tx_buf, rx_buf, 8 * sizeof(tx_buf));
		
	return 1.0f * (temp_buf[0] + (temp_buf[1] << 8)) / 0x100;
}