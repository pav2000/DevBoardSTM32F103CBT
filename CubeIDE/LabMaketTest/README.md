Программа для тестирования board stm32f103cbt6 <br>
Версия платы 1.5!!
Cхема платы и фото лежат в директории Board, сам проект разработан в CubeMX IDE.<br>
На плату ставится i2c флеш в зависимости от наличия at24c128 или at24c64, объемом 16/8 кбайт, на работу не влияет.<br>
Также замечана не стабильная работа датчика VL53L0X. Некоторые датчики работают стабильно а на некоторых возникает проблема с
вторичным запуском (датчик не показывает расстояние когда повторно входим в его раздел). Лечится путем снятия питания (сбросом датчика).<br>
Проблема изучается в настоящее время. 23.04.2023 - вроде разобрался датчик не любит повторной инициализации, сделал однократную инициализацию 
стало рабоать, но при перепрошивке иногда датчик не запускается - лечится снятием питания.<br>
<br>
При генерации заново исходников с использованием CubeMX, будет ошибка компиляции, необходимо удалить файл FATFS/Target/user_diskio.c, такой
файл (исправленный для работы с SD картой) уже лежит в основной ветке проекта.<br>
<br>
Вид некоторых пунктов меню:<br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/001.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/002.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/003.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/004.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/005.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/006.jpg" width="480" /> <br>
<img src="https://github.com/pav2000/DevBoardSTM32F103CBT/blob/main/CubeIDE/LabMaketTest/Picture/007.jpg" width="480" /> <br>


