# include "ServerNetworkInfo.hpp"

ServerNetworkInfo::ServerNetworkInfo()
{
    __client_max_body_size = static_cast<size_t>( -1 );
}

ServerNetworkInfo::~ServerNetworkInfo()
{

}

ServerNetworkInfo&  ServerNetworkInfo::operator=( const ServerNetworkInfo& server_network_info )
{
    __port                  = server_network_info.__port;
    __host                  = server_network_info.__host;
    __client_max_body_size  = server_network_info.__client_max_body_size;
    __ip_address            = server_network_info.__ip_address;

    return ( *this );
}

ServerNetworkInfo::ServerNetworkInfo( const ServerNetworkInfo& server_network_info ) :
    __port                      ( server_network_info.__port ),
    __host                      ( server_network_info.__host ),
    __client_max_body_size      ( server_network_info.__client_max_body_size ),
    __ip_address                ( server_network_info.__ip_address )
{
}

const port_t&          ServerNetworkInfo::get_port() const 
{
    return ( __port );
}
const host_t&          ServerNetworkInfo::get_host() const 
{
    return ( __host );
}
const size_t&          ServerNetworkInfo::get_client_max_body_size() const 
{
    return ( __client_max_body_size );
}
const std::string&     ServerNetworkInfo::get_ip_address() const 
{
    return ( __ip_address );
}

void            ServerNetworkInfo::set_port( const port_t port )
{
    __port = port;
}
void            ServerNetworkInfo::set_host( const host_t host )
{
    __host = host;
}
void            ServerNetworkInfo::set_client_max_body_size( const size_t max_body_size )
{
    __client_max_body_size = max_body_size;
}
void            ServerNetworkInfo::set_ip_address( const std::string ip_address )
{
    __ip_address = ip_address;
}