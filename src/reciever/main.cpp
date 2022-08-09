#include <iostream>

#include <unistd.h>

#include "reciever.h"

#define DEFAULT_PORT 12345


int PrintHelp(char *argv0);

int main(int argc, char **argv)
{
    std::cout << "UDP broadcast reciever" << std::endl;
    
    size_t port = 0;
    
    int opt = 0;
    while ( (opt = getopt(argc, argv, "p:h")) != -1){
        switch (opt) {
        case 'p':
            port = std::atoi(optarg); break;
        case 'h':
            return PrintHelp(argv[0]);
        }
    }
    
    if (port == 0 || port > 65535) port = DEFAULT_PORT;

    std::cout << "Broadcast reciever port: " << port << std::endl;
    std::cout << "Broadcast reciever start!" << std::endl;
    Reciever r(port);
    return r.Run();
}


int PrintHelp(char *argv0)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv0 << " [Options]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h               Display this help" << std::endl;
    std::cout << "  -p <PORTNUMBER>  Use port <PORTNUMBER>"
        " instead of default (" << DEFAULT_PORT << ")" << std::endl;
    return EXIT_SUCCESS;
}
