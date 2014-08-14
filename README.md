Шаблон проекта на микроконтроллерах stm32f10x
-----------

Inglish version below

Шаблон проекта на микроконтроллерах stm32f10x

В шаблоне реализовано:

* Моргание светодиодом
* Реакция на нажатие кнопки
* Задержка в миллисекундах по SysTick
* RTC
* Отображение текущей стадии на MOD-LCD3310


Логика работы программы:

* Включение светодиода
* Выключение светодиода
* Моргание светодиодом с задержкой 500мс по SysTick
* Моргание светодиодом по прерыванию RTC (раз в секунду)

Смена режимов работы происходит по нажатию UserButton

Шаблон адаптирован для отладочной платы Olimex STM-H103, но может быть легко перенесен на другие отладочные платы

-----------

Template project for microcontrollers stm32f10x

The template is implemented:

* Blinking LED
* The response to pressing on user button
* Delay in milliseconds for SysTick
* RTC
* Displays the current stage on MOD-LCD3310


The logic of the program:

* Turn on LED
* Turn off LED
* Blinking LED with delay 500ms by SysTick timer
* Blinking LED on RTC interrupt (1 time per second)

Changing modes is done by pressing UserButton

Template adapted for evaluation board Olimex STM-H103, but can be easily ported to other evaluation boards