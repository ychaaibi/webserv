# include "Server.hpp"

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>  
# include <sys/socket.h>
# include <iostream>

// typedef std::list<Client>::iterator client_it_t;


Server::Server( const error_pages_t& error_pages, const mimes_types_t& mimes_types ) :
        __server_info( error_pages, mimes_types )
{
    __client_max_body_size = static_cast<size_t>(-1);
}

Server::Server( const Server& server ) :
    __socket                ( server.__socket ),
    __port                  ( server.__port ),
    __host                  ( server.__host ),
    __client_max_body_size  ( server.__client_max_body_size ),
    __error_pages_server    ( server.__error_pages_server ),
    __locations             ( server.__locations ),
    __server_info           ( server.__server_info ),
    __server_network_info   ( server.__server_network_info ),
    __clients               ( server.__clients )
{
}

Server&             Server::operator=( const Server& server )
{
    __socket                = server.__socket;
    __port                  = server.__port;
    __host                  = server.__host;
    __client_max_body_size  = server.__client_max_body_size;
    __error_pages_server    = server.__error_pages_server;
    __locations             = server.__locations;
    __server_info           = server.__server_info;
    __server_network_info   = server.__server_network_info;
    __clients               = server.__clients;
    return ( *this );
}

Server::~Server()
{
}

void                Server::set_socket( int socket)
{
    __socket = socket;
}

void                Server::set_port( std::string port )
{
    __server_network_info.set_port( port );
}

void                Server::set_host( std::string host )
{
    __server_network_info.set_host( host );
}

void                Server::set_client_max_body_size( size_t client_max_body_size )
{
    __server_network_info.set_client_max_body_size( client_max_body_size );
}

void                Server::set_err_page( const status_t& error_status_code, const std::string& error_path_page)
{
   __server_info.add_error_page( error_status_code, error_path_page );
}             

const socket_t&     Server::get_socket() const
{
    return ( __socket );
}

const port_t&       Server::get_port() const
{
    return ( __server_network_info.get_port() );
}

const host_t&       Server::get_host() const
{
    return ( __server_network_info.get_host() );
}

const size_t&       Server::get_client_max_body_size() const
{
    return ( __server_network_info.get_client_max_body_size() );
}

const std::string&  Server::get_ip_address() const
{
    return ( __server_network_info.get_ip_address() );
}

const ServerNetworkInfo&    Server::get_server_network_info() const
{
    return ( __server_network_info );
}

const locations_t&  Server::get_locations() const
{
    return ( __locations );
}

const clients_t&     Server::get_clients() const
{
    return ( __clients );
}

clients_t&           Server::get_update_clients()
{
    return ( __clients );
}

void                Server::add_location( const Location& location )
{
    __locations.push_back( location );
}

void                Server::add_client( const Client& client)
{
    __clients.push_back( client );
}


Client              Server::accept_client()
{
    Client              client( *this );
    struct sockaddr_in  client_address;
    socklen_t           client_address_len;

    client.set_socket( accept( __socket, ( struct sockaddr * )&client_address, &client_address_len ) );
    if ( client.get_socket() < 0)
        throw accept_socket_failed();

    int k = 1;
    fcntl(__socket, F_SETFL, O_NONBLOCK);
    if ( setsockopt(client.get_socket(), SOL_SOCKET, SO_REUSEADDR, &k, sizeof(int)) < 0 )
        throw bind_socket_failed();
    
    client.set_time();
    return ( client );
}

client_it_t                Server::end_client_connection( const client_it_t& client_it )
{
    return ( __clients.erase( client_it ) );
}

void                Server::create_socket()
{
    if ( (__socket = socket( PF_INET, SOCK_STREAM, 0 )) < 0 )
        throw create_socket_failed();

    struct sockaddr_in  server_address;

    memset( &server_address, 0, sizeof( server_address ));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( std::atoi( __server_network_info.get_port().c_str() ) );

    if ( bind( __socket, ( struct sockaddr *)&server_address, sizeof( server_address ) ) < 0)
        throw bind_socket_failed();

    int k = 1;
    fcntl(__socket, F_SETFL, O_NONBLOCK);
    if ( setsockopt(__socket, SOL_SOCKET, SO_REUSEADDR, &k, sizeof(int)) < 0 )
        throw bind_socket_failed();
}

void                Server::lunch()
{
    create_socket();

    if ( listen( __socket, 5 ) < 0 )
        throw listen_socket_failed();
}

const ServerInfo&       Server::get_server_info() const
{
    return ( __server_info );
}

// // ExceptionClasses

const char* Server::create_socket_failed::what() const throw()
{
    return ("WebServer--Server-- Create socket failed !!");
}

const char* Server::bind_socket_failed::what() const throw()
{
    return ("WebServer--Server-- Bind socket failed !!");
}

const char* Server::listen_socket_failed::what() const throw()
{
    return ("WebServer--Server-- Listen socket failed !!");
}

const char* Server::accept_socket_failed::what() const throw()
{
    return ("WebServer--Server-- Accept socket failed !!");
}