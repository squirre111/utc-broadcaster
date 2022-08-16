#include <iostream>
#include <csignal>

#include <unistd.h>

#include "broadcaster.h"

#define DEFAULT_PORT 12345
#define DEFAULT_TIMEOUT 100

enum class BroadcastType {
    Bytes, String, Foobar

};

Runnable *BroadcastFactory(BroadcastType type, int port, int timeout);
std::string BroadcastType2String(BroadcastType type);
int PrintHelp(char *argv0);
Runnable *broadcaster = nullptr;

int main(int argc, char **argv)
{
    std::cout << "UDP Broadcaster" << std::endl;
    
    size_t port = 0, timeout = 0;
    
    BroadcastType type;
    type = BroadcastType::Bytes;
    
    int opt = 0;
    while ( (opt = getopt(argc, argv, "p:t:hbsf")) != -1) {
        switch (opt) {
        case 'p': port = std::atoi(optarg); break;
        case 't': timeout = std::atoi(optarg); break;
        case 'b': type = BroadcastType::Bytes; break;
        case 's': type = BroadcastType::String; break;
        case 'f': type = BroadcastType::Foobar; break;
        case 'h': return PrintHelp(argv[0]);
        }
    }
   
    if (port == 0 || port > 65535) port = DEFAULT_PORT;
    if (timeout == 0) timeout = DEFAULT_TIMEOUT;

    std::cout << "Broadcast port: " << port << std::endl;
    std::cout << "Broadcast timeout: " << timeout << std::endl;
    std::cout << "Broadcast type: " << BroadcastType2String(type) << std::endl;
    
    std::unique_ptr<Runnable> broadcaster(BroadcastFactory(type, port, timeout));
    
    // Block SIGUSR1 and SIGUSR2, so other threads
    // will inherit a copy of the signal mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1); sigaddset(&set, SIGUSR2);
    int res = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if (res != 0) std::cerr << "pthread_sigmask error " << res << std::endl;
    
    std::cout << "Broadcast start!" << std::endl;
    broadcaster->Run();

    int sig;
    while(1) {
        res = sigwait(&set, &sig);
        if (res != 0)
            std::cerr << "sigwait error " << res << std::endl;
        else if (sig == SIGUSR1) {
            std::cout << "SIGUSR1 handled" << std::endl;
            if (broadcaster->IsRunning()) 
                broadcaster->Stop();
            else
                broadcaster->Run();
        }
        else if (sig == SIGUSR2) {
            std::cout << "SIGUSR2 handled" << std::endl;
            broadcaster->Stop();
            break;
        }
    }

    return EXIT_SUCCESS;
}


Runnable* BroadcastFactory(BroadcastType type, int port, int timeout)
{
    switch (type) {
    case BroadcastType::String:
        return new Broadcaster<TimestampStringFactory>(port, timeout);
        break;
        
    case BroadcastType::Foobar:
        {
            Broadcaster<ConstFactory> *b = new Broadcaster<ConstFactory>(port, timeout);
            b->SetConst("foobar");
            return b;
        }
        break;
        
    case BroadcastType::Bytes:
    default:
        return new Broadcaster<TimestampFactory>(port, timeout);
    }
    return nullptr;
}


std::string BroadcastType2String(BroadcastType type)
{
    switch (type) {
    case BroadcastType::String:
        return "String";
    case BroadcastType::Foobar:
        return "Foobar";
    case BroadcastType::Bytes:
    default:
        return "Bytes";
    }
    return {};
}


int PrintHelp(char *argv0)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv0 << " [Options]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h               Display this help" << std::endl;
    std::cout << "  -p <PORTNUMBER>  Use port <PORTNUMBER>"
        " instead of default (" << DEFAULT_PORT << ")" << std::endl;
    std::cout << "  -t <TIMEOUT>     Use timeout <TIMEOUT>"
        " in ms instead of default (" << DEFAULT_TIMEOUT << ")" << std::endl;
    std::cout << "  -b               Broadcast timestamp as bytes (default)" << std::endl;
    std::cout << "  -s               Broadcast timestamp as string" << std::endl;
    std::cout << "  -f               Broadcast \"foobar\" string instead of timestamp" << std::endl;
    
    return EXIT_SUCCESS;
}



void atexit_handler() 
{
    std::cout << "atexit_handler" << std::endl;
    delete broadcaster;
}
