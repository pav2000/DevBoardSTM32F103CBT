#ifndef _W25QXXCONFIG_H
#define _W25QXXCONFIG_H

#define _W25QXX_SPI                   hspi1
#define _W25QXX_CS_GPIO               BOOT1_CS4_GPIO_Port //FLASH_CS_GPIO_Port
#define _W25QXX_CS_PIN                BOOT1_CS4_Pin //FLASH_CS_Pin
#define _W25QXX_USE_FREERTOS          0   // Не использовать FREERTOS  1 - использовать
#define _W25QXX_DEBUG                 0

#endif
