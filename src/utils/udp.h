#pragma once


#include "sock_addr.h"
#include "packets.h"


namespace UDP {

    /** C++ интерфейс для UDP-сокета */
    class Socket {
        
    public:
        Socket();
        ~Socket();

        /** Включение опции широковещательной передачи на сокете */
        void AllowBroadcast();

        /** Проверка наличия опции широковещательной передачи на сокете */
        bool HasBroadcast() const;

        /** Дескриптор сокета */
        int FD() const;
        
        /** Закрытие сокета */
        void Close();

        /** Связывание сокета с адресом */
        bool Bind(SockAddr &sa);
        
    private:
        int fd_;
    };

    /** Отправка данных по UDP */
    ssize_t SendTo(const Socket &sock, const SockAddr &sa, Packet &packet);

    /** Получение данных по UDP */
    ssize_t RecvFrom(const Socket &sock, Packet &packet);
}

