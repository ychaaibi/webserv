#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <map>
#include <vector>

#include <sys/un.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>

# include <list>

#include "../Client/Client.hpp"
#include "../Location/Location.hpp"
#include "../ServerInfo/ServerInfo.hpp"
#include "../ServerNetworkInfo/ServerNetworkInfo.hpp"

class   Client;

typedef int                             status_t;

typedef int                             socket_t;
typedef std::string                     port_t;
typedef std::string                     host_t;
// typedef std::map<status_t,std::string>  error_pages_t;
typedef std::vector<Location>           locations_t;

typedef std::list<Client>               clients_t;
typedef std::list<Client>::iterator     client_it_t;



class   Server
{
    private:
        socket_t                __socket;

        port_t                  __port;
        host_t                  __host;
        size_t                  __client_max_body_size;
        error_pages_t           __error_pages_server;
        locations_t             __locations;
        ServerInfo              __server_info;
        ServerNetworkInfo       __server_network_info;

    public:
        clients_t               __clients;


    //     int                     __check;

        class create_socket_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class bind_socket_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class listen_socket_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };
        class accept_socket_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };

    public:
        void                        set_socket( socket_t socket);
        void                        set_port( port_t port );
        void                        set_host( host_t host );
        void                        set_client_max_body_size( size_t client_max_body_size );
        void                        set_err_page( const status_t& error_status_code, const std::string& error_path_page);     

        const socket_t&             get_socket() const;
        const port_t&               get_port() const;
        const host_t&               get_host() const;
        const size_t&               get_client_max_body_size() const;
        const std::string&          get_ip_address() const;
        const ServerNetworkInfo&    get_server_network_info() const;

        const ServerInfo&           get_server_info() const;
        const locations_t&          get_locations() const;
        const clients_t&            get_clients() const;
        clients_t&                  get_update_clients();

    private:
        void                        create_socket();

    public:
        void                        add_client( const Client& client);
        void                        add_location( const Location& location );
        
        Client                      accept_client();
        client_it_t                 end_client_connection( const client_it_t& client_it );

        void                        lunch();


        Server( const error_pages_t& error_pages, const mimes_types_t& mimes_types );
        Server( const Server& server );
        Server& operator=( const Server& server );
        ~Server();
};

#endif
