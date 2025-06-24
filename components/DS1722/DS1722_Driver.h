#include <cstdint>

class DS1722_Driver
{
public:
	void init();
	float measure(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len); //Измерение температуры
	float get_temp();
};

extern DS1722_Driver Temp;

