#include "Parser.hpp"


inline bool     Parser::valid_indent_level( int level ) const
{
    return ( level >= 0 and level <= 2 );
}

inline int      Parser::get_indent_level( const std::string& line ) const
{
    for ( size_t i=0; i<line.length(); i++ )
        if ( line[i] != '\t' )
            return ( i );
    return ( line.length() );
}


inline void     Parser::parse_port(Server& server, const std::vector<std::string> &port_args )
{
    if ( IS_INFO_SET( __server_info, SERVER_PORT ))
        throw invalid_directive();

    if ( port_args.size() != 2 or
         !Utils::is_num( port_args[1] ) or
         port_args[1].length() >= 6 or
         port_args[1][0] == '0' or
         std::atoi( port_args[1].c_str() ) > 65535 )
        throw invalid_directive_arguments();
    

    server.set_port( port_args[1] );
    
    SET_INFO( __server_info, SERVER_PORT );
}

inline void     Parser::parse_host( Server& server, const std::vector<std::string>& host_args )
{
    if ( IS_INFO_SET( __server_info, SERVER_HOST ))
        throw invalid_directive();

    if ( host_args.size() != 2)
        throw invalid_directive_arguments();
    
    std::vector<std::string>    host_split = Utils::split_string( host_args[1], "." );
    if ( host_split.size() != 4 or
         Utils::count_reps( host_args[1], '.') != 3)
        throw invalid_directive_arguments();

    for ( size_t i=0; i<4; i++ )
    {
        if ( !Utils::is_num( host_split[i] ) or
             host_split[i].length() > 3 or
             std::atoi( host_split[i].c_str() ) > 255 )
             throw invalid_directive_arguments();
    }

    server.set_host( host_args[1] );
    
    SET_INFO( __server_info, SERVER_HOST );
}

inline void     Parser::parse_client_max_body_size( Server& server, const std::vector<std::string>& client_max_body_size_args )
{
    if ( IS_INFO_SET( __server_info, SERVER_CLIENT_MAX_BODY_SIZE ))
        throw invalid_directive();

    if ( client_max_body_size_args.size() != 2 or
          client_max_body_size_args[1][0] == '0' or
         !Utils::is_num(client_max_body_size_args[1]) or
          Utils::to_string( static_cast<size_t>(-1) ).length() < client_max_body_size_args[1].length() or
         ( Utils::to_string( static_cast<size_t>(-1) ) > client_max_body_size_args[1] and 
           Utils::to_string( static_cast<size_t>(-1) ).length() == client_max_body_size_args[1].length() ) )
        throw invalid_directive_arguments();
    

    server.set_client_max_body_size( Utils::s_to_size_t( client_max_body_size_args[1] ) );
    
    SET_INFO( __server_info, SERVER_CLIENT_MAX_BODY_SIZE );
}

inline void     Parser::parse_err_pages( Server& server, const std::vector<std::string>& err_pages_args )
{
    if (err_pages_args.size() < 3)
        throw invalid_directive();

    for ( size_t i=1; i<err_pages_args.size() - 1; i++ ) 
    {
        if ( !Utils::is_num( err_pages_args[i] ) )
            throw invalid_directive();
        if ( err_pages_args[i][0] == '0' )
            throw invalid_directive();
        if ( err_pages_args[i].length() == Utils::to_string( INT_MAX ).length() and 
             err_pages_args[i] > Utils::to_string( INT_MAX ) )
            throw invalid_directive();
    }

    for ( size_t i=1; i<err_pages_args.size() - 1; i++)
    {
        status_t status_code = atoi( err_pages_args[i].c_str() );

        server.set_err_page( status_code, err_pages_args.back() );
    }
}

inline void     Parser::parse_server_names( Server& server, const std::vector<std::string>& server_names_args )
{
    (void)server;
    (void)server_names_args;

    if ( IS_INFO_SET( __server_info, SERVER_SERVER_NAMES ) )
        throw invalid_directive();

    SET_INFO( __server_info, SERVER_SERVER_NAMES);
}

inline void     Parser::parse_allowed_methods( Location& location, const std::vector<std::string> &allowed_methods_args )
{
    if ( IS_INFO_SET( __location_info, LOCATION_ALLOWED_METHOD ) )
        throw invalid_directive();

    if ( allowed_methods_args.size() < 2 )
        throw invalid_directive();

    for ( size_t i=1; i<allowed_methods_args.size(); i++ )
    {
        if ( allowed_methods_args[i] != "POST" and
             allowed_methods_args[i] != "GET" and
             allowed_methods_args[i] != "DELETE" )
            throw invalid_directive();
        location.add_allowed_method( allowed_methods_args[i] );
    }

    SET_INFO( __location_info, LOCATION_ALLOWED_METHOD );
}

