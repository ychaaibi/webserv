#include "Client.hpp"

# include <iostream>

int         Client::get_method_type() const
{
    if ( __request.get_method() == "GET" )
        return ( GET_METHOD );
    else if ( __request.get_method() == "POST" )
        return ( POST_METHOD );
    else
        return ( DELETE_METHOD );
}

const std::string* Client::response()
{
    switch ( get_method_type() )
    {
        case GET_METHOD:
            return ( __response.__get() );
        case DELETE_METHOD:
            return ( __response.__delete() );
    }
    return ( NULL );
}

void    Client::request( const std::string& added_buff )
{
    std::string line;

    __request.update_buffer( added_buff );
    try
    {
        switch ( __request_level )
        {
            case URL_LEVEL:
                __request_level = __request.handle_url( __server.get_locations() );
                
                __response.set_delete_response( __request, __server.get_server_info() );
                __response.set_get_response( __request, __server.get_server_info() );
                __request.set_post_request( );
                if ( __request_level == URL_LEVEL )
                    return ;


            case HEADER_LEVEL:
                __request_level = __request.handle_header();

                if ( __request_level != BODY_LEVEL )
                    break ;

            case BODY_LEVEL:
                __request_level = __request.handle_body();
        }
    }
    catch ( int e )
    {
        if ( !__response.get_get_response() )
            __response.set_get_response( __request, __server.get_server_info() );
        __request_level = RESPONSE_LEVEL;
    }
}


const socket_t& Client::get_socket() const
{
    return ( __socket );
}

const int&      Client::get_request_level() const
{
    return ( __request_level );
}

void            Client::set_socket( socket_t socket )
{
    __socket = socket;
}

Client::Client( const Server& server ) :
    __request       ( server.get_server_network_info(), server.get_server_info() ),
    __server        ( server ),
    __request_level ( URL_LEVEL )
{
}

Client::Client( const Client& client ) :
    __socket        ( client.__socket ),
    __request       ( client.__request ),
    __server        ( client.__server ),
    __request_level ( client.__request_level ),
    __time          ( client.__time )
{
}

Client&  Client::operator=( const Client& client )
{
    __socket        = client.__socket;
    __request       = client.__request;
    __request_level = client.__request_level;
    __time          = client.__time;

    return ( *this );
}

Client::~Client()
{

}

bool            Client::get_end_response() const
{
    return ( __response.get_get_response()->get_end_response() );
}

const Response& Client::get_response() const
{
    return ( __response );
}

const Request&  Client::get_request() const
{
    return ( __request );
}

void            Client::set_time()
{
    __time  = Utils::get_curr_time();
}

time_t          Client::get_time() const
{
    return ( __time );
}
