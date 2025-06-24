#include "driver/spi_master.h"
#include "esp_log.h"
#include "DS1722_Driver.h"

#define TAG "DS1722"

void app_main()
{
	Temp.init();
	while (1)
	{
		ESP_LOGI(TAG, "Temp: %.2fC", Temp.get_temp());
		vTaskDelay(10);
	}
}
