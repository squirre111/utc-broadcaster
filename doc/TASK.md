Тестовое задание
===========================================================================

Нужно написать сервис рассылки времени, который броадкастом  (UDP) рассылает свой таймстемп клиентам.

Сервер берет время из системы и отдает клиентам раз в 100ms, в виде таймстемпа(ms).
Клиент, получает время и печатает в консоль:

1. свой таймстемп (в момент приема пакета) 
2. полученный таймстемп от сервера 
3. diff (в ms) таймстемпов сервера и клиента.

Логику работы с временем сервера/клиентам нужно развязать с реализацией передачи времени на транспортном уровне.

Для сборки используем cmake.

Для тестового задания, ограничимся рамками одной Linux машины (localhost).

Сервер и клиент должны собираться из исходников, запускаться и работать, как описано в задание.

Обязательно наличие юнит тестов (гнаться за покрытием не надо, лучше покрыть часть функционала, но хорошо)
