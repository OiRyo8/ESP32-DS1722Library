#include "driver/spi_master.h"
#include "esp_log.h"
#include "DS1722_Driver.h"

#define TAG "DS1722"

extern "C"
void app_main()
{	
	//For reading config manually
//	uint8_t tx_conf[2];
//	uint8_t rx_conf[2];
//	tx_conf[0] = 0x00;
		
	//Initialisation, 12 - resolution, 1 - shutdown mode
	Temp.init(12,1);
	
	while (1)
	{	
		//For reading config manually
//		ESP_LOGI(TAG, "Config: %.2fC", Temp.read(tx_conf, rx_conf, 16));
		
		//Getting temperature is SD mode
		ESP_LOGI(TAG, "Temp: %.2fC", Temp.get_tempSD());
		vTaskDelay(10);
	}
}
