#include <cstdint>

class DS1722_Driver
{
	void init();
	float measure(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len); //��������� �����������
	float get_temp();
};
extern DS1722_Driver Temp;