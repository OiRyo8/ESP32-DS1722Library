#include <cstdint>

class DS1722_Driver
{
public:
	void init(int resolution, bool SD_mode);
	float read(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len); //Измерение температуры
	void write_config(uint8_t conf_byte, uint16_t len);
	float get_temp();
	float get_tempSD();
	uint8_t config_byte(int resolution, bool SD_mode);
};

extern DS1722_Driver Temp;

