#include "ServerInfo.hpp"
# include <iostream>

void                    ServerInfo::add_error_page( status_t status_code, const std::string error_page_path )
{
    __error_pages_custom[ status_code ].second = error_page_path;
}

const mime_type_t       ServerInfo::get_mime_type( mime_type_t mime) const
{
    if ( __mimes_types.find( mime ) != __mimes_types.end() )
        return ( __mimes_types.find( mime )->second );
    return ( "" );
}

const error_page_t&     ServerInfo::get_error_page_default ( status_t status_code ) const
{
    return ( __error_pages_default.find( status_code )->second );
}

const error_page_t&     ServerInfo::get_error_page_custom ( status_t status_code ) const
{
    return (__error_pages_custom.find( status_code )->second);
}


ServerInfo::ServerInfo( const error_pages_t& error_pages, const mimes_types_t& mimes_types ):
            __error_pages_custom    ( error_pages ),
            __error_pages_default   ( error_pages ),
            __mimes_types           ( mimes_types )
{
    __error_pages_custom = __error_pages_default;
}

ServerInfo::ServerInfo( const ServerInfo& server_info ):
            __error_pages_custom    ( server_info.__error_pages_custom ),
            __error_pages_default   ( server_info.__error_pages_default ),
            __mimes_types           ( server_info.__mimes_types )
{
}

ServerInfo& ServerInfo::operator=( const ServerInfo& server_info )
{
    __error_pages_custom    = server_info.__error_pages_custom;
    return ( *this );
}

ServerInfo::~ServerInfo()
{

}
