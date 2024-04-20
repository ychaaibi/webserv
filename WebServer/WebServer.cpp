#include "WebServer.hpp"

void                    WebServer::response_client( Client& client)
{
    const std::string*  send_data = client.response();

    if ( send_data == NULL or send_data->empty())
        return ;

    send_status_t       status = send( client.get_socket(), send_data->data(), send_data->size(), 0);

    if ( status == 0)
        return ;
    if (status == SEND_FAILED)
        throw multi_plexing_send_failed();
    client.set_time();
}

void                    WebServer::request_client( Client& client)
{
    char            recv_buffer[2048];

    recv_status_t   status = recv( client.get_socket(), &recv_buffer, 2048, 0 );

    if ( status == RECV_FAILED )
        throw multi_plexing_recv_failed();
    else if ( status == CONNECTION_CLOSED )
        throw multi_plexing_clear_client();
    
    client.request( std::string( recv_buffer, status ) );
    client.set_time();
}


client_it_t             WebServer::drop_client( Server& server, const client_it_t& c_it )
{
    unset_socket_read_checkable( c_it->get_socket() );
    unset_socket_write_checkable( c_it->get_socket() );

    if ( c_it->get_response().get_get_response()->get_cgi()->get_pid() != -1)
        kill( c_it->get_response().get_get_response()->get_cgi()->get_pid(), SIGKILL );

    return ( server.end_client_connection( c_it ) );
}

void                    WebServer::serve_clients()
{
    for ( std::list<Server>::iterator s_it = __servers.begin(); s_it != __servers.end(); s_it++ )
    {
        Server& server = *s_it;

        for ( client_it_t c_it = (server.get_update_clients()).begin(); c_it != (server.__clients).end(); c_it++ )
        {
            Client& client = *c_it;

            if ( is_socket_readable( client.get_socket() ) and 
                 client.get_request_level() != RESPONSE_LEVEL )
            {
                try
                {
                    request_client( client );
                }
                catch ( const std::exception& e )
                {
                    unset_socket_read_checkable( c_it->get_socket() );
                    unset_socket_write_checkable( c_it->get_socket() );
                    c_it = server.end_client_connection( c_it );
                    continue ;
                }
            }
                
            if ( is_socket_writable( client.get_socket() ) )
            {
                if ( client.get_response().get_get_response() and client.get_end_response() )
                {
                    unset_socket_read_checkable( c_it->get_socket() );
                    unset_socket_write_checkable( c_it->get_socket() );
                    c_it = server.end_client_connection( c_it );
                    continue ;
                }

                if ( client.get_request_level() == RESPONSE_LEVEL )
                {
                    try
                    {
                        response_client( client );
                    }  
                    catch ( const std::exception& e )
                    {
                        // drop_client( server, c_it );
                        unset_socket_read_checkable( c_it->get_socket() );
                        unset_socket_write_checkable( c_it->get_socket() );
                        c_it = server.end_client_connection( c_it );
                        continue ;
                    }
                }
            }

            // if ( Utils::get_curr_time() - client.get_time() >= TIMEOUT)
            // {
            //     unset_socket_read_checkable( c_it->get_socket() );
            //     unset_socket_write_checkable( c_it->get_socket() );
            //     c_it = server.end_client_connection( c_it );
            //     continue ;
            // }

        }
    }
}

