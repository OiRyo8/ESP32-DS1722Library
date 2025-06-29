#include "driver/spi_master.h"
#include "esp_log.h"
#include "DS1722_Driver.h"

#define TAG "DS1722"

extern "C"
void app_main()
{	
	uint8_t tx_conf[2];
	uint8_t rx_conf[2];
	tx_conf[0] = 0x00;
		
	Temp.init(12,1);
	
	while (1)
	{	
		ESP_LOGI(TAG, "Config: %.2fC", Temp.read(tx_conf, rx_conf, 16));
		ESP_LOGI(TAG, "Temp: %.2fC", Temp.get_tempSD());
		vTaskDelay(10);
	}
}
