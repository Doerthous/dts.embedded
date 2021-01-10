/*
   The MIT License (MIT)

   Copyright (c) 2020 Doerthous

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   Authour: Doerthous <doerthous@gmail.com>
*/

#ifndef ENC28J60_H_
#define ENC28J60_H_

#include <dts_hal_gpio.h>
#include <dts_hal_spi.h>
#include <stddef.h>

typedef struct
{
    dts_hal_gpio_t cs;
    dts_hal_spi_t *spi;
    uint8_t mac_addr[6];
    uint8_t half_mode:1;
    //
    uint8_t current_bank:2;
} enc28j60_t;

int enc28j60_init(enc28j60_t *enc28j60);
size_t enc28j60_send(enc28j60_t *enc28j60, uint8_t *data, size_t size);
size_t enc28j60_recv(enc28j60_t *enc28j60, uint8_t *data, size_t size);

#define ENC28J60_REG_EIE            (0x1B)
# define ENC28J60_REG_EIE_RXERIE        (1<<0)
# define ENC28J60_REG_EIE_TXERIE        (1<<1)
# define ENC28J60_REG_EIE_TXIE          (1<<3)
# define ENC28J60_REG_EIE_LINKIE        (1<<4)
# define ENC28J60_REG_EIE_DMAIE         (1<<5)
# define ENC28J60_REG_EIE_PKTIE         (1<<6)
# define ENC28J60_REG_EIE_INTIE         (1<<7)
#define ENC28J60_REG_EIR            (0x1C)
# define ENC28J60_REG_EIR_RXERIF        (1<<0)
# define ENC28J60_REG_EIR_TXERIF        (1<<1)
# define ENC28J60_REG_EIR_TXIF          (1<<3)
# define ENC28J60_REG_EIR_LINKIF        (1<<4)
# define ENC28J60_REG_EIR_DMAIF         (1<<5)
# define ENC28J60_REG_EIR_PKTIF         (1<<6)
#define ENC28J60_REG_ESTAT          (0x1D)
# define ENC28J60_REG_ESTAT_INT         (1<<7)
#define ENC28J60_REG_ECON2          (0x1E)
#define ENC28J60_REG_ECON1          (0x1F)

int 
enc28j60_write_ctrl_reg
(
    enc28j60_t *enc28j60, 
    uint8_t reg, 
    uint8_t *data, 
    uint32_t size
);
int 
enc28j60_read_ctrl_reg
(
    enc28j60_t *enc28j60, 
    uint8_t reg, 
    uint8_t *buff, 
    uint32_t size
);
int enc28j60_bit_field_set(enc28j60_t *enc28j60, uint8_t reg, uint8_t data);
int enc28j60_bit_field_clear(enc28j60_t *enc28j60, uint8_t reg, uint8_t data);
int enc28j60_select_bank(enc28j60_t *enc28j60, int bank);


static inline int enc28j60_enable_interrupt(enc28j60_t *enc28j60, int what)
{
    return enc28j60_bit_field_set(enc28j60, ENC28J60_REG_EIE, what);
}
static inline int enc28j60_disable_interrupt(enc28j60_t *enc28j60, int what)
{
    return enc28j60_bit_field_clear(enc28j60, ENC28J60_REG_EIE, what);
}
static inline int enc28j60_enable_global_interrupt(enc28j60_t *enc28j60)
{
    return enc28j60_enable_interrupt(enc28j60, ENC28J60_REG_EIE_INTIE);
}
static inline int enc28j60_disable_global_interrupt(enc28j60_t *enc28j60)
{
    return enc28j60_disable_interrupt(enc28j60, ENC28J60_REG_EIE_INTIE);
}
// This should NOT be called in interrupt context
#define enc28j60_isr(enc28j60, handler) do \
{ \
    uint8_t reg; \
    enc28j60_disable_global_interrupt(enc28j60); \
    if (enc28j60_read_ctrl_reg(enc28j60, ENC28J60_REG_ESTAT, &reg, 1)) { \
        if (reg & ENC28J60_REG_ESTAT_INT) { \
            if (enc28j60_read_ctrl_reg(enc28j60, ENC28J60_REG_EIR, &reg, 1)) { \
                handler(enc28j60, reg); \
            } \
        } \
    } \
    enc28j60_enable_global_interrupt(enc28j60); \
} while (0)

typedef struct
{
    uint8_t dist_mac_addr[6];
    uint8_t src_mac_addr[6];
    uint8_t *payload;
    uint16_t payload_size;
    uint16_t type;
    uint32_t crc;
    //
    uint8_t *data;
    uint32_t data_size;
} enc28j60_packet_t;

int enc28j60_pack(enc28j60_packet_t *packet);
int enc28j60_unpack(enc28j60_packet_t *packet);


#endif // ENC28J60_H_ 
