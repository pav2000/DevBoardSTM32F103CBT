Использование платы с Arduino IDE (maple mini)<br>
pav2000 10/01/2024 <br>
ВНИМАНИЕ!  При прошивке на Arduino IDE (версия 1.8.х) у меня выскочила ошибка Java Runtime Environment, после обновления на 8 версию java
лечится либо откатом на 7 версию либо переходом на 32 битную 8 версию. Данная проблема описана в инете. <br>
https://github.com/rogerclarkmelbourne/Arduino_STM32/issues/813  - я использовал вот это (последний пост) <br>
https://github.com/rogerclarkmelbourne/Arduino_STM32/issues/834 <br>
https://community.platformio.org/t/bluepill-f103c8-java-runtime-error/24761 <br>
https://www.stm32duino.com/viewtopic.php?t=604 <br>
Возможно надо перейти на среду версии 2.х <br>
<br>
Отладочную плату STM32 базе микроконтроллера STM32F103C8T6 можно программировать
с помощью Arduino IDE. STM32 дает больше возможностей в плане производительности
 по сравнению с Arduino Nano.<br>
Данная плата должна быть прошита загрузчиком от maple mini и при выборе платы в менеджере плат надо выбирать maple mini.<br>
Загрузчик надо шить generic-none_bootloader.bin (с maple_mini_boot20.bin почему то не появлялся ком порт) но DFU появлялось<br>
bootloaderSTM32.zip - содержит два варианта загрузчиков для платы.<br>
Директория tools содержит утилиту для обмена по modbus и драйвера для чипа ch340 (usb-uart), оба архива для windows.<br>
Директория Examples - содержит различные примеры по работе с периферией платы.<br>
Директория Lib - содержит библиотеки (в них есть простые примеры работы, но надо настраивать под плату) для работы с периферией платы.<br>
<br>
Интеграция stm32f103 в среду arduino описана вот здесь:<br>
https://habr.com/ru/post/395577/<br>
https://istarik.ru/blog/arduino/102.html<br>
http://www.count-zero.ru/2017/stm32duino/<br>
http://digitrode.ru/computing-devices/mcu_cpu/2237-kak-zaprogrammirovat-stm32-s-pomoschyu-arduino-ide.html <br>
http://arduino.ru/forum/obshchii/arduino-ide-addon-ot-stm32<br>
<br>
Видео по настройке arduino ide для stm32<br>
https://www.youtube.com/watch?v=5mThFmpC1U4<br>
https://www.youtube.com/watch?v=878k4KqF7Xs<br>
https://www.youtube.com/watch?v=K-jYSysmw9w<br>
https://www.youtube.com/watch?v=zUk0lN1oEwQ<br>
<br>
github с доработками для ардуино под stm32 https://github.com/rogerclarkmelbourne/Arduino_STM32<br>

