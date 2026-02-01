#include "lcd_drv.h"
#include <stdint.h>
#include "io.h"

#ifndef APB_DMAC_BASE
#include "bsp.h"
#define APB_DMAC_BASE   IO_APB_SLAVE_0_INPUT
#endif

#ifndef APB_LCD_BASE
#include "bsp.h"
#define APB_LCD_BASE    IO_APB_SLAVE_1_INPUT
#endif

LCD_CTRL::LCD_CTRL() {
}

void LCD_CTRL::lcdReset() {
    // gpio_setOutput(GPIO_LCD, 0);
    bsp_uDelay(100*1000);
    // gpio_setOutput(GPIO_LCD, (1<<LCD_GPIO_BIT_RES));
    bsp_uDelay(100*1000);
}

void LCD_CTRL::sendCommand(uint8_t cmd) {
    uint32_t dat = 0x000 | (uint32_t)(cmd&0xFF);
    write_u32(dat, APB_LCD_BASE + 0x800);
}

void LCD_CTRL::sendData8(uint8_t data) {
    uint32_t dat = 0x100 | (uint32_t)(data&0xFF);
    write_u32(dat, APB_LCD_BASE + 0x800);
}

void LCD_CTRL::sendData16(uint16_t dat) {
    sendData8(dat >> 8);    //上位8bit
    sendData8(dat);         //下位8bit
}

void LCD_CTRL::sendRptData(uint32_t lenByte, uint8_t size, uint32_t data) {
    while(fifoBusy());
    write_u32(lenByte, APB_LCD_BASE + 0x100);
    write_u32(size, APB_LCD_BASE + 0x104);
    write_u32(data, APB_LCD_BASE + 0x108);
    write_u32(1, APB_LCD_BASE + 0x10C);
}

void LCD_CTRL::sendDataWithDMA(uint16_t *data, uint32_t size) {
    dmasg_input_memory(APB_DMAC_BASE, 0, (uint32_t)data, 4);
    dmasg_output_stream(APB_DMAC_BASE, 0, 0, 0, 0, 0);
    dmasg_direct_start(APB_DMAC_BASE, 0, size, 0);
    // while(dmasg_busy(APB_DMAC_BASE, 0) != 0);
}

bool LCD_CTRL::dmaBusy() {
    return dmasg_busy(APB_DMAC_BASE, 0) != 0;
}

bool LCD_CTRL::fifoBusy() {
    return (read_u32(APB_LCD_BASE + 0x0000) & 0x00000001) == 0x00000001;
}

void LCD_CTRL::lcdInit() {
}

void LCD_CTRL::setAddress(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
    sendCommand(ST77XX_CASET); // Column address set
    sendData8(xStart >> 8);     // X start_H
    sendData8(xStart & 0xFF);   // X start_L
    sendData8(xEnd >> 8);       // X end_H
    sendData8(xEnd & 0xFF);     // X end_L

    sendCommand(ST77XX_RASET); // Row address set
    sendData8(yStart >> 8);     // Y start_H
    sendData8(yStart & 0xFF);   // Y start_L
    sendData8(yEnd >> 8);       // Y end_H
    sendData8(yEnd & 0xFF);     // Y end_L

    sendCommand(ST77XX_RAMWR); // Memory write
}

