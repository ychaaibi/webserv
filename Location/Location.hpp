#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <vector>
#include <string>
#include <algorithm>

class Location
{
    private:
        std::vector<std::string>    __allowed_methods;
        //redirection
        bool                        __autoindex;
        std::string                 __root;
        std::string                 __index;
        std::string                 __upload_pass;

        std::vector<std::string>         __redirection;
        std::vector<std::string>         __cgi_binary;    

        bool                        __cgi_existed;
        bool                        __cgi_existed_php;
        bool                        __cgi_existed_py;

        std::string                 __location_path;

    public:

        void                        add_allowed_method( const std::string& method );
        //redircetion
        void                        set_autoindex( const bool& autoindex );
        void                        set_root( const std::string& root );
        void                        set_index( const std::string& index );
        void                        set_upload_pass ( const std::string& upload_pass );
        //cgi
        void                        set_location_path( const std::string& location_path );

        bool                        is_allowed_method( const std::string& method ) const;
        //redircetion
        const bool&                 get_autoindex() const;
        const std::string&          get_root() const;
        const std::string&          get_index() const;
        const std::string&          get_upload_pass() const;
        const std::string&          get_location_path() const;


        const bool&                 is_cgi_existed() const;
        const bool&                 is_cgi_existed_php() const;
        const bool&                 is_cgi_existed_py() const;
        void                        set_cgi_existed( bool exsited );
        void                        set_cgi_existed_php( bool exsited );
        void                        set_cgi_existed_py( bool exsited );

        // const std::string           get_autoindex_content() const;
        //cgi

        const std::vector<std::string>& get_redirection() const;
        void                        set_redirection( const std::vector<std::string> redirect );



        Location();
        ~Location();
};

#endif
