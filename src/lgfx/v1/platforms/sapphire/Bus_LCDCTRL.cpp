/*----------------------------------------------------------------------------/
  Lovyan GFX - Graphics library for embedded devices.

Original Source:
 https://github.com/lovyan03/LovyanGFX/

Licence:
 [FreeBSD](https://github.com/lovyan03/LovyanGFX/blob/master/license.txt)

Author:
 [lovyan03](https://twitter.com/lovyan03)

Contributors:
 [ciniml](https://github.com/ciniml)
 [mongonta0716](https://github.com/mongonta0716)
 [tobozo](https://github.com/tobozo)
/----------------------------------------------------------------------------*/
#if defined (SAPPHIRE)

#include "Bus_LCDCTRL.hpp"
#include "common.hpp"

#include "../../misc/pixelcopy.hpp"

#include "lcd_drv.h"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------
  static LCD_CTRL lcd;

  void Bus_LCDCTRL::config(const config_t& cfg)
  {
    _cfg = cfg;
  }

  bool Bus_LCDCTRL::init(void)
  {
    // _inited = spi::init(_cfg.spi_host, _cfg.pin_sclk, _cfg.pin_miso, _cfg.pin_mosi, dma_ch).has_value();

    return true;
  }

  static void gpio_reset(size_t pin)
  {
  }

  void Bus_LCDCTRL::release(void)
  {
  }

  void Bus_LCDCTRL::beginTransaction(void)
  {
    // if (_cfg.use_lock) spi::beginTransaction(_cfg.spi_host);
  }

  void Bus_LCDCTRL::endTransaction(void)
  {
    // if (_cfg.use_lock) spi::endTransaction(_cfg.spi_host);
  }

  void Bus_LCDCTRL::wait(void)
  {
  }

  bool Bus_LCDCTRL::busy(void) const
  {
    return false;
  }

  bool Bus_LCDCTRL::writeCommand(uint32_t data, uint_fast8_t bit_length)
  {
    auto bytes = bit_length >> 3;
    do
    {
      lcd.sendCommand((uint8_t)data);
      data >>= 8;
    } while (--bytes);
    // lcd.sendCommand((uint8_t)data);
    // if (bit_length >= 16) {
    //   lcd.sendCommand((uint8_t)(data >> 8));
    // }
    // if (bit_length >= 24) {
    //   lcd.sendCommand((uint8_t)(data >> 16));
    // }
    return true;
  }

  void Bus_LCDCTRL::writeData(uint32_t data, uint_fast8_t bit_length)
  {
    auto bytes = bit_length >> 3;
    do
    {
      lcd.sendData8((uint8_t)data);
      data >>= 8;
    } while (--bytes);
    // lcd.sendData8((uint8_t)data);
    // if (bit_length >= 16) {
    //   lcd.sendData8((uint8_t)(data >> 8));
    // }
    // if (bit_length >= 24) {
    //   lcd.sendData8((uint8_t)(data >> 16));
    // }
  }

  void Bus_LCDCTRL::writeDataRepeat(uint32_t data, uint_fast8_t bit_length, uint32_t count)
  {
    // 24bit bafedcba dcbafedc fedcbafe ...
    // 16bit dcbadcba ...
    //  8bit babababa ...
    uint32_t regbuf[3];
    regbuf[0] = data | data << bit_length;
    // make 12Bytes data.
    bool bits24 = (bit_length == 24);
    if (bits24) {
      regbuf[1] = regbuf[0] >> 8 | regbuf[0] << 16;
      regbuf[2] = regbuf[0] >>16 | regbuf[0] <<  8;
    } else { // 8 or 16bit
      if (bit_length == 8) { regbuf[0] |= regbuf[0] << 16; }
      regbuf[1] = regbuf[0];
      regbuf[2] = regbuf[0];
    }

    uint32_t size = (bit_length>>3) * count;
    // uint8_t* buf = (uint8_t*)malloc(size);
    // if (buf == nullptr) {
    //   return;
    // }
    // uint8_t* p = buf;
    // for (int i=0; i<size/3; i++) {
    //   memcpy(p, regbuf, 3);
    //   p += 3;
    // }
    // memcpy(p, regbuf, size%3);

    // lcd.sendDataWithDMA(buf, size);
    // free(buf);
    
    uint8_t* p = (uint8_t*)regbuf;
    uint32_t pp = 0;
    for (int i=0; i<size; i++) {
      lcd.sendData8(p[pp++]);
      if (pp >= 12) pp = 0;
    }
  }

  void Bus_LCDCTRL::writePixels(pixelcopy_t* param, uint32_t length)
  {
    const uint8_t bytes = param->dst_bits >> 3;
    uint32_t data;
    param->fp_copy((uint8_t*)&data, 0, 1, param);
    lcd.sendData8((uint8_t)data);
    uint32_t i = 0;
    while (++i != bytes)
    {
      lcd.sendData8(data >> (i << 3));
    }
    if (--length)
    {
      do
      {
        param->fp_copy((uint8_t*)&data, 0, 1, param);
        i = 0;
        do
        {
          uint32_t tmp = data >> (i << 3);
          lcd.sendData8(tmp);
        } while (++i != bytes);
      } while (--length);
    }
  }

  void Bus_LCDCTRL::writeBytes(const uint8_t* data, uint32_t length, bool dc, bool use_dma)
  {
    if (dc) {
      for (int i=0; i<length; i++) {
        lcd.sendData8(data[i]);
      }
    } else {
      for (int i=0; i<length; i++) {
        lcd.sendCommand(data[i]);
      }
    }
  }

  void Bus_LCDCTRL::addDMAQueue(const uint8_t* data, uint32_t length)
  {
    // とりあえず未対応時はwriteBytesしておけばおけ
    writeBytes(data, length, true, true);
  }

  void Bus_LCDCTRL::execDMAQueue(void)
  {
    // とりあえず未対応時はなにもしないでおけ
  }

  uint8_t* Bus_LCDCTRL::getDMABuffer(uint32_t length)
  {
    return nullptr;
  }

  void Bus_LCDCTRL::beginRead(uint_fast8_t dummy_bits)
  {
  }

  void Bus_LCDCTRL::beginRead(void)
  {
  }

  void Bus_LCDCTRL::endRead(void)
  {
  }

  uint32_t Bus_LCDCTRL::readData(uint_fast8_t bit_length)
  {
    return 0;
  }

  bool Bus_LCDCTRL::readBytes(uint8_t* dst, uint32_t length, bool use_dma)
  {
    return true;
  }

  void Bus_LCDCTRL::readPixels(void* dst, pixelcopy_t* param, uint32_t length)
  {
  }

  void Bus_LCDCTRL::_alloc_dmadesc(size_t len)
  {
    // if (_dmadesc) heap_caps_free(_dmadesc);
    // _dmadesc_size = len;
    // _dmadesc = (lldesc_t*)heap_caps_malloc(sizeof(lldesc_t) * len, MALLOC_CAP_DMA);
  }

  void Bus_LCDCTRL::_spi_dma_reset(void)
  {
  }

  void Bus_LCDCTRL::_setup_dma_desc_links(const uint8_t *data, int32_t len)
  {
  }

//----------------------------------------------------------------------------
 }
}

#endif