// static const uint8_t
//   generic_st7789[] =  {                // Init commands for 7789 screens
//     9,                                 //  9 commands in list:
//     ST77XX_SWRESET,   ST_CMD_DELAY,    //  1: Software reset, no args, w/delay
//     150,                               //     ~150 ms delay
//     ST77XX_SLPOUT ,   ST_CMD_DELAY,    //  2: Out of sleep mode, no args, w/delay
//     10,                                //      10 ms delay
//     ST77XX_COLMOD , 1+ST_CMD_DELAY,    //  3: Set color mode, 1 arg + delay:
//     0x55,                              //     16-bit color
//     10,                                //     10 ms delay
//     ST77XX_MADCTL , 1,                 //  4: Mem access ctrl (directions), 1 arg:
//     0x08,                              //     Row/col addr, bottom-top refresh
//     ST77XX_CASET  , 4,                 //  5: Column addr set, 4 args, no delay:
//     0x00,
//     0,                                 //     XSTART = 0
//     0,
//     240,                               //     XEND = 240
//     ST77XX_RASET  , 4,                 //  6: Row addr set, 4 args, no delay:
//     0x00,
//     0,                                 //     YSTART = 0
//     320>>8,
//     320&0xFF,                          //     YEND = 320
//     ST77XX_INVON  ,   ST_CMD_DELAY,    //  7: hack
//     10,
//     ST77XX_NORON  ,   ST_CMD_DELAY,    //  8: Normal display on, no args, w/delay
//     10,                                //     10 ms delay
//     ST77XX_DISPON ,   ST_CMD_DELAY,    //  9: Main screen turn on, no args, delay
//     10                                 //    10 ms delay
// };

// void ST7789::init(uint16_t width, uint16_t height) {
//     // commonInit(NULL);
//     if (width == 240 && height == 240) {
//         // 1.3", 1.54" displays (right justified)
//         _rowstart = (320 - height);
//         _rowstart2 = 0;
//         _colstart = _colstart2 = (240 - width);
//     } else if (width == 135 && height == 240) {
//         // 1.14" display (centered, with odd size)
//         _rowstart = _rowstart2 = (int)((320 - height) / 2);
//         // This is the only device currently supported device that has different
//         // values for _colstart & _colstart2. You must ensure that the extra
//         // pixel lands in _colstart and not in _colstart2
//         _colstart = (int)((240 - width + 1) / 2);
//         _colstart2 = (int)((240 - width) / 2);
//     } else {
//         // 1.47", 1.69, 1.9", 2.0" displays (centered)
//         _rowstart = _rowstart2 = (int)((320 - height) / 2);
//         _colstart = _colstart2 = (int)((240 - width) / 2);
//     }

//     windowWidth = width;
//     windowHeight = height;

//     displayInit(generic_st7789);
//     setRotation(0);
// }

// void ST7789::displayInit(const uint8_t *addr) {

//   uint8_t numCommands, cmd, numArgs;
//   uint16_t ms;

//   numCommands = (*addr++); // Number of commands to follow
//   while (numCommands--) {              // For each command...
//     cmd = (*addr++);       // Read command
//     numArgs = (*addr++);   // Number of args to follow
//     ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
//     numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
//     sendCommand(cmd, addr, numArgs);
//     addr += numArgs;

//     if (ms) {
//       ms = (*addr++); // Read post-command delay time (ms)
//       if (ms == 255)
//         ms = 500; // If 255, delay for 500 ms
//       bsp_uDelay(ms*1000);
//     }
//   }
// }

// void ST7789::setRotation(uint8_t m) {
//   uint8_t madctl = 0;

//   rotation = m & 3; // can't be higher than 3

//   switch (rotation) {
//   case 0:
//     madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
//     _xstart = _colstart;
//     _ystart = _rowstart;
//     _width = windowWidth;
//     _height = windowHeight;
//     break;
//   case 1:
//     madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
//     _xstart = _rowstart;
//     _ystart = _colstart2;
//     _height = windowWidth;
//     _width = windowHeight;
//     break;
//   case 2:
//     madctl = ST77XX_MADCTL_RGB;
//     _xstart = _colstart2;
//     _ystart = _rowstart2;
//     _width = windowWidth;
//     _height = windowHeight;
//     break;
//   case 3:
//     madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
//     _xstart = _rowstart2;
//     _ystart = _colstart;
//     _height = windowWidth;
//     _width = windowHeight;
//     break;
//   }

//   sendCommand(ST77XX_MADCTL, &madctl, 1);
// }
