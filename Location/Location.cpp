# include "Location.hpp"

Location::Location()
{
    __cgi_existed       = false;
    __root              = "/";
    __autoindex         = false;
    __cgi_existed_php   = false;
    __cgi_existed_py    = false;
}

Location::~Location()
{
}

void    Location::add_allowed_method( const std::string& method )
{
    __allowed_methods.push_back( method );
}

//redircetion

void    Location::set_autoindex( const bool& autoindex )
{
    __autoindex = autoindex;
}

void    Location::set_root( const std::string& root )
{
    __root = root;
}

void    Location::set_index( const std::string& index )
{
    __index = index;
}

void    Location::set_upload_pass ( const std::string& upload_pass )
{
    __upload_pass = upload_pass;
}

void    Location::set_location_path( const std::string& location_path )
{
    __location_path = location_path;
}


const bool&  Location::is_cgi_existed() const
{
    return ( __cgi_existed );
}


void        Location::set_cgi_existed( bool existed )
{
    __cgi_existed = existed;
}


//cgi

bool    Location::is_allowed_method( const std::string& method ) const
{
    if ( std::find( __allowed_methods.begin(), 
               __allowed_methods.end(),
               method ) != __allowed_methods.end() )
        return ( true );
    return (false);
}

//redircetion

const bool&    Location::get_autoindex() const 
{
    return ( __autoindex );
}

const std::string&      Location::get_root() const 
{
    return ( __root );
}

const std::string&      Location::get_index() const 
{
    return ( __index );
}

const std::string&      Location::get_upload_pass() const 
{
    return ( __upload_pass );
}

const std::string&      Location::get_location_path() const
{
    return ( __location_path );
}

const bool&              Location::is_cgi_existed_php() const
{
    return ( __cgi_existed_php );
}

const bool&              Location::is_cgi_existed_py() const
{
    return ( __cgi_existed_py );
}

void                    Location::set_cgi_existed_php( bool existed )
{
    __cgi_existed_php = existed;
}

void                    Location::set_cgi_existed_py( bool existed )
{
    __cgi_existed_py = existed;
}

const std::vector<std::string>&  Location::get_redirection() const
{
    return ( __redirection );
}

void                            Location::set_redirection( const std::vector<std::string> redirect )
{
    __redirection = redirect;
}

//cgi

// 404 500 403 413 413 414 505 200 201 301 302 500 