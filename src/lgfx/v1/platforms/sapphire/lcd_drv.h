// 1.54インチ 240×240ドットカラーグラフィックTFT LCDモジュール
// https://akizukidenshi.com/catalog/g/g131019/
// Controller : ST7789

#include <stdint.h>
#include "soc.h"
#include "bsp.h"
#include "spi.h"
#include "dmasg.h"

#define LCD_GPIO_BIT_DC      0
#define LCD_GPIO_BIT_BLK     6
#define LCD_GPIO_BIT_RES     7

class LCD_CTRL {
public:
    LCD_CTRL();
    void lcdInit();
    void setAddress(uint16_t xStart = 0, uint16_t yStart  = 0, uint16_t xEnd = 239, uint16_t yEnd = 239);
    void sendCommand(uint8_t cmd);
    void sendData8(uint8_t data);
    void sendData16(uint16_t data);
    void sendRptData(uint32_t lenByte, uint8_t size, uint32_t data);
    void sendDataWithDMA(uint16_t *data, uint32_t size);
    bool dmaBusy();
    bool fifoBusy();
private:
    void lcdReset();
    const uint32_t GPIO_LCD = SYSTEM_GPIO_1_IO_CTRL;
    const uint32_t SPI_LCD = SYSTEM_SPI_1_IO_CTRL;
    const uint32_t SPI_CS  = 0;
};

#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_RDDPM      0x0A
#define ST77XX_RDDMADCTL  0x0B
#define ST77XX_RDDCOLMOD  0x0C
#define ST77XX_RDDIM      0x0D
#define ST77XX_RDDSM      0x0E
#define ST77XX_RDDSDR     0x0F

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_GAMSET     0x26
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_VSCRDEF    0x33
#define ST77XX_TEOFF      0x34
#define ST77XX_TEON       0x35
#define ST77XX_MADCTL     0x36
#define ST77XX_COLMOD     0x3A
#define ST77XX_VSCRSADD   0x37
#define ST77XX_IDMOFF     0x38
#define ST77XX_IDMON      0x39
#define ST77XX_COLMOD     0x3A
#define ST77XX_RAMWRC     0x3C
#define ST77XX_RAMRDC     0x3E
#define ST77XX_RDESCAN    0x45
#define ST77XX_WRDISBV    0x51
#define ST77XX_RDDISBV    0x52
#define ST77XX_WRCTRLD    0x53
#define ST77XX_RDCTRLD    0x54
#define ST77XX_WRCACE     0x55
#define ST77XX_RDCABC     0x56
#define ST77XX_WRCABCMB   0x5E
#define ST77XX_RDCABCMB   0x5F
#define ST77XX_RDABCSDR   0x68

// #define ST77XX_MADCTL_MY  0x80
// #define ST77XX_MADCTL_MX  0x40
// #define ST77XX_MADCTL_MV  0x20
// #define ST77XX_MADCTL_ML  0x10
// #define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
// #define ST77XX_RDID4      0xDD

// Some ready-made 16-bit ('565') color settings:
#define ST77XX_BLACK      0x0000
#define ST77XX_WHITE      0xFFFF
#define ST77XX_RED        0xF800
#define ST77XX_GREEN      0x07E0
#define ST77XX_BLUE       0x001F
#define ST77XX_CYAN       0x07FF
#define ST77XX_MAGENTA    0xF81F
#define ST77XX_YELLOW     0xFFE0
#define ST77XX_ORANGE     0xFC00
