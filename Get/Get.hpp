#pragma once

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <cerrno>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../ServerInfo/ServerInfo.hpp"
#include "../Cgi/Cgi.hpp"

# define  AUTO_INDEX 2000

class ServerInfo;
class Request;
class Response;
class Cgi;

class Get{
    private:
        Request&            req;
        const ServerInfo&   inf;
        std::ifstream       file;
        std::string         _header;
        std::string         *_buffer;
        std::string         _saved_path;
        std::string         _new_path;
        bool                __end_response;
        //std::string         _content_type;
        int                 flag;
        int                 buffersize;
        bool                __auto_index;
        Cgi                 *cgi;


    public:
        Get(Request &passed, const ServerInfo& info);
        void            get_method();
        bool            dir_have_permission(std::string path);
        int             get_resource_type(std::string res);
        std::string     send_chunk();
        void            set_header(int status, std::string str);
        void            set_auto_index(bool i);
        const std::string*     get_buffer() const;
        bool            get_auto_index();
        void            fill_header(std::string const header);
        void            set_title_error(int status);
        void            list_dir(std::string path);
        void            chunk_file(std::string path, int status);
        long            get_content_lenght();
        std::string     generate_title(int status);
        std::string     get_content_type();
        //////
        void            check_error_map(int status);
        void            status_line();
        void            header_type();
        void            header_length();
        void            set_flag(int flag);
        int             get_flag();
        std::string     get_header();
        const Cgi*      get_cgi() const;

        /////////////////////////////////////////////////////////////////
        void                   set_new_path(std::string path);
        std::string            get_new_path();
        void                   set_saved_path(std::string path);
        std::string            get_saved_path();
        std::string            set_new_url(std::string str);
        /////////////////////////////////////////////////////////////////
        void                set_end_response( bool end_response );
        bool                get_end_response() const;

        ~Get();
};