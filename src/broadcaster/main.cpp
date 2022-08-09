#include <iostream>

#include <unistd.h>

#include "broadcaster.h"

#define DEFAULT_PORT 12345
#define DEFAULT_TIMEOUT 100

enum class BroadcastType {
    Bytes, String, Foobar

};

std::unique_ptr<Runnable> BroadcastFactory(BroadcastType type, int port, int timeout);
std::string BroadcastType2String(BroadcastType type);
int PrintHelp(char *argv0);



int main(int argc, char **argv)
{
    std::cout << "UDP Broadcaster" << std::endl;
    
    size_t port = 0, timeout = 0;
    
    BroadcastType type;
    type = BroadcastType::Bytes;
    
    int opt = 0;
    while ( (opt = getopt(argc, argv, "p:t:hbsf")) != -1){
        switch (opt) {
        case 'p':
            port = std::atoi(optarg); break;
        case 't':
            timeout = std::atoi(optarg); break;
        case 'b':
            type = BroadcastType::Bytes; break;
        case 's':
            type = BroadcastType::String; break;
        case 'f':
            type = BroadcastType::Foobar; break;
        case 'h':
            return PrintHelp(argv[0]);
        }
    }
    
    if (port == 0 || port > 65535) port = DEFAULT_PORT;
    if (timeout == 0) timeout = DEFAULT_TIMEOUT;

    std::cout << "Broadcast port: " << port << std::endl;
    std::cout << "Broadcast timeout: " << timeout << std::endl;
    std::cout << "Broadcast type: " << BroadcastType2String(type) << std::endl;

    // @see #1
    std::unique_ptr<Runnable> broadcaster = BroadcastFactory(type, port, timeout);

    std::cout << "Broadcast start!" << std::endl;
    return broadcaster->Run();
}


std::unique_ptr<Runnable> BroadcastFactory(BroadcastType type, int port, int timeout)
{
    std::unique_ptr<Runnable> res;
    
    switch (type) {
        
    case BroadcastType::String:
        res.reset(new Broadcaster<TimestampStringFactory>(port, timeout));
        break;
        
    case BroadcastType::Foobar:
        {
            Broadcaster<ConstFactory> *b = new Broadcaster<ConstFactory>(port, timeout);
            b->SetConst("foobar");
            res.reset(b);
        }
        break;
        
    case BroadcastType::Bytes:
    default:
        res.reset(new Broadcaster<TimestampFactory>(port, timeout));
        
    }
    return res;
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
