#include "Response.hpp"

Response::Response()
{
    __get_response      = NULL;
    __delete_response   = NULL;

}

Response::~Response()
{
    if ( __get_response )
        delete __get_response;

    if ( __delete_response )
        delete __delete_response;
}

void        Response::set_get_response( Request& req, const ServerInfo& info )
{
    __get_response      = new Get( req, info );
}

// void        set_post_response( const Request& req, const ServerInfo& info );
void        Response::set_delete_response( Request& req, const ServerInfo& info )
{
    __delete_response   = new Delete( req, info );
}

void                Response::set_buff( const std::string& buff )
{
    __buff = buff;
}

const std::string&  Response::get_buff()
{
    return ( __buff );
}

const std::string* Response::__get()
{
    try
    {
        __get_response->get_method();
    }
    catch ( int )
    {
        __get_response->set_flag( 0 );
    }
    return ( __get_response->get_buffer() );
}

const std::string* Response::__delete()
{   
    try
    {
        __delete_response->delete_method();  
    }
    catch( int )
    {
        __get_response->set_flag( 0 );
    }
    
    return ( NULL );
}

const Get         *Response::get_get_response() const
{
    return ( __get_response );
}

const Delete         *Response::get_delete_response() const
{
    return ( __delete_response );
}