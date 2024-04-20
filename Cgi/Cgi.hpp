#ifndef CGI_HPP
# define CGI_HPP

typedef int pipe_t;

# include "../Request/Request.hpp"
# include "../Get/Get.hpp"
# include <vector>
# include <string>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>


# define PREFACE_CGI 0
# define WAIT_CGI 1
# define READ_CGI 2
# define ERROR_STATUS 1

class Request;
class Get;

class Cgi
{
    private:
        int                             __clone_0;
        pid_t                           __pid;
        status_t                        __exit_status;
        pipe_t                          __pipe[2];
        Request&                        __request;
        std::string                     __extention;
        std::string                     __cgi_binary;
        std::string                     __res;
        int                             __cgi_level;
        std::vector<std::string>        __env;
        char*                           __args_cgi[ 4 ];
        char*                           __env_cgi[ 15 ];
        char                            __readed[ 2048 ];
        int                             __bytes_readed;


    public:
        Cgi( Request& req );
        ~Cgi();

        char                            **get_cgi_args();
        char                            **get_cgi_envs();
        
        std::string                     get_cgi_binary( std::string ext );

        pid_t                           get_pid() const;

        void                            preface_cgi();
        void                            wait_cgi();
        void                            read_cgi();
        std::string                     call_cgi();
};

#endif