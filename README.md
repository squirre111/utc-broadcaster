# UTC broadcaster

Само задание [здесь](TASK.md)

## Описание проекта

## Сборка

Согласно заданию, для сборки используется CMake. Для удобства использования представлен скрипт `build_and_test.sh` который собирает проект и выполняет тесты

```
git clone git@github.com:squirre111/utc-broadcaster.git --recurse-submodules
cd utc-broadcaster
bash build_and_test.sh
```

![Процесс сборки](https://github.com/squirre111/utc-broadcaster/raw/main/doc/make.gif)


Все подпроекты собраны с максимально строгим набором флагов: **-Wall -Wextra -Wpedantic -Werror**

## Структура проекта

```
├── src
│   ├── broadcaster
│   │   ├── broadcaster.h   Сервис Broadcaster
│   │   └── main.cpp
│   │
│   ├── reciever
│   │   ├── reciever.h      Слушающий клиент Reciever
│   │   └── main.cpp
│   │
│   └── utils
│       ├── factories.cpp   Реализации нескольких фабрик построения и разбора пакетов
│       ├── factories.h
│       ├── sock_addr.cpp   C++ интерфейс для структуры адреса сокета
│       ├── sock_addr.h
│       ├── udp.cpp         C++ интерфейс для сокета и методов SendTo и RecvFrom
│       ├── udp.h
│       ├── packets.h       Реализации нескольких типов пакетов
│       └── system_clock.h  Функции работы со временем
│
├── tests
│   ├── utils-test.cpp      Unit-тесты проекта
│   └── main.cpp
│
├── lib
│   └── googletest          Подмодуль googletest
│
└── doc                     Задание и изображения
```

## Демонстрация

## Известные допущения

