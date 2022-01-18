Примеры на различное железо платы для Arduino IDE <br>
<br>
ВНИМАНИЕ смотим соответствие версии платы и софта!<br>
При переходе с версии 1.3 на 1.4 были большие изменения (переопределение GPIO), поэтому примеры надо адаптировать под версию платы.<br>
Примеры тестировались на версии платы 1.3, 1.2.<br>
Modbus пример работает на версиях 1.2 1.3 1.4 (номер версии надо указать в исходном коде) <br>
Для подключения дисплея (почти все примеры это требуют) надо установить две библиотеки Adafruit_GFX_Library и Adafruit-ST7735-Library-master.
Остальные библиотеки нужные для конкретного примера лежат вместе с примером и продублированы в папке с библиотеками.<br>
<br>

Соединение перефирии:<br>
<b>Hardware 1.3 (аналогично 1.2)</b><br>
Key_sensor+LED3:PC13<br>
Photo (R30) ADC0:PA0<br>
SPI 1  [SPI_SCK:PA5, SPI_MISO:PA6, SPI_MOSI:PA7]<br>
TFT [FT_CS: PA1, TFT_RST:PA15, SPI_DC:PA9, TFT_LED PA8(control leds tft)]<br>
Analog key  ADC2:PA2 (alt usart2 TX)<br>
Current sensor (ACS712) ADC3:PA3 (alt usart2 RX)<br>
Alt function USART2 [TX:PA2, RX:PA3]<br>
SD SPI_CS3_sd:PA10<br>
nrf24l01 SPI_CS2_nrf:PA4, SPI_DC:PA9<br>
Value (R31) ADC8:PB0<br>
LED2:PB1 (maple mini led)<br>
LED1:PB10 <br>
OneWire:PB11 <br>
STEPPER:PB12, PB14, PB13, PB15 <br>
USB [USB_DP:PA12 USB_DM:PA11] <br>
Buzzer:PB4 <br>
Encoder [ENC_BTN:PB5, ENC_B:PB8, ENC_A:PB9] <br>
I2C1 [I2C1_SDA:PB7, I2C1_SCL:PB6] <br>

<br>
<b>Hardware pins version 1.4 </b><br>
led1_CE_nrf: [PB10]<br>
LED2 (maple mini led): [PB1]<br> 
LED3+Key_sensor: [PC13]<br>
TFT backlight (PWM): [TFT_LED:PA1]<br>
Photo resistor (R28): [ADC0:PA0]<br> 
Value resistor (R31): [ADC8:PB0]<br>
SPI1 pins: [SPI_SCK:PA5, SPI_MISO:PA6, SPI_MOSI:PA7]<br>
SPI1 TFT: [TFT_CS:PB13, TFT_RST:PA15, SPI_DC:PB14]<br>
SPI1 SD card: [SPI_CS3_sd:PA10]<br>
SPI1 nrf24l01: [SPI_CS2_nrf:PA4, led1_CE_nrf:PB10]<br>
Analog buttons (alt usart2 TX): [ADC2:PA2]<br> 
Current sensor ACS70331 (alt usart2 RX): [ADC3:PA3]<br> 
USART2 (alt function): [TX:PA2, RX:PA3]<br>
OneWire DS18b20: [PB11]<br>
STEPPER: [PB12, PB8, PB9, PB15]<br>
USB: [USB_DP:PA12 USB_DM:PA11]<br>
Buzzer: [PB4]<br>
Encoder internal: [ENC_BTN:PB5, ENC_B:PA8, ENC_A:PA9]<br>
Encoder external: [ENC_BTN:PC13, ENC_B:PA8, ENC_A:PA9] (alt)<br>
I2C1: [I2C1_SDA:PB7, I2C1_SCL:PB6]<br>

<br>
<b>Hardware pins version 1.5 </b><br>
led1_CE_nrf: [PB10]<br>
LED2 (maple mini led): [PB1]<br> 
LED3+Key_sensor: [PC13]<br>
TFT backlight (PWM): [TFT_LED:PA1]<br>
Analog buttons:[ADC0:PA0]<br>
Value resistor (R31): [ADC8:PB0]<br>
SPI1 pins: [SPI_SCK:PA5, SPI_MISO:PA6, SPI_MOSI:PA7]<br>
SPI1 TFT: [TFT_CS:PB13, TFT_RST:PA15, SPI_DC:PB14]<br>
SPI1 nrf24l01: [SPI_CS2_nrf:PA4, led1_CE_nrf:PB10]<br>
SPI1 SD card: [SPI_CS3_sd:PA10]<br>
SPI1 W25Q64: [SPI_CS4_boot1:PB2]<br>
Photo resistor R28 (alt usart2 TX): [ADC2:PA2]<br> 
Current sensor ACS70331 (alt usart2 RX): [ADC3:PA3]<br> 
USART2 (alt function): [TX:PA2, RX:PA3]<br>
OneWire DS18b20: [PB11]<br>
STEPPER: [PB12, PB8, PB9, PB15]<br>
USB: [USB_DP:PA12 USB_DM:PA11]<br>
Buzzer: [PB4]<br>
Encoder internal: [ENC_BTN:PB5, ENC_B:PA8, ENC_A:PA9]<br>
Encoder external: [ENC_BTN:PC13, ENC_B:PA8, ENC_A:PA9] (alt)<br>
I2C1: [I2C1_SDA:PB7, I2C1_SCL:PB6]<br>
<br> 
