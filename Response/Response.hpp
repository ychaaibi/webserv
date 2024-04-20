#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Utils/Utils.hpp"
# include "../Get/Get.hpp"
// # include "../Post/Post.hpp"
# include "../Delete/Delete.hpp"
# include "../ServerInfo/ServerInfo.hpp"
# include "../Request/Request.hpp"
# include "../Post/Post.hpp"

# include <map>
# include <exception>

class Post;
class Get;
class Request;
class ServerInfo;
class Delete;

typedef std::map<std::string,std::string>    headers_t;

class Response
{
    private:
        std::string         __buff;
        headers_t           __headers;
        // bool                __end_response;

        Get*                __get_response;
        Delete*             __delete_response;

    public:
        void                set_get_response( Request& req, const ServerInfo& info );
        void                set_delete_response( Request& req, const ServerInfo& info );

        const std::string*  __get();
        const std::string*  __delete();

        void                set_end_response( bool end_response );
        bool                get_end_response() const;
        void                set_buff( const std::string& buff );

        const Get           *get_get_response() const;
        const Delete        *get_delete_response() const; 

        const std::string&  get_buff();

    public:
        Response();
        ~Response();
};

#endif
