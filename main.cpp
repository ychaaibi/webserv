#include <iostream>
#include "WebServer/WebServer.hpp"
#include "Parser/Parser.hpp"

int main( int ac, char **av )
{
    if ( ac != 2)
    {
        std::cerr << "WebServer : Invalid Number of arguments !!" << std::endl;
        return (1);
    }


    try
    {
        {
            Parser      config_file;
            WebServer&  web_server = config_file.get_web_server( av[1] );

            web_server.lunch();
        }
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }

    return (0);
}