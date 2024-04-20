#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../Location/Location.hpp"
# include "../Utils/Utils.hpp"
# include "../Get/Get.hpp"
# include "../ServerNetworkInfo/ServerNetworkInfo.hpp"
# include "../ServerInfo/ServerInfo.hpp"
# include "../Post/Post.hpp"

# include <vector>
# include <list>
# include <map>

# define URL_LEVEL 0
# define HEADER_LEVEL 1
# define BODY_LEVEL 2
# define RESPONSE_LEVEL 3
# define READING_HEADER true

# define GET "GET"
# define POST "POST"
# define DELETE "DELETE"

# define CRLF "\r\n"
# define NEXT_LINE 1

# define CGI_SIGN 1000


typedef int                                     status_t;
typedef std::string                             url_t;
typedef std::string                             query_t;
typedef std::string                             cookies_t;
typedef std::string                             resource_t;
typedef std::string                             header_t;
typedef std::string                             method_t;
typedef std::map<std::string, std::string>      headers_t;
typedef const Location*                         location_const_ptr_t;
typedef std::vector<Location>                   locations_t;
typedef std::vector<Location>::iterator         locations_const_it_t;
typedef std::vector<Location>::const_iterator   locations_it_t;
typedef Location*                               location_ptr_t;


class Post;

class Request
{
    private:
        std::string                         __buff;
        headers_t                           __headers;
        query_t                             __query;
        url_t                               __url;
        cookies_t                           __cookies;
        resource_t                          __resource;
        method_t                            __method;
        location_const_ptr_t                __location;
        status_t                            __error_set; // 0 serve __error_page;
        status_t                            __header_set; // 0 201 200
        const ServerNetworkInfo&            __server_network_info;
        const ServerInfo&                   __server_info;
        std::string                         __cgi_body_path;
        method_t                            __real_method;
        Post                                *__post_request;

    private:
        class crlf_not_found : std::exception
        {
            public:
                const char* what () const throw();
        };

        class end_body : std::exception
        {
            public:
                const char *what() const throw();
        };

    public:
    //     const std::string&              get_buff() const;
        const ServerNetworkInfo&            get_server_network_info() const;
        const ServerInfo&                   get_server_info() const;
        const headers_t&                    get_headers() const;
        const query_t&                      get_query() const;
        const url_t&                        get_url() const;
        const cookies_t&                    get_cookies() const;
        const resource_t&                   get_resource() const;
        const method_t&                     get_method() const;
        const location_const_ptr_t&         get_location() const;

        void                                set_resource( const std::string resource );
        void                                set_method( const std::string method );

    public:
        location_const_ptr_t                find_location( const locations_t& locations ) const;


        int                                 handle_url( const locations_t& locations );
        int                                 handle_header();
        int                                 handle_body();

        url_t                               read_url();
        header_t                            read_header();


        void                                update_buffer( const std::string& added_buff );

        void                                parse_url( const std::vector<std::string>& split, const locations_t& locations );

        
        void                                set_error(int err);

        status_t                            get_error();
        status_t                            get_header_set();

        void                                set_header_set(int status);    
        const std::string                   get_buff() const;
        void                                set_buff( const std::string buffer );
        const std::string                   get_header( std::string head ) const;

        void                                set_post_request();

        std::string                         get_cgi_body_path() const ;
        method_t                            get_real_method() const ;

        void                                set_cgi_body_path( std::string );
        void                                set_real_method( method_t );

    public:

        void                                add_line( std::string added_buff );
        const std::string                   get_line();

        Request( const ServerNetworkInfo&, const ServerInfo& );
        Request( const Request& );
        Request&    operator=( const Request& );
        ~Request();
};

#endif
