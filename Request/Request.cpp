#include <iostream>
#include "Request.hpp"

Request::Request( const ServerNetworkInfo& server_network_info, const ServerInfo& server_info ) :
                __server_network_info (server_network_info ), __server_info( server_info )
{
    __location      = NULL;
    __error_set     = 0;
    __header_set    = 0;
    __post_request  = NULL;
}

Request::Request( const Request& request) :
    __buff                  ( request.__buff ),
    __headers               ( request.__headers ),
    __query                 ( request.__query ),
    __url                   ( request.__url ),
    __cookies               ( request.__cookies ),
    __resource              ( request.__resource ),
    __method                ( request.__method ),
    __location              ( request.__location ),
    __error_set             ( request.__error_set ),
    __header_set            ( request.__header_set ),
    __server_network_info   ( request.__server_network_info ),
    __server_info           ( request.__server_info ),
    __post_request          ( request.__post_request )
{
}

Request&    Request::operator=( const Request& request )
{
    __buff                      = request.__buff;
    __headers                   = request.__headers;
    __query                     = request.__query;
    __url                       = request.__url;
    __cookies                   = request.__cookies;
    __resource                  = request.__resource;
    __method                    = request.__method;
    __location                  = request.__location;
    __error_set                 = request.__error_set;
    __header_set                = request.__header_set;
    __post_request              = request.__post_request;
    return ( *this );
}


Request::~Request()
{
    delete __post_request;
}

location_const_ptr_t    Request::find_location( const locations_t& locations ) const
{
    location_const_ptr_t    location_searched = NULL;

    for ( std::vector<Location>::const_iterator loc = locations.begin();
                                loc != locations.end();
                                loc++ )
    {
        if ( loc->get_location_path().length() > __url.length() )
            continue ;
        
        if ( !__url.compare(0, loc->get_location_path().length(), loc->get_location_path() ) )
            if ( !location_searched
                    or location_searched->get_location_path().length() < loc->get_location_path().length() )
                location_searched = &(*loc);
    }
    return ( location_searched );
}


void    Request::parse_url( const std::vector<std::string>& split, const locations_t& locations )
{
    if ( !Utils::valid_url( split[1] ) )
        set_error( 400 );

    __query             = split[1].substr( split[1].find( "?" ) + 1 ); 
    __url               = split[1].substr( 0, split[1].length() - __query.length() - !__query.empty() );
    __url               = Utils::decode_url( __url );
    __location          = find_location( locations );

    if ( !__location )
        set_error( 404 );
    else if ( __location->get_redirection().size() )
        set_header_set( atoi( __location->get_redirection()[0].c_str() ) );
    else if ( !__location->is_allowed_method( __method ) )
        set_error( 405 );
    else if ( split[1].length() > 2048 )
        set_error( 414 );
    else if ( split[2] != "HTTP/1.1" )
        set_error( 505 );
}

int Request::handle_url( const locations_t& locations )
{
    try
    {
        url_t   url       = read_url();
        split_t url_split = Utils::split_string( url, " " );

        __method          = __real_method   = url_split[0];

        if ( __method != "GET" and __method != "POST" and __method != "DELETE" )
            set_error( 501 );
        parse_url( url_split, locations );

        if ( __error_set or __header_set )
            return ( RESPONSE_LEVEL );

        __resource      = __location->get_root();
        __resource     += __url.substr( __location->get_location_path().length() );

        return ( HEADER_LEVEL );
    }
    catch( const crlf_not_found& )
    {
    }
    return ( URL_LEVEL );
}

int Request::handle_header()
{
    try
    {
        while ( READING_HEADER )
        {
            header_t    header       = read_header();
            split_t     header_split = Utils::split_string( header, ":" );

            if ( header.empty() )
            {
                if ( __method == "GET" or __method == "DELETE" )
                    return ( RESPONSE_LEVEL );
                else
                {
                    return ( BODY_LEVEL );
                }
            }

            __headers[header_split[0]] = header_split[1].substr(1);
        }
    }
    catch ( const crlf_not_found& )
    {
    }
    return ( HEADER_LEVEL );
}

int Request::handle_body()
{
    try
    {

        __post_request->post_method( __buff );
        __buff = "";
    }
    catch ( const end_body& )
    {
        return ( RESPONSE_LEVEL );
    }
    catch ( int status )
    {
        throw status ;
    }

    return ( BODY_LEVEL );
}

void        Request::update_buffer( const std::string& added_buff )
{
    __buff += added_buff;
}

header_t    Request::read_header()
{
    size_t      pos    = __buff.find( CRLF );
    std::string line;

    if ( pos == std::string::npos)
        throw crlf_not_found();
    else
    {
        line    = __buff.substr( 0, pos );
        __buff  = __buff.substr( pos + 2 );

        return ( line );
    }
    return ("");
}

url_t        Request::read_url()
{
    size_t      pos = __buff.find( CRLF );
    std::string line;

    if ( pos == std::string::npos)
        throw crlf_not_found();
    else
    {
        line    = __buff.substr( 0, pos );
        __buff  = __buff.substr( pos + 2 );

        return ( line );
    }
    return ( "" );
}


// const std::string&      get_buff()
// {
//     return (  __buff );
// }

const headers_t&        Request::get_headers() const
{
    return ( __headers );
}

const query_t&          Request::get_query() const
{
    return ( __query );
}

const url_t&            Request::get_url() const
{
    return ( __url );
}

// const cookies_t&        get_cookies()
// {
//     return ( __cookies );
// }

const resource_t&               Request::get_resource() const
{
    return ( __resource );
}

const method_t&                 Request::get_method() const
{
    return ( __method );
}

const location_const_ptr_t&     Request::get_location() const
{
    return ( __location );
}

void                            Request::set_resource( const std::string resource )
{
    __resource = resource;
}

const char* Request::crlf_not_found::what() const throw()
{
    return ( "" );
}


const ServerNetworkInfo&            Request::get_server_network_info() const
{
    return ( __server_network_info );
}

const ServerInfo&                   Request::get_server_info() const
{
    return ( __server_info );
}

const std::string                   Request::get_header( std::string head ) const
{
    if ( __headers.find( head ) == __headers.end() )
        return ( "" );
    return ( __headers.find( head )->second );
}


void                            Request::set_error(int err)
{
    __error_set = err ;
    __method = GET;
    throw err;
}

status_t                        Request::get_error()
{
     return __error_set ;
}

status_t                        Request::get_header_set()
{
    return __header_set ;
}

void                            Request::set_header_set(int status)
{
    __header_set = status ; 
    __method = GET;
    throw status;
}

void                            Request::set_method( const std::string method )
{
    __method = method;
}

const std::string               Request::get_buff() const
{
    return ( __buff );
}


void                            Request::set_buff( const std::string buffer )
{
    __buff = buffer;
}

const char*  Request::end_body::what() const throw()
{
    return ( "end_body" );
}

void        Request::set_post_request( )
{
    __post_request = new Post( this );
}

std::string Request::get_cgi_body_path() const 
{
    return ( __cgi_body_path );
}

method_t    Request::get_real_method() const 
{
    return ( __real_method );
}


void        Request::set_cgi_body_path( std::string cgi_body_path )
{
    __cgi_body_path = cgi_body_path;
}

void        Request::set_real_method( method_t real_method )
{
    __real_method = real_method;
}
