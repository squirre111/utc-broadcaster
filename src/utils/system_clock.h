#pragma once

#include <chrono>
#include <thread>

#define MS_STR_LEN 13

namespace SystemClock {

    /** Основной тип хранения времени в миллисекундах */
    using ms_size = uint64_t;


    /** Получение текущего времени системы */
    inline ms_size GetMS() {
        return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
    }

    
    /** Приостановка текущего потока на x миллисекунд */
    inline void SleepMS(size_t x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }
}
