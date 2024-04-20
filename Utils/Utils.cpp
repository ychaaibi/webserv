#include "Utils.hpp"
# include <iostream>

size_t                         Utils::c = 0;

std::vector<std::string>  Utils::split_string( const std::string& str, std::string seps)
{
    std::vector<std::string>    split;
    size_t                      i, start;

    for ( i = start = 0; i<str.length(); i++ )
    {
        if ( seps.find(str[i]) != std::string::npos )
        {
            if (i > start)
                split.push_back( str.substr( start, i - start ) );
            start = i + 1;
        }
    }
    if ( i > start )
        split.push_back( str.substr( start, i - start ) );
    return (split);
}

bool                 Utils::is_num( const std::string& str )
{
    for ( size_t i=0; i<str.length(); i++)
    {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

std::string          Utils::to_string( size_t num )
{
    std::ostringstream convert;

    convert << num;
    return ( convert.str() );
}

size_t               Utils::s_to_size_t( std::string str )
{
    size_t              ret;
    std::stringstream   ss( str );

    ss >> ret;
    return (ret);
}

int                  Utils::count_reps( std::string s, char c )
{
    int count = 0;

    for ( size_t i=0; i<s.length(); i++ )
        count += ( s[i] == c );
    return ( count );
}

bool                Utils::valid_url( const std::string url )
{
    for ( size_t i=0; i<url.length(); i++)
        if ( url.find_first_not_of( VALID_URL_CHAR ) != std::string::npos )
            return ( false );
    return ( true );
}

int                 Utils::hex( char c )
{
    return ( std::string( "0123456789abcdef" ).find( c ) );
}

std::string         Utils::decode_url( const std::string url )
{
    std::string res;

    for ( size_t i=0; i<url.length(); i++ )
    {
        if ( url[i] == '+' )
            res += ' ';
        else if ( url[i] == '%' )
        {
            res += char( hex( url[i + 1] ) * 16 + hex( url[i + 2] ) );
            i += 2;
        }
        else
            res += url[i];
    }
    return ( res );
}


std::string         Utils::extention( std::string path )
{
    if ( path.find_last_of(".") == std::string::npos )
        return ( "" );
        
    return ( path.substr( path.find_last_of(".") ) );
}

std::string         Utils::name_file( std::string ext )
{
    return ( Utils::to_string(c++) + ext );
}

size_t                 Utils::to_hex( std::string hx )
{
    size_t  val = 0;

    for ( size_t i=0; i<hx.length(); i++ )
        val = val * 16 + hex( hx[i] );
    return ( val );
}

time_t                  Utils::get_curr_time()
{
    time_t  seconds = time( NULL );

    return ( seconds );
}