inline void     Parser::parse_redirection( Location& location, const std::vector<std::string> &redirection_args ) 
{
    if ( IS_INFO_SET( __location_info, LOCATION_REDIRECTION ) or 
         IS_INFO_SET( __location_info, LOCATION_CGI_PASS ) or 
         IS_INFO_SET( __location_info, LOCATION_UPLOAD_PASS) )
        throw invalid_directive();

    if ( redirection_args.size() != 3 )
        throw invalid_directive();

    std::vector<std::string>    redirecs(2);

    redirecs[0] = redirection_args[1];
    redirecs[1] = redirection_args[2];
    
    location.set_redirection( redirecs );

    SET_INFO( __location_info, LOCATION_REDIRECTION );
}

inline void     Parser::parse_autoindex( Location& location, const std::vector<std::string> &autoindex_args )
{
    if ( IS_INFO_SET( __location_info, LOCATION_AUTOINDEX ) )
        throw invalid_directive();

    if ( autoindex_args.size() != 2)
        throw invalid_directive();
    if ( autoindex_args[1] == "off" )
        location.set_autoindex( false );
    else if ( autoindex_args[1] == "on" )
        location.set_autoindex( true );
    else
        throw invalid_directive();

    SET_INFO( __location_info, LOCATION_AUTOINDEX );
}
inline void     Parser::parse_root( Location& location, const std::vector<std::string> &root_args ) 
{
    if ( IS_INFO_SET( __location_info, LOCATION_ROOT ) )
        throw ( invalid_directive() );

    if ( root_args.size() != 2 )
        throw invalid_directive();

    if ( root_args.back()[root_args.back().size() - 1] != '/' or root_args.back()[0] != '/')
        throw invalid_directive();

    location.set_root( root_args[1] );

    SET_INFO( __location_info, LOCATION_ROOT );
}
inline void     Parser::parse_index( Location& location, const std::vector<std::string> &index_args )
{
    if ( IS_INFO_SET( __location_info, LOCATION_INDEX ) )
        throw ( invalid_directive() );

    if ( index_args.size() != 2 )
        throw invalid_directive();
    
    location.set_index( index_args[1] );

    SET_INFO( __location_info, LOCATION_INDEX );
}

inline void     Parser::parse_upload_pass( Location& location, const std::vector<std::string> &upload_pass_args )
{
    if ( IS_INFO_SET( __location_info, LOCATION_REDIRECTION ) or 
         IS_INFO_SET( __location_info, LOCATION_CGI_PASS ) or 
         IS_INFO_SET( __location_info, LOCATION_UPLOAD_PASS) )
        throw invalid_directive();

    if ( upload_pass_args.size() != 2 )
        throw invalid_directive();

    if ( upload_pass_args.back()[upload_pass_args.back().size() - 1] != '/' or upload_pass_args.back()[0] != '/')
        throw invalid_directive(); 

    location.set_upload_pass( upload_pass_args[1] );

    SET_INFO( __location_info, LOCATION_UPLOAD_PASS );
}

inline void     Parser::parse_cgi_pass( Location& location, const std::vector<std::string> &cgi_pass_args )
{
    if ( cgi_pass_args.size() > 3)
        throw invalid_directive();

    if ( IS_INFO_SET( __location_info, LOCATION_REDIRECTION ) or 
         IS_INFO_SET( __location_info, LOCATION_CGI_PASS ) or 
         IS_INFO_SET( __location_info, LOCATION_UPLOAD_PASS) )
        throw invalid_directive();

    for ( size_t i=1; i<cgi_pass_args.size(); i++ )
    {
        if ( cgi_pass_args[i] != ".php" and cgi_pass_args[i] != ".py" )
            throw invalid_directive();

        if ( cgi_pass_args[i] == ".php" )
        {
            if ( location.is_cgi_existed_php() )
                throw invalid_directive();
            else
                location.set_cgi_existed_php( true );
        }

        if ( cgi_pass_args[i] == ".py" )
        {
            if ( location.is_cgi_existed_py() )
                throw invalid_directive();
            else
                location.set_cgi_existed_py( true );
        }
    }

    location.set_cgi_existed( true );

    SET_INFO( __location_info, LOCATION_CGI_PASS );
}

