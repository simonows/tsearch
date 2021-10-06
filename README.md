# Тестовое задание

## Особенности

* Поиск регистронезависимый, но это можно изменить добавив CASE_SENSITIVE в [tsearch.h](include/tsearch/tsearch.h)
* Поддержана только английская локаль.
* Алгоритм расчета чексуммы такой, каким его понял я: побитовая кольцевая сумма 32 разрядных слов.

## Сборка

Используется система сборки cmake.
Поддерживаются сборки:
* Make (Linux, Mac)
* MSYS2 (Windows)

Возможно соберется и будет работать в VS, но я не проверял.

```sh
mkdir build
cd build

cmake ..
# или
cmake -G "MSYS Makefiles" ..

make

# ./test -f ../../test/example.txt -m word -v people
# ./test -f ../../test/example.txt -m checksum
make run
# 13
# a15f40f
```

Цель: `./build/src/test`

## Структура

* [tsearch.cxx](src/tsearch.cxx) - Реализации 2 главных функций ТЗ
* [mmap.cxx](src/mmap.cxx) - Реализация класса MmapReader для отображения файлов в память
* [main.cxx](src/main.cxx) - Точка входа
* [input_data.cxx](src/input_data.cxx) - Обработка аргументов командной строки
* [genskip.cxx](src/genskip.cxx) - Генераторы шаблонов для более быстрого поиска
* [error.cxx](src/error.cxx) - Определения локальных ошибок

