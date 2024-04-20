#ifndef Parser_HPP
# define Parser_HPP

# include "../WebServer/WebServer.hpp"
# include "../Utils/Utils.hpp"
# include <fstream>
# include <exception>
# include <climits>
# include <string.h>
# include <stdlib.h>

typedef std::ifstream    config_stream_t;

# define    CONFIG_INDENT_LEVEL 0
# define    SERVER_INDENT_LEVEL 1
# define    LOCATION_INDENT_LEVEL 2

# define    TAB '\t'
# define    SPACE ' '

# define    WHITESPACES "\t "

# define    SERVER_HOST 0
# define    SERVER_PORT 1
# define    SERVER_CLIENT_MAX_BODY_SIZE 2
# define    SERVER_SERVER_NAMES 3


# define    LOCATION_REDIRECTION 0
# define    LOCATION_UPLOAD_PASS 1
# define    LOCATION_CGI_PASS 2
# define    LOCATION_ALLOWED_METHOD 3
# define    LOCATION_INDEX 4
# define    LOCATION_AUTOINDEX 5
# define    LOCATION_ROOT 6

# define    IS_INFO_SET( info, level ) ( ( info >> level ) & 1 )

# define    SET_INFO( info, level ) ( info = ( info | ( 1 << level ) ) )

class Parser
{
    private:
        int                     __location_info;
        int                     __server_info;
        std::set<std::string>   __host_port;
    
    private:
        class invalid_indent : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class invalid_directive : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class invalid_directive_arguments : public std::exception
        {
            public:
                const char* what() const throw();
        };

        class duplicate_host_port : public std::exception
        {
            public:
                const char* what() const throw();
        };

    // Config File Parsing

    private:
        config_stream_t config_file_stream;
        std::string     line;
        bool            end_of_file;

    private:

        inline bool     valid_indent_level( int level ) const;
        inline int      get_indent_level( const std::string& line ) const;


        inline void     parse_port( Server& server, const std::vector<std::string> &port_args );
        inline void     parse_host( Server& server, const std::vector<std::string>& host_args );
        inline void     parse_client_max_body_size( Server& server, const std::vector<std::string>& clien_max_body_size_args );
        inline void     parse_err_pages( Server& server, const std::vector<std::string>& err_pargs );
        inline void     parse_server_names( Server& server, const std::vector<std::string>& server_names_args );

        inline void     parse_allowed_methods( Location& location, const std::vector<std::string> &allowed_methods_args );
        inline void     parse_redirection( Location& location, const std::vector<std::string> &redirection_args );
        inline void     parse_autoindex( Location& location, const std::vector<std::string> &autoindex_args );
        inline void     parse_root( Location& location, const std::vector<std::string> &root_args );
        inline void     parse_index( Location& location, const std::vector<std::string> &index_args );
        inline void     parse_upload_pass( Location& location, const std::vector<std::string> &upload_pass_args );
        inline void     parse_cgi_pass( Location& location, const std::vector<std::string> &cg_pass_args );
        
        inline void     parse_config_level_directive();
        inline void     parse_server_level_directive( Server& server );
        inline void     parse_location_level_directive( Location& location );

        inline void     parse_location_level( Server& server, const std::vector<std::string>& args );
        inline void     parse_server_level();
        inline void     parse_config_level();

        void            parse_config_file( const std::string& config_file_name );
    
    
    // Mimes Types Parsing
    
    private:
        void            parse_mimes_types();

    // Mimes
    private:
        WebServer       __web_server;

    public:
        WebServer&      get_web_server( const std::string& config_file_name );
};

#endif