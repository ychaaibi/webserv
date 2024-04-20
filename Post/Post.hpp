#ifndef POST_HPP
# define POST_HPP

# define FIRST_CHUNK -1
# define READ_CHUNK 0

# define CHUNK_FIRST 0
# define CHUNK_LAST 1
# define CHUNK_READ 2
# define READING_SIZE_CHUNK 3

# define CRLF "\r\n"

# define EMPTY 0
# define POST_LEVEL 1
# define POST_CHUNKED_LEVEL 3
# define POST_LENGTH_UPLOAD 4
# define POST_END 5

# define CGI 1
# define CGI_SIGN 1000

# define _PATH_NOT_EXIST 0
# define _DIRECTORY 1
# define _FILE 2

# define END_BODY 2000

# include <string>
# include <fstream>
# include "../Request/Request.hpp"
# include "../Utils/Utils.hpp"

class Request;

class Post
{
    private:
        Request*            __request;
        size_t              __read_body_length;
        size_t              __body_length;
        int                 __level;
        std::string         __buff;
        std::string         __post_file_name;
        std::fstream        __post_stream_upload;
        size_t              __size_chunked;
        size_t              __size_readed;
        int                 __chunk_body_level;

    private:
        void                end_upload();
        void                length_upload();
        void                chunked_upload();
        void                preface_upload();

        int                 check_resource( std::string res );

    public:
        void                post_method( std::string buff );

        Post( Request* req );
        ~Post();
};




#endif