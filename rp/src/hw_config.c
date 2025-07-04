/* hw_config.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/
/*

This file should be tailored to match the hardware design.

There should be one element of the spi[] array for each hardware SPI used.

There should be one element of the sd_cards[] array for each SD card slot.
The name is should correspond to the FatFs "logical drive" identifier.
(See http://elm-chan.org/fsw/ff/doc/filename.html#vol)
The rest of the constants will depend on the type of
socket, which SPI it is driven by, and how it is wired.



*/

// NOLINTBEGIN

#include <string.h>
//
#include "hw_config.h"
//
#include "ff.h" /* Obtains integer types */
//
#include "diskio.h" /* Declarations of disk functions */
#include "sdcard.h"

// Hardware Configuration of SPI "objects"
// Note: multiple SD cards can be driven by one SPI if they use different slave
// selects.
static spi_t spis[] = {  // One for each SPI.
    {
        .hw_inst = spi0,  // SPI component
        .miso_gpio = 4,   // GPIO number (not pin number)
        .mosi_gpio = 3,
        .sck_gpio = 2,
        .spi_mode = 3,  // SPI Mode 3
        // .baud_rate = 12.5 * 1000 * 1000,
        .set_drive_strength = true,  // Set drive strength for GPIOs
        .mosi_gpio_drive_strength = GPIO_DRIVE_STRENGTH_2MA,
        .sck_gpio_drive_strength = GPIO_DRIVE_STRENGTH_2MA,
        // .use_static_dma_channels = true,  // Use static DMA channels
        // .tx_dma = 10,                     // DMA channel for TX
        // .rx_dma = 11,                     // DMA channel for RX
    }};

// Hardware Configuration of the SPI interface
static sd_spi_if_t spi_ifs[] = {{
    .spi = &spis[0],             // Pointer to the SPI driving this card
    .ss_gpio = -1,               // The SPI slave select GPIO for this SD card
    .set_drive_strength = true,  // Set drive strength for GPIOs
    .ss_gpio_drive_strength = GPIO_DRIVE_STRENGTH_2MA,
}};

// Hardware Configuration of the SD Card "objects"
static sd_card_t sd_cards[] = {  // One for each SD card
    {
        .type = SD_IF_SPI,
        .spi_if_p = &spi_ifs[0],  // Pointer to the SPI driving this card
        .use_card_detect = false,
        .card_detect_gpio = -1,   // Card detect
        .card_detected_true = -1  // What the GPIO read returns when a card is
                                  // present. Use -1 if there is no card detect.
    }};

/* ********************************************************************** */
size_t sd_get_num() { return count_of(sd_cards); }
sd_card_t *sd_get_by_num(size_t num) {
  if (num <= sd_get_num()) {
    return &sd_cards[num];
  } else {
    return NULL;
  }
}
size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
  if (num <= sd_get_num()) {
    return &spis[num];
  } else {
    return NULL;
  }
}

// NOLINTEND
/* [] END OF FILE */