#if defined (SAPPHIRE)

#include "common.hpp"

  namespace spi
  {
    
    // static LCD_CTRL lcd;

    // cpp::result<void, error_t> init(int sercom_index, int pin_sclk, int pin_miso, int pin_mosi)
    // {
    //   return {};
    // }

    //void release(int spi_host) {}
    void release(int sercom_index)
    {
    }

    void beginTransaction(int sercom_index, uint32_t freq, int spi_mode)
    {
    }

    void endTransaction(int sercom_index)
    {
    }

    void writeBytes(int spi_host, const uint8_t* data, size_t len)
    {
        // if (len < 16) {
        //     for (int i=0; i<(int)len; i++) {
        //        lcd.sendData8(data[i]); 
        //     }
        // } else {
        //     lcd.sendDataWithDMA(data, len);
        // }
    }

    void readBytes(int sercom_index, uint8_t* data, size_t length)
    {
    }
  }

#endif
