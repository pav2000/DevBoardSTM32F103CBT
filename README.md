# DevBoardSTM32F103CBT
Development board on stm32f103cbt6 <br>
<br>
ATTENTION! The board is gradually being finalized and changed, the latest version of hardware is 1.5.
Please note that the proposed program code may NOT be suitable for the specific hardware of the board
(requires further development). Make sure the programming code is hardware correct (i.e. it was developed
for this version of the board).<br>
<br>
Repository structure: <br>
Arduino - working with the board in arduino IDE (libraries and examples). <br>
Board - schematic diagram and photo of the board, old versions. <br>
CubeIDE - working with the board in CubeMX IDE (examples and test program).<br>
DataSheet - documentation for individual chips. <br>
<br>
The board was designed to master the programming of microcontrollers of the stm32 series.
There are a huge number of debug boards on the market for every color and taste. Therefore,
for novice electronics developers, the fastest way to master microcontroller programming is
 to buy a ready-made debug board, since it is necessary to spend significant material and
 time resources on developing and manufacturing your own board.<br>
<br>
Your attention is invited to the project of a debug board based on the STM32F103 microcontroller.<br>
This board is the fruit of the author's work, circuit solutions are borrowed from the technical
documentation of other similar boards. The layout of the board may need to be adjusted to increase its functionality.<br>
<br>
The controller module board is a universal tool based on the STM32F103cbt6 microcontroller,
designed to be built into various systems as a control node.<br>
<br>
Features of the development board:<br>
<br>
- STM32F103CBT6 microcontroller: 72 MHz Cortex-M3, 128 KB program flash, 20 KB RAM,<br>
- CAN, I²C, IrDA, LIN, SPI, UART/USART, USB, DMA, Motor Control PWM, PDR, POR, PVD, PWM, Temp Sensor, WDT, etc.;<br>
- color TFT display with a resolution of 128x160 pixels using spi interface controller st7735;<br>
- connector for sd card (spi is used to connect with the controller);<br>
- radio interface based on nrf24l01 connected via spi; <br>
- reset button;<br>
- jumpers boot0&boot1; <br>
- 4 analog buttons;<br>
- 3 LEDs;<br>
- mechanical encoder with output duplication on a separate connector
(with the ability to select an internal / external encoder using jumpers);<br>
- beeper;<br>
- swd programming connector with swo output extension;<br>
- internal clock (STM32F103 rtc) with battery power and external quartz;<br>
- variable resistor connected to the analog input STM32F103;<br>
- photo resistor connected to the analog input STM32F103;<br>
- temperature sensor ds18b20;<br>
- instead of two analog inputs, it can be configured with jumpers uart STM32F103;<br>
- uart can connect bluetooth module hc-05 or other devices via 4-pin connector;<br>
- distance sensor bus i2c chip vl53l0x;<br>
- magnetic field sensor bus i2c chip hmc5883l;<br>
- pulse sensor bus i2c chip max30102;<br>
- flash memory on the i2c at24c128 (or at24c64) bus (16/8 KB);<br>
- DAC on i2c bus mcp4725 12bit, with connector and LED output;<br>
- i2c connector for connecting external peripherals;<br>
- flash memory on the spi w25q32 bus (4 MB);<br>
- stepper motor control based on uln2003;<br>
- current sensor for measuring the current consumption of a stepper motor based on the ACS70331 chip;<br>
- micro usb connector for VCP or MSD;<br>
- supply voltage +5 V (via mocro USB connector or a separate two-pin connector);<br>
- power supply of the controller core from a 3 V battery;<br>
- overall dimensions: 100x100 mm;<br>
<br>
<br>