inline void         Parser::parse_location_level_directive(Location& location)
{
    std::vector<std::string>  args = Utils::split_string(line, WHITESPACES);
    
    line = " ";

    if ( args[0] == "allowed_methods" )
        parse_allowed_methods( location, args );
    else if ( args[0] == "return" )
        parse_redirection( location, args );
    else if ( args[0] == "autoindex" )
        parse_autoindex(location, args );
    else if ( args[0] == "root" )
        parse_root( location, args );
    else if ( args[0] == "index" )
        parse_index( location, args );
    else if ( args[0] == "upload_pass" )
        parse_upload_pass( location, args );
    else if ( args[0] == "cgi_pass" )
        parse_cgi_pass( location, args );
    else
        throw invalid_directive();
}

inline void     Parser::parse_location_level( Server& server, const std::vector<std::string>& args )
{
    Location    location;

    __location_info = 0;

    if ( args.size() != 2)
        throw invalid_directive_arguments();

    if ( args.back()[args.back().size() - 1] != '/' or args.back()[0] != '/')
        throw invalid_directive();

    location.set_location_path( args[1] );
    while ( !end_of_file )
    {

        if ( line.find_last_not_of( WHITESPACES ) == std::string::npos )
        {
            if ( !std::getline( config_file_stream, line ) )
            {
                end_of_file = true;
                break ;
            }
            continue ;
        }

        int indent_level = get_indent_level( line );
        if ( !valid_indent_level( indent_level ) or
             line[0] == SPACE)
            throw invalid_directive();

        if (indent_level != LOCATION_INDENT_LEVEL)
            break ;
        
        parse_location_level_directive( location );
    }

    server.add_location( location );
}

inline void         Parser::parse_server_level_directive( Server& server )
{
    std::vector<std::string>  args = Utils::split_string(line, WHITESPACES);

    line = " ";
    if ( args[0] == "port" )
        parse_port( server, args );
    else if ( args[0] == "host" )
        parse_host( server, args );
    else if ( args[0] == "client_max_body_size" )
        parse_client_max_body_size( server, args );
    else if ( args[0] == "error_pages" )
        parse_err_pages( server, args );
    else if ( args[0] == "server_names" )
        parse_server_names( server, args );
    else if ( args[0] == "location" )
        parse_location_level( server, args );
    else
        throw invalid_directive();
}

inline void     Parser::parse_server_level()
{
    Server      server( __web_server.get_error_pages_default(), __web_server.get_mimes_types() );
    
    __server_info = 0;

    while ( !end_of_file )
    {
        if ( line.find_last_not_of( WHITESPACES ) == std::string::npos )
        {
            if ( !std::getline( config_file_stream, line ) )
                end_of_file = true;
            continue ;
        }
        
        int indent_level = get_indent_level( line );
    
        if ( !valid_indent_level( indent_level ) or 
              indent_level == LOCATION_INDENT_LEVEL or
              line[0] == SPACE)
            throw invalid_indent();

        if ( indent_level == CONFIG_INDENT_LEVEL)
            break ;

        parse_server_level_directive( server );
    }

    if ( __host_port.find( server.get_host() + ":" + server.get_port() ) != __host_port.end() )
        throw duplicate_host_port();
    __host_port.insert( server.get_host() + ":" + server.get_port());
    __web_server.add_server( server );
}

inline void         Parser::parse_config_level_directive()
{
    std::vector<std::string>  args = Utils::split_string( line,WHITESPACES );

    line = " ";
    if ( args.size() == 1 and args[0] == "server" )
        parse_server_level();
    else
        throw invalid_directive();

}

inline void     Parser::parse_config_level()
{
    while ( !end_of_file )
    {

        if ( line.find_last_not_of( WHITESPACES ) == std::string::npos )
        {
            if ( !std::getline( config_file_stream, line ) )
                end_of_file = true;
            continue ;
        }

        if (get_indent_level( line ) != CONFIG_INDENT_LEVEL or
            line[0] == SPACE)
            throw invalid_indent();
        
        parse_config_level_directive();
    }
}


inline void    Parser::parse_config_file( const std::string& config_file_name )
{
    end_of_file = false;
    line = "";
    config_file_stream.open( config_file_name.c_str() );

    parse_config_level();
}

WebServer&      Parser::get_web_server( const std::string& config_file_name )
{
    __web_server.init_error_pages_default();
    __web_server.init_mimes_types();
    parse_config_file( config_file_name );
    
    return ( __web_server );
}


const char*     Parser::invalid_indent::what() const throw()
{
    return ( "WebServer--Error Config File : Invalid indent " );
}

const char*     Parser::invalid_directive::what() const throw()
{
    return ( "WebServer--Error Config File : Invalid directive " );
}

const char*     Parser::invalid_directive_arguments::what() const throw()
{
    return ( "WebServer--Error Config File : Invalid directive arguments " );
}

const char*     Parser::duplicate_host_port::what() const throw()
{
    return ( "WebServer--Error Config File : duplicate_host_port" );
}