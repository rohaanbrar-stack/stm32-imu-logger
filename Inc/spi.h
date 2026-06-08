#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void SPI_Init(void);
uint8_t SPI_Transfer(uint8_t data);
void SPI_CS_Low(void);
void SPI_CS_High(void);

#endif
