#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../Request/Request.hpp"
# include "../Response/Response.hpp"
# include "../Server/Server.hpp"
# include "../Get/Get.hpp"

# define HEADER_LEVEL 1
# define BODY_LEVEL 2

# define GET_METHOD 0
# define POST_METHOD 1
# define DELETE_METHOD 2

typedef int                         socket_t;

class Server;
class Get;

class Client
{
    private:
        int                     __socket;
        Request                 __request;
        Response                __response;
        const Server&           __server;
        int                     __request_level;
        time_t                  __time;

    public:
        int                     get_method_type() const;

        const socket_t&         get_socket() const;
        void                    set_socket( socket_t );

        const int&              get_request_level() const;

        void                    request( const std::string& );
        const std::string*      response();

        const Response&         get_response() const;
        const Request&          get_request() const;


        bool                    get_end_response() const;

        void                    display_request();

        void                    set_time();
        time_t                  get_time() const;


    public:
        Client( const Server& server );
        Client( const Client& client );
        Client&  operator=( const Client& client );
        ~Client();
};

#endif
