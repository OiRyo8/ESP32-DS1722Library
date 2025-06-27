#include "driver/spi_master.h"
#include "esp_log.h"
#include "DS1722_Driver.h"

#define TAG "DS1722"

extern "C"
void app_main()
{
		
	uint8_t tx_buf_conf[2];
	uint8_t rx_buf_conf[2];
	
	Temp.init();
	
	
	while (1)
	{
		tx_buf_conf[0] = 0x80;
		tx_buf_conf[1] = 0xE8; // 0xE0 + 0 + 0x08 + 0; 3 первых бита 111 + OneShot + разрешение + Shutdown mode
	
		Temp.config(tx_buf_conf, rx_buf_conf, 8 * sizeof(tx_buf_conf));
	
		tx_buf_conf[0] = 0x00;
	
		ESP_LOGI(TAG, "Config byte: %.2f", Temp.read(tx_buf_conf, rx_buf_conf, 8 * sizeof(tx_buf_conf)));
		
		
		ESP_LOGI(TAG, "Temp: %.2fC", Temp.get_temp());
		vTaskDelay(10);
	}
}