void                    WebServer::new_clients()
{
    for ( std::list<Server>::iterator s_it = __servers.begin(); s_it != __servers.end(); s_it++ )
    {
        Server& server = *s_it;

        if ( is_socket_readable( server.get_socket() ) )
        {
            try
            {
                Client  new_client = server.accept_client();

                set_socket_read_checkable( new_client.get_socket() );
                set_socket_write_checkable( new_client.get_socket() );

                server.add_client( new_client );
            }
            catch( const std::exception& e )
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
}

//ExceptionsClasses

const char*             WebServer::multi_plexing_select_failed::what() const throw()
{
    return ("WebServer--Multi_plexing-- select failed :  !!" );
}

const char*             WebServer::multi_plexing_recv_failed::what() const throw()
{
    return ("WebServer--Multi_plexing-- recv failed :  !!" );
}

const char*             WebServer::multi_plexing_clear_client::what() const throw()
{
    return ("WebServer--Multi_plexing-- End connection !!");
}

const char*             WebServer::multi_plexing_send_failed::what() const throw()
{
    return ("WebServer--Multi_plexing-- recv failed :  !!" );
}


bool                    WebServer::is_socket_writable( int socket )
{
    return ( FD_ISSET( socket, &curr_sockets_writeable_set ) );
}

bool                    WebServer::is_socket_readable( int socket )
{
    return ( FD_ISSET ( socket, &curr_sockets_readable_set ) );
}

void                    WebServer::set_socket_read_checkable( socket_t socket )
{
    sockets_curr.insert( socket );

    max_socket_check = *( sockets_curr.begin() );
    FD_SET( socket, &sockets_read_set );
}

void                    WebServer::set_socket_write_checkable( socket_t socket )
{
    FD_SET( socket, &sockets_write_set );
}

void                    WebServer::unset_socket_read_checkable( socket_t socket )
{
    sockets_curr.erase( socket );

    max_socket_check = *( sockets_curr.begin() );

    FD_CLR( socket, &sockets_read_set );
    
    close( socket );
}

void                    WebServer::unset_socket_write_checkable( socket_t socket )
{
    FD_CLR( socket, &sockets_write_set );
}


void                    WebServer::multi_plexing()
{
    while ( WEB_SERVER_ON )
    {
        curr_sockets_readable_set       = sockets_read_set;
        curr_sockets_writeable_set      = sockets_write_set;

        if ( select( max_socket_check + 1, &curr_sockets_readable_set, &curr_sockets_writeable_set,  0, 0 ) < 0 )
            throw multi_plexing_select_failed();
        
        new_clients();
        serve_clients();
    }
}

void                    WebServer::lunch_servers()
{
    FD_ZERO( &sockets_read_set );
    FD_ZERO( &sockets_write_set );

    for ( std::list<Server>::iterator s_it = __servers.begin(); s_it != __servers.end(); s_it++ )
    {
        Server& server = *s_it;

        try
        {
            server.lunch();
            set_socket_read_checkable( server.get_socket() );
        }
        catch (const std::exception&)
        {
            s_it = __servers.erase( s_it );
        }
    }
}

void                    WebServer::lunch()
{
    max_socket_check = -1;
    signal( SIGPIPE, SIG_IGN);


    if ( __servers.empty() )
    {
        std::cerr << "WebServer-- Have no servers... !!" << std::endl;
        return ;
    }

    lunch_servers();
    multi_plexing();
}

void                    WebServer::add_server( const Server& server )
{
    __servers.push_back( server );
}

void                    WebServer::init_error_pages_default()
{
    __error_pages_default[101] = std::make_pair("101 Switching Protocols", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/101.html" );
    __error_pages_default[200] = std::make_pair("200 OK", "" );
    __error_pages_default[201] = std::make_pair("201 Created", "" );
    __error_pages_default[204] = std::make_pair("204 No Content", "" );
    __error_pages_default[301] = std::make_pair("301 Moved Permanently", "" );
    __error_pages_default[302] = std::make_pair("302 Found", "" );
    __error_pages_default[400] = std::make_pair("400 Bad Request", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/400.html" );
    __error_pages_default[401] = std::make_pair("401 Unauthorized", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/401.html" );
    __error_pages_default[403] = std::make_pair("403 Forbidden", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/403.html" );
    __error_pages_default[405] = std::make_pair("405 Method Not Allowed", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/405.html" );
    __error_pages_default[404] = std::make_pair("404 Not Found", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/404.html" );
    __error_pages_default[413] = std::make_pair("413 Content Too Large", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/413.html" );
    __error_pages_default[414] = std::make_pair("414 URI Too Long", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/414.html" );
    __error_pages_default[500] = std::make_pair("500 Internal Server Error", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/500.html" );
    __error_pages_default[501] = std::make_pair("501 Not Implemented", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/501.html" );
    __error_pages_default[503] = std::make_pair("503 Service Unavailable", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/503.html" );
    __error_pages_default[503] = std::make_pair("505 HTTP Version Not Supported", "/nfs/homes/ychaaibi/Desktop/last/ErrorPages/505.html" );
    // __server_info.get_error
}

void                    WebServer::init_mimes_types()
{
    std::ifstream   mimes_types_stream( "/nfs/homes/ychaaibi/Desktop/last/MimeTypes/mime.types" );
    std::string     line;

    while ( std::getline( mimes_types_stream, line ) )
    {
        split_t mime_type_split = Utils::split_string( line, WHITESPACES );

        __mimes_types[ mime_type_split[0] ] = mime_type_split[1];
        __mimes_types[ mime_type_split[1] ] = mime_type_split[0];
    }
    __mimes_types[ "" ] = "";
}

const error_pages_t&    WebServer::get_error_pages_default()
{
    return ( __error_pages_default );
}

const mimes_types_t&    WebServer::get_mimes_types()
{
    return ( __mimes_types );
}


