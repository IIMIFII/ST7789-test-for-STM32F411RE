# STM32F4 + ST7789 TFT Display Driver (HAL & Bare-Metal)

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform: STM32F4](https://img.shields.io/badge/Platform-STM32F4-blue)
![Version](https://img.shields.io/badge/Version-1.0.0-blue)

---

## 🇷🇺 Русская версия

Полноценный драйвер для подключения цветного SPI-дисплея на базе контроллера ST7789 (240x240 пикселей) к микроконтроллерам STM32F4. Проект включает в себя два варианта реализации: базовую низкоуровневую (bare-metal) инициализацию и графическую библиотеку, а также пример использования с HAL от STM32CubeIDE.

![Подключение ST7789 к STM32F4](https://github.com/IIMIFII/ST7789-test-for-STM32F411RE/raw/main/image%20for%20git/st7789-ips-tft-no-cs-pin.png)

### ✨ Возможности

*   Поддержка дисплеев на контроллере ST7789 с разрешением 240x240 пикселей (IPS).
*   Интерфейс: SPI (Mode 3, до 25 МГц).
*   **Графическая библиотека:**
    *   Рисование пикселей, линий, прямоугольников, окружностей, треугольников.
    *   Заливка областей и экрана целиком.
    *   Вывод текста различными шрифтами (7x10, 11x18, 16x26).
    *   Вывод полноэкранных и частичных изображений (RGB565).
*   **Два способа реализации:**
    1.  **Bare-metal:** Прямая работа с регистрами (SPI, GPIO) для максимальной эффективности и понимания процессов.
    2.  **HAL (STM32CubeIDE):** Реализация с использованием Hardware Abstraction Layer для быстрой интеграции в проекты, созданные в CubeMX/CubeIDE.
*   Настраиваемая ориентация экрана (ротация).

### 🔌 Аппаратное подключение

| ST7789 TFT | STM32F411 Nucleo-64 | Примечание               |
|------------|----------------------|--------------------------|
| VCC        | 5V                   | Питание                  |
| GND        | GND                  | Общий                    |
| SCL        | PA5 (SPI1_SCK)       | Тактовый сигнал SPI      |
| SDA        | PA7 (SPI1_MOSI)      | Линия данных SPI (TX)    |
| RES        | PA0 (GPIO Output)    | Сброс дисплея            |
| DC         | PA1 (GPIO Output)    | Data/Command (1/0)       |

**Важно:** У данного модуля TFT отсутствует пин CS (Chip Select) — он постоянно притянут к земле внутри модуля.

### 🛠️ Настройка и использование

#### 1. Конфигурация SPI (Режим 3)
Для корректной работы дисплея SPI должен быть настроен в режиме 3:
*   `CPOL = 1` (Полярность: HIGH)
*   `CPHA = 1` (Фаза: 2 edge)

#### 2. Добавление в проект (HAL + CubeMX)

1.  Скопируйте файлы `st7789.h`, `st7789.c`, `fonts.h`, `fonts.c` и `delay.h`, `delay.c` в папку `Core/Inc` и `Core/Src` вашего проекта.
2.  В CubeMX настройте:
    *   Пины PA5, PA6, PA7 как `SPI1` с параметрами `CPOL=High` и `CPHA=2 Edge`.
    *   Пины PA0, PA1 как `GPIO_Output`.
3.  Включите в `main.c`:
    ```c
    /* USER CODE BEGIN Includes */
    #include "st7789.h"
    #include "delay.h"
    /* USER CODE END Includes */

    /* USER CODE BEGIN 2 */
    systick_init(); // Инициализация SysTick для задержек
    ST7789_Init();
    ST7789_Test(); // Запустить демо-тест
    /* USER CODE END 2 */
    ```

#### 3. Пример кода (Bare-Metal)

```c
#include "st7789.h"
#include "delay.h"

int main(void) {
    systick_init();
    ST7789_Init();                // Инициализация дисплея

    ST7789_Fill_Color(BLACK);     // Очистка экрана

    // Рисуем примитивы
    ST7789_DrawRectangle(20, 20, 120, 120, RED);
    ST7789_DrawFilledCircle(70, 70, 30, BLUE);
    ST7789_DrawLine(0, 0, 239, 239, GREEN);
    ST7789_WriteString(10, 150, "Hello, World!", Font_11x18, WHITE, BLACK);

    while(1);
}
```

### 🖼️ Конвертация изображений для вывода на дисплей

Для отображения картинок с помощью функции `ST7789_DrawImage()` их необходимо преобразовать в массив языка C формата RGB565 (16 бит на пиксель). Для этого можно воспользоваться специальным Python-скриптом.

#### 🔧 Конвертер JPG в RGB565 массив

Скрипт **[STM32-ST7789_image_to_uint16_converter](https://github.com/IIMIFII/STM32-ST7789_image_to_uint16_converter)** автоматически выполняет все необходимые преобразования.

**Что он делает:**
1.  Загружает JPG-изображение.
2.  Изменяет его размер до 240x240 пикселей.
3.  Конвертирует каждый пиксель в формат RGB565 (R:5 бит, G:6 бит, B:5 бит).
4.  Приводит байты к формату **little-endian**, который ожидает драйвер.
5.  Генерирует готовые к компиляции `.c` и `.h` файлы с двумерным массивом `[240][240]`.
6.  Создает JPG-превью, чтобы визуально убедиться в корректности преобразования.

#### 📥 Установка и использование конвертера

1.  **Установите Python-зависимости:**
    ```bash
    pip install Pillow numpy
    ```
2.  **Склонируйте репозиторий со скриптом:**
    ```bash
    git clone https://github.com/IIMIFII/STM32-ST7789_image_to_uint16_converter.git
    cd STM32-ST7789_image_to_uint16_converter
    ```
3.  **Запустите конвертацию:**
    ```bash
    python convert.py your_image.jpg -n my_image_name
    ```
    *   `your_image.jpg` — ваш исходный файл.
    *   `-n my_image_name` — желаемое имя массива в коде (по умолчанию `saber`).

После выполнения вы получите файлы `my_image_name_le.c` и `my_image_name_le.h`.

#### 🎯 Интеграция изображения в проект

1.  Скопируйте сгенерированные `.c` и `.h` файлы в папку `Core/Src` и `Core/Inc` вашего проекта.
2.  В том месте, где нужно вывести картинку, добавьте код:
    ```c
    #include "my_image_name_le.h"

    // Вывод изображения
    ST7789_DrawImage(0, 0, 240, 240, (const uint16_t*)my_image_name);
    ```

**Важно:** Размер данных для полноэкранного изображения 240x240 составляет **115 200 байт** (112.5 КБ). Убедитесь, что в вашем микроконтроллере достаточно свободной Flash-памяти.

### 📚 API

| Функция                             | Описание                                      |
|-------------------------------------|-----------------------------------------------|
| `ST7789_Init()`                     | Инициализация дисплея                         |
| `ST7789_SetRotation(uint8_t m)`     | Установка ориентации экрана (0-3)             |
| `ST7789_Fill_Color(uint16_t color)` | Заливка всего экрана цветом                   |
| `ST7789_DrawPixel()`                | Рисование пикселя                             |
| `ST7789_DrawLine()`                 | Рисование линии                               |
| `ST7789_DrawRectangle()`            | Рисование прямоугольника (контур)             |
| `ST7789_DrawFilledRectangle()`      | Рисование залитого прямоугольника             |
| `ST7789_DrawCircle()`               | Рисование окружности (контур)                 |
| `ST7789_DrawFilledCircle()`         | Рисование залитой окружности                  |
| `ST7789_WriteString()`              | Вывод строки текста                           |
| `ST7789_DrawImage()`                | Вывод изображения из массива (RGB565)         |
| `ST7789_InvertColors()`             | Инверсия цветов всего экрана                  |

### 🧪 Тестирование
Функция `ST7789_Test()` запускает демонстрационный цикл, последовательно отображая различные цвета, геометрические фигуры, текст и тестовое изображение для проверки работоспособности дисплея.

### 📁 Структура проекта

*   `Core/Inc/` & `Core/Src/`:
    *   `st7789.h`, `st7789.c` - основной драйвер.
    *   `fonts.h`, `fonts.c` - файлы со шрифтами и тестовым изображением.
    *   `delay.h`, `delay.c` - функции задержки (`delay`).
    *   `spi.h`, `spi.c` - низкоуровневая инициализация SPI и передача данных (bare-metal часть).

---

## 🇬🇧 English Version

A comprehensive driver for interfacing STM32F4 microcontrollers with SPI-based ST7789 TFT displays (240x240 pixels). The project includes two implementation options: bare-metal register-level initialization and a HAL-based version for STM32CubeIDE integration.

![ST7789 Connection to STM32F4](https://github.com/IIMIFII/ST7789-test-for-STM32F411RE/raw/main/image%20for%20git/st7789-ips-tft-no-cs-pin.png)

### ✨ Features

*   Support for ST7789-based displays with 240x240 pixel resolution (IPS).
*   Interface: SPI (Mode 3, up to 25 MHz).
*   **Graphics Library:**
    *   Drawing pixels, lines, rectangles, circles, triangles.
    *   Filling areas and full screen.
    *   Text output with multiple fonts (7x10, 11x18, 16x26).
    *   Full-screen and partial image display (RGB565).
*   **Two Implementation Methods:**
    1.  **Bare-metal:** Direct register manipulation (SPI, GPIO) for maximum efficiency and learning.
    2.  **HAL (STM32CubeIDE):** Hardware Abstraction Layer implementation for quick integration into CubeMX/CubeIDE projects.
*   Configurable screen rotation.

### 🔌 Hardware Connection

| ST7789 TFT | STM32F411 Nucleo-64 | Description             |
|------------|----------------------|-------------------------|
| VCC        | 5V                   | Power                   |
| GND        | GND                  | Ground                  |
| SCL        | PA5 (SPI1_SCK)       | SPI Clock               |
| SDA        | PA7 (SPI1_MOSI)      | SPI Data (TX)           |
| RES        | PA0 (GPIO Output)    | Display Reset           |
| DC         | PA1 (GPIO Output)    | Data/Command (1/0)      |

**Important:** This TFT module has no CS (Chip Select) pin — it's permanently tied to ground internally.

### 🛠️ Setup and Usage

#### 1. SPI Configuration (Mode 3)
For proper display operation, SPI must be configured in Mode 3:
*   `CPOL = 1` (Polarity: HIGH)
*   `CPHA = 1` (Phase: 2 edge)

#### 2. Adding to Project (HAL + CubeMX)

1.  Copy the files `st7789.h`, `st7789.c`, `fonts.h`, `fonts.c`, `delay.h`, and `delay.c` to your project's `Core/Inc` and `Core/Src` folders.
2.  In CubeMX, configure:
    *   Pins PA5, PA6, PA7 as `SPI1` with parameters `CPOL=High` and `CPHA=2 Edge`.
    *   Pins PA0, PA1 as `GPIO_Output`.
3.  Include in `main.c`:
    ```c
    /* USER CODE BEGIN Includes */
    #include "st7789.h"
    #include "delay.h"
    /* USER CODE END Includes */

    /* USER CODE BEGIN 2 */
    systick_init(); // Initialize SysTick for delays
    ST7789_Init();
    ST7789_Test(); // Run demo test
    /* USER CODE END 2 */
    ```

#### 3. Bare-Metal Code Example

```c
#include "st7789.h"
#include "delay.h"

int main(void) {
    systick_init();
    ST7789_Init();                // Initialize display

    ST7789_Fill_Color(BLACK);     // Clear screen

    // Draw primitives
    ST7789_DrawRectangle(20, 20, 120, 120, RED);
    ST7789_DrawFilledCircle(70, 70, 30, BLUE);
    ST7789_DrawLine(0, 0, 239, 239, GREEN);
    ST7789_WriteString(10, 150, "Hello, World!", Font_11x18, WHITE, BLACK);

    while(1);
}
```

### 🖼️ Image Conversion for Display Output

To display images using the `ST7789_DrawImage()` function, they need to be converted to a C array in RGB565 format (16 bits per pixel). A specialized Python script can be used for this purpose.

#### 🔧 JPG to RGB565 Array Converter

The **[STM32-ST7789_image_to_uint16_converter](https://github.com/IIMIFII/STM32-ST7789_image_to_uint16_converter)** script automatically performs all necessary conversions.

**What it does:**
1.  Loads a JPG image.
2.  Resizes it to 240x240 pixels.
3.  Converts each pixel to RGB565 format (R:5 bits, G:6 bits, B:5 bits).
4.  Converts bytes to **little-endian** format expected by the driver.
5.  Generates ready-to-compile `.c` and `.h` files with a 2D array `[240][240]`.
6.  Creates a JPG preview for visual verification of the conversion.

#### 📥 Installation and Usage

1.  **Install Python dependencies:**
    ```bash
    pip install Pillow numpy
    ```
2.  **Clone the script repository:**
    ```bash
    git clone https://github.com/IIMIFII/STM32-ST7789_image_to_uint16_converter.git
    cd STM32-ST7789_image_to_uint16_converter
    ```
3.  **Run the conversion:**
    ```bash
    python convert.py your_image.jpg -n my_image_name
    ```
    *   `your_image.jpg` — your source file.
    *   `-n my_image_name` — desired array name in code (default: `saber`).

After execution, you'll get the files `my_image_name_le.c` and `my_image_name_le.h`.

#### 🎯 Integrating the Image into Your Project

1.  Copy the generated `.c` and `.h` files to your project's `Core/Src` and `Core/Inc` folders.
2.  Where you want to display the image, add the code:
    ```c
    #include "my_image_name_le.h"

    // Display the image
    ST7789_DrawImage(0, 0, 240, 240, (const uint16_t*)my_image_name);
    ```

**Important:** The data size for a full-screen 240x240 image is **115,200 bytes** (112.5 KB). Ensure your microcontroller has enough free Flash memory.

### 📚 API Reference

| Function                           | Description                               |
|------------------------------------|-------------------------------------------|
| `ST7789_Init()`                    | Display initialization                    |
| `ST7789_SetRotation(uint8_t m)`    | Set screen orientation (0-3)              |
| `ST7789_Fill_Color(uint16_t color)`| Fill entire screen with color              |
| `ST7789_DrawPixel()`               | Draw a pixel                              |
| `ST7789_DrawLine()`                | Draw a line                               |
| `ST7789_DrawRectangle()`           | Draw a rectangle (outline)                |
| `ST7789_DrawFilledRectangle()`     | Draw a filled rectangle                   |
| `ST7789_DrawCircle()`              | Draw a circle (outline)                   |
| `ST7789_DrawFilledCircle()`        | Draw a filled circle                      |
| `ST7789_WriteString()`             | Print a text string                       |
| `ST7789_DrawImage()`               | Display an image from array (RGB565)      |
| `ST7789_InvertColors()`            | Invert all screen colors                  |

### 🧪 Testing
The `ST7789_Test()` function runs a demonstration cycle, sequentially displaying various colors, geometric shapes, text, and a test image to verify display functionality.

### 📁 Project Structure

*   `Core/Inc/` & `Core/Src/`:
    *   `st7789.h`, `st7789.c` - main driver.
    *   `fonts.h`, `fonts.c` - font files and test image.
    *   `delay.h`, `delay.c` - delay functions.
    *   `spi.h`, `spi.c` - low-level SPI initialization and data transfer (bare-metal part).

---

**Happy Coding!** 🖥️✨
