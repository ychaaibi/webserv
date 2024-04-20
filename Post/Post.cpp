#include "Post.hpp"

Post::Post( Request* req_ptr ) : __request ( req_ptr )
{
    __level             = POST_LEVEL;
    __read_body_length  = 0;
    __post_file_name    = "";
    __size_chunked      = 0;
    __size_readed       = 0;
    __chunk_body_level  = READING_SIZE_CHUNK;
    __buff              = std::string( "", 0 );
    // __post_stream_upload.clear();
}

Post::~Post()
{
}

int     Post::check_resource(std::string res)
{
    struct stat pathinfo;

    if(stat(res.c_str(), &pathinfo) != 0)
        return _PATH_NOT_EXIST;
    if (S_ISDIR(pathinfo.st_mode))
        return _DIRECTORY;
    if (S_ISREG(pathinfo.st_mode))
        return _FILE;
    return -1;
}

void    Post::end_upload()
{
    __post_stream_upload.close();
    switch ( int( __request->get_location()->is_cgi_existed() ) )
    {
        case CGI:
            char    path[1024 ];

            __request->set_cgi_body_path( std::string( getcwd( path, 1024 ) ) + "/" + __post_file_name ) ;
            __request->set_method( GET );
            throw END_BODY;
        
        default:
            __request->set_header_set( 201 );
    }
}

void    Post::length_upload()
{
    __level         = POST_LENGTH_UPLOAD;

    if ( __request->get_headers().find( "Content-Length" ) != __request->get_headers().end() )
        __body_length   = Utils::s_to_size_t( __request->get_headers().find( "Content-Length" )->second );
    else
        __body_length   = 0;

    if ( __request->get_server_network_info().get_client_max_body_size() < __body_length )
    {
        __post_stream_upload.close();
        remove( __post_file_name.c_str() );
        __request->set_error( 413 );
    }

    __read_body_length += __buff.length();
    __post_stream_upload.write( __buff.c_str(), __buff.length() );

    if ( __read_body_length >= __body_length )
        end_upload();

    __buff = "";
}


void    Post::chunked_upload()
{
    size_t  find_crlf;

    __level = POST_CHUNKED_LEVEL;


    switch ( __chunk_body_level )
    {
        case READING_SIZE_CHUNK:
            if ( __buff[0] == '\r')
                find_crlf = __buff.find( CRLF , 2 );
            else
                find_crlf = __buff.find( CRLF, 0 );

            if ( find_crlf == std::string::npos )
                return ;

            if ( __buff[0] == '\r' )
                __size_chunked  = Utils::to_hex( __buff.substr( 2, find_crlf - 2 ) ) ;
            else
                __size_chunked  = Utils::to_hex( __buff.substr( 0, find_crlf ) );

            __buff          = __buff.substr( find_crlf + 2 );

        case CHUNK_READ:
            while ( __size_chunked and __size_chunked - __size_readed <= __buff.length() )
            {
                __read_body_length      += __size_chunked - __size_readed;
                
                if ( __read_body_length > __request->get_server_network_info().get_client_max_body_size() )
                {        
                    __post_stream_upload.close();
                    remove( __post_file_name.c_str() );
                    __request->set_error( 413 );
                }

                __post_stream_upload.write( __buff.substr(0, __size_chunked - __size_readed).c_str(), __size_chunked - __size_readed );
                
                __buff                  = __buff.substr( __size_chunked - __size_readed );

                find_crlf               = __buff.find( CRLF, 2 );
                __size_readed           = 0;

                if ( find_crlf == std::string::npos )
                {
                    __chunk_body_level = READING_SIZE_CHUNK;
                    return ;
                }
                
                __size_chunked  = Utils::to_hex( __buff.substr( 2, find_crlf - 2 ) );
                __buff          = __buff.substr( find_crlf + 2 ) ;
            }

            if ( !__size_chunked )
                end_upload();


            __chunk_body_level  = CHUNK_READ;
            __size_readed       += __buff.length();
            __read_body_length  += __buff.length();

            __post_stream_upload.write ( __buff.c_str(), __buff.length() );
            __buff              = "";
    }
}

void    Post::preface_upload()
{
    __post_file_name        = __request->get_location()->get_upload_pass() + "_" + Utils::name_file( __request->get_server_info().get_mime_type( __request->get_header("Content-Type") ) );

    __post_stream_upload.open( __post_file_name.c_str(), std::ios::in | std::ios::out | std::ios::trunc );

    if ( __request->get_header( "Transfer-Encoding" ) == "chunked" )
        chunked_upload();
    else
        length_upload();
}

void    Post::post_method( std::string buff )
{
    __buff += buff;


    switch ( __level )
    {
        case POST_LEVEL:
            switch ( __request->get_location()->get_upload_pass().length() )
            {
                case    EMPTY:
                    switch ( check_resource( __request->get_resource() ) )
                    {
                        case _PATH_NOT_EXIST:
                            __request->set_error( 404 );

                        case _DIRECTORY:
                            __request->set_resource( __request->get_resource() + __request->get_location()->get_index() );

                            switch ( check_resource( __request->get_resource() ) )
                            {
                                case _PATH_NOT_EXIST:
                                    __request->set_error( 404 );
                                case _DIRECTORY:
                                    __request->set_error( 403 );
                            }
                    }
                    preface_upload();
                    break ;

                default:
                    switch ( check_resource( __request->get_location()->get_upload_pass() ) )
                    {
                        case _DIRECTORY:
                            preface_upload();
                            break ;

                        default:
                            __request->set_error( 403 );
                    }
            }
            break ;

        case POST_CHUNKED_LEVEL:
            chunked_upload();
            break ;
        
        case POST_LENGTH_UPLOAD:
            length_upload();
            break ;
    }
}