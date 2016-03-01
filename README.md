# Control_artesian_well

САУ артскважина

1 Назначение системы

Система разрабатывается для дополнения существующей системы управления насосом артскважины с плавным пуском и остановом. Существующая система плавного пуска и останова не может запустить насос при работе от резервного генератора. Устройство плавного пуска реализовано на тиристорной схеме. При пуске насоса происходит просадка напряжения генератора из-за непостоянной нагрузки. Регулятор напряжения на генераторе не справляется с регулированием катушки возбуждения при плвном пуске насоса.
При подключении генератора на прямую к насосу, пуск и работа насоса происходит без существенных провалов по напряжению.

2 Функции системы
- САУ должна работать совместно со штатной системой плавного пуска и останова. Приоритет управления выставляется в ручном режиме.
- САУ должна поддерживать установленное давление воды в системе
- САУ должна иметь защиту от:
    - от разрыва трубопровода или отсутствия нагрузки на насосе
    - от аварийного давления в системе
- на САУ должно отображаться индикация:
    - текущее давление в системе
    - задание по давлению
    - ошибки системы
- САУ должна позволять изменять задание и гистерезис по давлению
- настройки должны храниться в энергонезависимой памяти
- возможность поддержки протокола ModBus. (Опционально, если хватит ресурсов)
- ну и собственно аппаратная платформа, пускай будет arduino UNO.

3 Данные для разработки
- напряжение питания САУ - 220 В 50 ГЦ 
- мощность насоса 2500 кВА
- давление воды 2-3 кгс/см2
- датчик давления  -  ММ370 (или аналог) с сопротивлением 150-200 Ом

4 Структурная схема
![Проект](https://github.com/MaxKravt/Control_artesian_well/raw/master/img/project_1.png)

5 Схема подключения
![Схема](https://github.com/MaxKravt/Control_artesian_well/raw/master/img/project_2.png)
       
      




    
