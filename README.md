# Libreria sAPI para Placa Educativa STM32F4

La placa base educativa que hemos desarrollado para montar el kit STM32F4DISCOVERY es ideal para trabajar en forma práctica en entornos de enseñanza de sistemas embebidos. La misma integra los periféricos más utilizados para dicho tipo de prácticas.

<p align="center">
    <img width="300" alt="Foto de la baseboard" src="../master/docs/baseboard_picture.png?raw=true">
</p>

El kit cuenta con un microcontrolador STM32F407VGT6 de la empresa STMicroelectronics con núcleo de 32-bit ARM®Cortex®-M4 con FPU, 1-Mbyte de memoria Flash y 192-Kbyte de RAM.

Hemos desarrollado también el firmware necesario para acelerar el proceso de enseñanza-aprendizaje partiendo del port de la librería sAPI (utilizada en el curso de capacitación de sistemas embebidos del INET) y varios ejemplos de aplicación que la complementan. 
El hardware y el firmware desarrollados en este proyecto permiten reducir los tiempos, los errores de interconexión y los materiales auxiliares necesarios para trabajar con distintos periféricos, sobre todo con alumnos recién iniciados en este campo.

<p align="center">
    <img width="700" alt="Diagrama de bloques simplificado" src="../master/docs/bloques.png?raw=true">
</p>

A continuación se detalla la lista de periféricos incluidos en la placa para este proyecto:
- Un potenciómetro y circuitos de acondicionamiento de señales para entradas para pruebas analógicas.
- Botonera de 5 pulsadores tipo push button y conector para teclado matricial de 4x4.
- Interfaces de comunicación UART y conector para GPIOs adicionales.
- Interfaz LCD (display) de 2x16 caracteres.
- Dos salidas optoacopladas con LEDs indicadores.
- Un LED RGB con sus transistores driver y un display de 7 segmentos con transistor de habilitación.
- Buzzer piezoeléctrico sin oscilador o micro parlante con su transistor driver.
- Un encoder incremental de dos fases en cuadratura de acción manual.

<p align="center">
    <img alt="PCB" src="../master/docs/baseboard.png?raw=true">
</p>
