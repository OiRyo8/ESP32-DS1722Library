#include "driver/spi_master.h"
#include "esp_log.h"
#include "DS1722_Driver.h"

#define TAG "DS1722"

extern "C"
void app_main()
{
		
	uint8_t tx_buf[2];
	uint8_t rx_buf[2];
	
	Temp.init();
	
	//tx_buf[1] = 0xE8; 0xE0 + 0 + 0x08 + 0; 3 первых бита 111 + OneShot + разрешение + Shutdown mode
	
	Temp.config(tx_buf, rx_buf, 8 * sizeof(tx_buf));
	
	tx_buf[0] = 0x00;
	tx_buf[1] = 0x00;
	ESP_LOGI(TAG, "Config byte: %.2f", Temp.measure(tx_buf, rx_buf, 8 * sizeof(tx_buf)));
	
	while (1)
	{
		ESP_LOGI(TAG, "Temp: %.2fC", Temp.get_temp());
		vTaskDelay(10);
	}
}
