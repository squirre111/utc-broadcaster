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

        /** Включение опции переиспользования порта для bind */
        void AllowReusePort();

        /** Проверка наличия опции широковещательной передачи на сокете */
        bool HasBroadcast() const;

        /** Проверка наличия опции переиспользования порта для bind */
        bool HasReusePort() const;

        /** Дескриптор сокета */
        int FD() const;
        
        /** Закрытие сокета */
        void Close();

        /** Связывание сокета с адресом */
        bool Bind(SockAddr &sa);
        
    private:
        
        /** Установка абстрактной опции */
        template<typename T>
        void SetSockOption(int optname, const T val);
        
        /** Считывание абстрактной опции */
        template<typename T>
        bool GetSockOption(int optname, T *val) const;
        
        int fd_;
        
    };

    /** Отправка данных по UDP */
    ssize_t SendTo(const Socket &sock, const SockAddr &sa, Packet *packet);

    /** Получение данных по UDP */
    ssize_t RecvFrom(const Socket &sock, Packet *packet);
}

