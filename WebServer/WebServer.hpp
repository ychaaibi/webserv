#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <string>
# include <vector>
# include <iostream>
# include <fstream>
# include <list>
# include <set>

# include "../Server/Server.hpp"
# include "../ServerInfo/ServerInfo.hpp"
# include "../Utils/Utils.hpp"

typedef int                         socket_t;
typedef std::list<Server>           servers_t;

typedef std::list<Client>                           clients_t;
typedef std::list<Client>::iterator           client_it_t;

# define    WEB_SERVER_ON true

# define    FAILED -1
# define    CONNECTION_CLOSED 0

# define    RECV_FAILED -1
# define    SEND_FAILED -1

# define    TIMEOUT 60

typedef int recv_status_t;
typedef int send_status_t;

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <exception>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>


class WebServer
{

    private:
        class multi_plexing_select_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class multi_plexing_recv_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class multi_plexing_send_failed : public std::exception
        {
            public:
                const char* what() const throw();
        };

        
        class multi_plexing_clear_client : public std::exception
        {
            public:
                const char* what() const throw();
        };

    private:
        fd_set                  sockets_read_set;
        fd_set                  sockets_write_set;

        fd_set                  curr_sockets_readable_set;
        fd_set                  curr_sockets_writeable_set;

        std::set<socket_t, std::greater<socket_t > >   sockets_curr;
        socket_t                max_socket_check;

        std::string             buff;

        // socket_t        max_socket_check;

    // private:

        bool                    is_socket_readable( socket_t socket );
        bool                    is_socket_writable( socket_t socket );

        void                    set_socket_read_checkable( socket_t socket );
        void                    set_socket_write_checkable( socket_t socket );

        void                    unset_socket_read_checkable( socket_t socket );
        void                    unset_socket_write_checkable( socket_t socket );

    // private:

    //     void            response_client();
        void                    request_client( Client& client );
        void                    response_client( Client& client );

        void                    new_clients();

        void                    serve_clients();

        void                    multi_plexing();


    private:
        void                    lunch_servers();

    public:
        void                    add_server( const Server& server );

    public:
        void                    init_error_pages_default();
        void                    init_mimes_types();
        
        const error_pages_t&    get_error_pages_default();
        const mimes_types_t&    get_mimes_types();

        client_it_t             drop_client( Server& server, const client_it_t& client );


    public:
        void                    lunch();
    // public:
    //     WebServer();
    //     ~WebServer();

    private:
        error_pages_t           __error_pages_default;
        mimes_types_t           __mimes_types;
        servers_t               __servers;
};

#endif
// #end_of_file

