#pragma once

#include <string>
#include <netinet/in.h>

/**
   C++ интерфейс для структуры адреса сокета
*/
class SockAddr {
    
public:
    enum class Type : uint32_t {
        ANY = INADDR_ANY,
        BROADCAST = INADDR_BROADCAST
    };

    /** Создание адреса по строке */
    SockAddr(std::string addr, size_t port);
    
    /** Создание адреса через типизированное перечисление */
    SockAddr(Type addr, size_t port);
    
    /** Строковое представление адреса */
    std::string Str() const;

    /** Порт */
    size_t Port() const;

    /** Сырой указатель на структуру адреса */
    const struct sockaddr *Addr() const;

    /** Длина структуры адреса */
    socklen_t AddrLen() const;
    
private:
    struct sockaddr_in sa_;
    
};