ВНИМАНИЕ! Плата постепенно дорабатывается и изменяется, последняя версия железа 1.5.
Обратите внимание что предложенный программный код может НЕ подходить к конкретному железу платы (требует доработки).
Убедитесь что программный код соответствет железу (т.е был разработан для этой версии платы). <br>
<br>
Структура репозитария:<br>
Arduino - работа с платой в arduino IDE (библиотеки и примеры).<br>
Board - принципиальная схема и фото платы, старые версии.<br>
CubeIDE - работа с платой в CubeMX IDE (примеры и тестовая программа).<br>
DataSheet - документация на отдельные микросхемы.<br>
<br>
Плата была разработана для освоения программирования микроконтроллеров серии stm32.
На рынке представлено огромное количество отладочных плат на любой цвет и вкус.
Поэтому для начинающих разработчиков электроники самым быстрым способом освоения
программирования микроконтроллеров является покупка готовой отладочной платы,
поскольку на разработку и изготовление собственной платы необходимо затратить
значительные материальные и временные ресурсы.<br>

Вашему вниманию предлагается проект отладочной платы на базе микроконтроллера STM32F103.<br>
Данная плата является плодом труда автора, схемотехнические решения заимствованы
из технической документации других аналогичных плат.
Разводка платы, возможно, требует корректировки, для увеличения ее функционала.<br>

Плата модуля контроллера является универсальным средством на базе микроконтроллера STM32F103cbt6,
предназначенным для встраивания в различные системы в качестве узла управления.<br>
<br>
Характеристики отладочной платы:<br>
- микроконтроллер STM32F103CBT6: 72 МГц Cortex-M3, 128 Кбайт флэш-памяти программ, 20Кбайта ОЗУ,
- CAN, I²C, IrDA, LIN, SPI, UART/USART, USB, DMA, Motor Control PWM, PDR, POR, PVD, PWM, Temp Sensor, WDT и т.д.;<br>
- цветной TFT дисплей с разрешением 128х160 точек использующий spi интрефейс контроллер st7735;
- разъем для sd карты (для соединения с контролером используется spi);<br>
- радио-интрефейс на основе nrf24l01 подключенный по spi ;<br>
- кнопка сброса ;<br>
- перемычки boot0&boot1;<br>
- 4 аналоговые кнопки;<br>
- 3 светодиода;<br>
- механический энкодер с дублированием выводов на отдельный разъем (с возможностью с помощью перемычек выбирать внутренний/внешний энкодер);<br>
- биппер;<br>
- разъем для программирования swd с расширением выхода swo;<br>
- внутренние часы (STM32F103 rtc) с питанием от батарейки и внешнем кварцем;<br>
- переменный резистор заведенный на аналоговый вход STM32F103;<br>
- фото резистор заведенный на аналоговый вход STM32F103;<br>
- датчик температуры ds18b20;<br>
- вместо двух аналоговых входов можно сконфигурировать перемычками uart STM32F103;<br>
- к uart можно присединять модуль bluetooth hc-05 или другие устройства через 4-х пиновый разъем;
- датчик расстояния шина i2c чип vl53l0x;<br>
- датчик магнитного поля шина i2c чип hmc5883l;<br>
- датчик пульса шина i2c чип max30102;<br>
- флеш память на шине i2c at24c128 (или at24c64) объем 16/8 кбайта;<br>
- DAC на шине i2c mcp4725 12bit, с выходом на разъем и светодиод;<br>
- разъем i2c для подключения внешней периферии;<br>
- флеш память на шине spi w25q32  (объем 4 мбайта);<br>
- управление шаговым двигателеми на основе uln2003;<br>
- датчик тока для измерения тока потребления шаговым двигателем на основе чипа ACS70331;<br>
- micro usb разъем для VCP или MSD;<br>
- напряжение питания +5 В (через разъем mocro USB или отдельный двух пиновый разъем);<br>
- питание ядра контроллера от батарейки 3 В;<br>
- габаритные размеры: 100х100 мм;<br>

Внешний вид платы ver 1.5: <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/Board/view2_v150.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/Board/view3_v150.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/Board/view_up_v150.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/Board/view_down_v150.jpg" width="480" /><br>
<br>
Собранная плата ver 1.5: <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/Board/view_v150.jpg" width="480" /><br>


