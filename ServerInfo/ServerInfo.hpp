#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include <map>
# include <string>
# include <algorithm>

typedef std::map<int,std::pair<std::string,std::string> >   error_pages_t;
typedef std::pair<std::string,std::string>                  error_page_t;

typedef std::map<std::string,std::string>                   mimes_types_t;
typedef std::string                                         mime_type_t;

typedef int                                                 status_t;

class ServerInfo
{
    private:
        error_pages_t           __error_pages_custom;
        const error_pages_t    &__error_pages_default;
        const mimes_types_t    &__mimes_types;

    public:
        void                    add_error_page( status_t status_code, const std::string error_page );
        
    public:
        const std::string       get_mime_type( std::string mime_type_t ) const;
        const error_page_t&     get_error_page_default ( status_t status_code ) const;
        const error_page_t&     get_error_page_custom ( status_t status_code ) const;


    public:
        ServerInfo( const error_pages_t& error_pages, const mimes_types_t& mimes_types );
        ServerInfo( const ServerInfo& server_info );
        ServerInfo& operator=( const ServerInfo& server_info );
        ~ServerInfo();
};

#endif
