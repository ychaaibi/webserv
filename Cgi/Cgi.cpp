#include "Cgi.hpp"

Cgi::Cgi( Request& req) : __request( req )
{
    __res       = "";
    __cgi_level = PREFACE_CGI;
    __clone_0   = -1;
    __pid       = -1;

}

Cgi::~Cgi()
{
    if ( __pid != -1 )
        kill( __pid, SIGTERM );
}

pid_t                       Cgi::get_pid() const
{
    return ( __pid );
}


std::string                 Cgi::get_cgi_binary( std::string ext )
{
    if ( ext == ".php" and __request.get_location()->is_cgi_existed_php() )
        return ( "/nfs/homes/ychaaibi/Desktop/last/Cgi-Bin/php-cgi" );
    else if ( ext == ".py" and __request.get_location()->is_cgi_existed_py() )
        return ( "/nfs/homes/ychaaibi/Desktop/last/Cgi-Bin/python-cgi" );
    else
        return ( "" );
            
}

char        **Cgi::get_cgi_args()
{
    __args_cgi[0] = strdup(__cgi_binary.c_str());
    __args_cgi[1] = strdup(__request.get_resource().c_str());
    __args_cgi[2] = NULL;
    
    return ( __args_cgi );
}

char        **Cgi::get_cgi_envs()
{
    __env.push_back( std::string( "REQUEST_METHOD=" ) + __request.get_real_method() );
    __env.push_back( std::string( "QUERY_STRING=" ) + __request.get_query() );
    __env.push_back( "SERVER_PROTOCOL=HTTP/1.1" );
    __env.push_back( "GATEWAY_INTERFACE=CGI/1.1" );
    __env.push_back( "REDIRECT_STATUS=200" );
    __env.push_back( std::string("REMOTE_ADDR=" ) + __request.get_server_network_info().get_ip_address() );
    __env.push_back( std::string("SERVER_PORT=" ) + __request.get_server_network_info().get_port( ) );
    __env.push_back( std::string("PATH_INFO=" + __request.get_resource() ) );
    __env.push_back( std::string("SCRIPT_FILENAME=" + __request.get_resource() ) );
    __env.push_back( std::string("SCRIPT_NAME="+ __request.get_resource() ) );
    
    if ( __request.get_headers().find( "Content-Type" ) != __request.get_headers().end() )
        __env.push_back( std::string( "CONTENT_TYPE" ) + "=" + __request.get_headers().find( "Content-Type" )->second );

    if ( __request.get_headers().find( "Content-Length" ) != __request.get_headers().end() )
        __env.push_back( std::string( "CONTENT_LENGTH" ) + "=" + __request.get_headers().find( "Content-Length" )->second );

    if ( __request.get_headers().find( "Cookie" ) != __request.get_headers().end() )
        __env.push_back( std::string( "HTTP_COOKIE" ) + "=" + __request.get_headers().find( "Cookie" )->second );

    if ( __request.get_headers().find( "User-Agent" ) != __request.get_headers().end() )
        __env.push_back( std::string( "HTTP_USER_AGENT" ) + "=" + __request.get_headers().find( "User-Agent" )->second );

    for (size_t i=0; i<__env.size(); i++)
        __env_cgi[i] = strdup( __env[i].c_str() );

    __env_cgi[ __env.size() ] = NULL;

    return ( __env_cgi );
}

void        Cgi::preface_cgi()
{
    __extention     = Utils::extention( __request.get_resource() );
    __cgi_binary    = get_cgi_binary( __extention );


    if ( __cgi_binary == "" )
    {
        __request.set_error( 404 );
        return ;
    }
    if ( pipe( __pipe ) == -1 )
        __request.set_error( 500 );

    __pid = fork();
    if ( !__pid )
    {
        int __fd_cgi;

        if ( __request.get_real_method() == POST )
        {
            __fd_cgi = open( __request.get_cgi_body_path().c_str(), O_RDONLY );

            if (__fd_cgi == -1)
            {
                close( __pipe[0] );
                close( __pipe[1] );
                exit(1);
            }
            
            close( __pipe[0] );
            close( __pipe[1] );
            dup2( __fd_cgi, 0);
            close( __fd_cgi );
        }
        else
        {
            dup2( __pipe[1], 1 );

            close( __pipe[0] );
            close( __pipe[1] );
        }

        execve( __cgi_binary.c_str(), get_cgi_args(), get_cgi_envs() );

        exit( ERROR_STATUS );
    }

    if ( __clone_0 == -1)
        __clone_0 = dup( 0 );

    dup2( __pipe[0], 0 );

    close( __pipe[1] );
    close( __pipe[0] );
    wait_cgi();
}


void        Cgi::wait_cgi()
{

    __cgi_level = WAIT_CGI;

    waitpid( __pid, &__exit_status, WNOHANG );

    if ( !WIFEXITED( __exit_status ) )
        return ;

    if ( WEXITSTATUS( __exit_status ) )
        if ( __exit_status )
            __request.set_error( 500 );

    read_cgi();
}

void    Cgi::read_cgi( )
{
    __res = "";

    switch ( __cgi_level )
    {
        case WAIT_CGI:
            __res = "HTTP/1.1 200 OK\r\n";
            __bytes_readed = read( 0, __readed, 2000 );
            break ;
            
        case READ_CGI:
            __bytes_readed = read( 0, __readed, 2000 );
            break ;
    }
    
    if ( !__bytes_readed )
    {
        if ( !__request.get_cgi_body_path().empty() )
            remove( __request.get_cgi_body_path().c_str() );

        dup2( __clone_0, 0 );
        throw -1;
    }

    __readed[ __bytes_readed ] = 0; 
    __res += std::string( __readed );

    __cgi_level = READ_CGI;
}

std::string Cgi::call_cgi()
{
    __res = "";

    switch ( __cgi_level )
    {
        case PREFACE_CGI:
            preface_cgi();
            break ;
        case WAIT_CGI:
            wait_cgi();
            break ;
        case READ_CGI:
            read_cgi();
            break ;
    }

    return ( __res );
}

