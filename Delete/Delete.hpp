#ifndef DELETE_HPP
# define DELETE_HPP

# include "../ServerInfo/ServerInfo.hpp"
# include "../Get/Get.hpp"
# include "../Request/Request.hpp"

# include <stdio.h>

class Request;

class Delete
{
    private:
        Request&            _req;
        const ServerInfo&   _inf;
        int flag;
    public:
        void        delete_method();
        void        delete_file(std::string path);
        void        delete_dir(std::string path);
        std::string set_new_url(std::string path);
        void        delete_file_dir();
        
        Delete( Request &passed, const ServerInfo& info );
        ~Delete();
};

#endif
