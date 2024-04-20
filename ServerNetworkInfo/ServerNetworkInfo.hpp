// # include "../Server/Server.hpp"
#ifndef SERVERNETWORKINFO_HPP
# define SERVERNETWORKINFO_HPP

# include <string>

typedef int                             socket_t;
typedef std::string                     port_t;
typedef std::string                     host_t;

class ServerNetworkInfo
{
    private:
        port_t                  __port;
        host_t                  __host;
        size_t                  __client_max_body_size;
        std::string             __ip_address;

    public:
        const port_t&           get_port() const ;
        const host_t&           get_host() const ;
        const size_t&           get_client_max_body_size() const ;
        const std::string&      get_ip_address() const ;

        void            set_port( const port_t );
        void            set_host( const host_t );
        void            set_client_max_body_size( const size_t );
        void            set_ip_address( const std::string );

        ServerNetworkInfo();
        ServerNetworkInfo&  operator=( const ServerNetworkInfo& );
        ServerNetworkInfo( const ServerNetworkInfo& );
        ~ServerNetworkInfo();
};

#endif
