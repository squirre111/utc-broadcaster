#pragma once

#include <chrono>
#include <thread>

#define MS_STR_LEN 13

namespace SystemClock {

    using ms_size = uint64_t;

    inline ms_size GetMS() {
        return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
    }

    inline void SleepMS(size_t x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }
}
