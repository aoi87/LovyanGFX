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
#pragma once

#include <string.h>

#include "../../Bus.hpp"
#include "../common.hpp"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  class Bus_LCDCTRL : public IBus
  {
  public:
    struct config_t
    {
      uint8_t spi_mode = 0;
      uint8_t dma_channel = 0;
    };

    constexpr Bus_LCDCTRL(void) = default;

    const config_t& config(void) const { return _cfg; }

    void config(const config_t& config);

    bus_type_t busType(void) const override { return bus_type_t::bus_spi; }

    bool init(void) override;
    void release(void) override;

    void beginTransaction(void) override;
    void endTransaction(void) override;
    void wait(void) override;
    bool busy(void) const override;
    uint32_t getClock(void) const override { return 0; }
    void setClock(uint32_t freq) override {}
    uint32_t getReadClock(void) const override { return 0; }
    void setReadClock(uint32_t freq) override {}

    void flush(void) override {}
    bool writeCommand(uint32_t data, uint_fast8_t bit_length) override;
    void writeData(uint32_t data, uint_fast8_t bit_length) override;
    void writeDataRepeat(uint32_t data, uint_fast8_t bit_length, uint32_t count) override;
    void writePixels(pixelcopy_t* pc, uint32_t length) override;
    void writeBytes(const uint8_t* data, uint32_t length, bool dc, bool use_dma) override;

    void initDMA(void) override {}
    void addDMAQueue(const uint8_t* data, uint32_t length) override;
    void execDMAQueue(void) override;
    uint8_t* getDMABuffer(uint32_t length) override;

    void beginRead(uint_fast8_t dummy_bits) override;
    void beginRead(void) override;
    void endRead(void) override;
    uint32_t readData(uint_fast8_t bit_length) override;
    bool readBytes(uint8_t* dst, uint32_t length, bool use_dma) override;
    void readPixels(void* dst, pixelcopy_t* pc, uint32_t length) override;

  private:

    // void _alloc_dmadesc(size_t len);
    // void _spi_dma_reset(void);
    // void _setup_dma_desc_links(const uint8_t *data, int32_t len);

    config_t _cfg;
    FlipBuffer _flip_buffer;
    uint8_t _dma_ch = 0;
  };

//----------------------------------------------------------------------------
 }
}
