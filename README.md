# UTC broadcaster

Тестовое задание [здесь](doc/TASK.md)

## Описание проекта

Сервис для рассылки широковещательных датаграмм с временными метками, а также клиент для сбора этих датаграмм.

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

## Запуск

Запуск `Broadcaster`

```
./build/src/broadcaster/broadcaster
```

Запуск `Reciever`

```
./build/src/reciever/reciever
```

![Пример работы](https://github.com/squirre111/utc-broadcaster/raw/main/doc/demo.gif)


## Опции командной строки

```
squirre1@pc:/tmp/utc-broadcaster$ ./build/src/broadcaster/broadcaster -h
UDP Broadcaster
Usage:
./build/src/broadcaster/broadcaster [Options]

Options:
  -h               Display this help
  -p <PORTNUMBER>  Use port <PORTNUMBER> instead of default (12345)
  -t <TIMEOUT>     Use timeout <TIMEOUT> in ms instead of default (100)
  -b               Broadcast timestamp as bytes (default)
  -s               Broadcast timestamp as string
  -f               Broadcast "foobar" string instead of timestamp
  
squirre1@pc:/tmp/utc-broadcaster$ ./build/src/reciever/reciever -h
UDP broadcast reciever
Usage:
./build/src/reciever/reciever [Options]

Options:
  -h               Display this help
  -p <PORTNUMBER>  Use port <PORTNUMBER> instead of default (12345)
```



## Примечания

1. Интересным является вопрос, что называть сервером, а что клиентом, т.к. обычно сервер принимает соединения. Поэтому, в данном случае, для сервиса отправки датаграмм было выбрано название `Broadcaster`, а для сервиса получения датаграмм - `Reciever`.

2. Файлы `factories.h` и `packets.h` содержат больше одного класса лишь для удобства чтения тестового задания. В реальных проектах обычно используется схема: **один класс - один файл**, и каждый файл по имени соответствует классу.

3. Фабрики `TimestampStringFactory` и `ConstFactory` представлены для примера того, что
разработанный класс `Broadcaster` может работать с различным типом создаваемых и отправляемых пакетов (паттерн Стратегия).
Класс, который получает пакеты - `Reciever`, работает только с одной базовой фабрикой `TimestampFactory` для экономии времени разработки.

4. Класс `DynamicPacket` также представлен в качестве демонстрации.

5. Константа `#define MS_STR_LEN 13` в файле `system_clock.h` была вычислена эмпирически, т.к. на ближайшие сотни лет это будет длина utc timestamp в миллисекундах.

6. Метод `DynamicPacket::SetData` написан на языке Си. Работа с байтами в таком виде показалась более наглядной.

7. ~~При первом взгляде на задание возникло ощущение, что предполагается использовать мультиплексирование и многопоточность, однако при ознакомлении с документацией по работе с UDP *(Стивенс У.Р. "UNIX: разработка сетевых приложений", 3-е издание, Глава 8, раздел 8.4)* пришло понимание, что в этом нет необходимости.~~

8. **При более детальном взгляде на задание возникло понимание, что формирование пакета и отправка его будут вносить небольшие задержки в основной цикл и ошибка таймаута будет накапливаться. Для решения данной проблемы, отправка датаграмм была вынесена в отдельный поток и реализована через уведомление из потока таймера.**

## Известные допущения

1. Возможно `system_clock.h` не самое удачное имя для файла, т.к. есть системный класс `std::chrono::system_clock`. Впрочем, сборка была проверена на нескольких системах, и это решение проблем не вызвало.

2. Не было реализовано тестирование `UDP::RecvFrom`, на данный момент не найдено решение как протестировать эту функцию.

3. В файле `broadcaster/main.cpp` присутствует фабрика `BroadcastFactory` и умный указатель на Runnable-объект. Данное решение возникло для реализации возможности создания объекта в зависимости от опций командной строки. Этого можно было не делать, создавать всё на стеке, но тогда бы был менее изящный код. В реальной жизни, если пользователь класса `Broadcaster` будет просто создавать один его экземпляр, то всё будет на стеке и в одну строчку.

4. В функции `UDP::RecvFrom` в случае ошибки парсинга (несоответствия размера структуры) метод `Packet::SetData` просто ничего не присвоит. Программа не сигнализирует об ошибке в явном виде, это показалось мне допустимой стратегией обработки.

