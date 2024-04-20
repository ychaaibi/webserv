#include "Get.hpp"

int Get::get_resource_type(std::string res)
{
    struct stat pathinfo;
    if(stat(res.c_str(), &pathinfo) != 0)
        return 0;
    if (S_ISDIR(pathinfo.st_mode))
        return 1;
    if (S_ISREG(pathinfo.st_mode))
        return 2;
    return -1;
}

void    Get::fill_header(std::string const header){
    _header = header;
}

std::string Get::get_header(){
    return(_header);
}

bool    Get::dir_have_permission(std::string path)
{
    const char *pathdir = path.c_str();

    DIR *dir = opendir(pathdir);
    if(dir){
        closedir(dir);
        return true;
    }
    else if(errno == EACCES)
        return false;
    return ( true );
}

std::string getFileExtension(const std::string& path) {
    size_t pointpos = path.find_last_of('.');

    if (pointpos != std::string::npos && pointpos < path.length() - 1) 
        return path.substr(pointpos);
    return "";
}

std::string    Get::get_content_type()
{ 
    std::string content_type;
    std::string file_ext;

    file_ext = getFileExtension(req.get_resource());

    content_type = inf.get_mime_type(file_ext);

    if(content_type.empty() && get_auto_index() == false)
        return "Content-Type: application/octet-stream\r\n";
    else if(content_type.empty() && __auto_index == true)
        return "Content-Type: text/html\r\n";
    return "Content-Type: " + content_type + "\r\n";
}

void    Get::set_auto_index(bool i){

    __auto_index = i;

}

bool    Get::get_auto_index(){
    return __auto_index;
}

std::string    Get::generate_title(int status)
{
    if ( inf.get_error_page_default( status ).second == "" and status != 200 )
        return "HTTP/1.1 " + inf.get_error_page_default(status).first + "\r\n\r\n";
    else
        return "HTTP/1.1 " + inf.get_error_page_default(status).first + "\r\n";
}

long    Get::get_content_lenght()
{
    struct stat fileStat;

    if ( stat(req.get_resource().c_str(), &fileStat) != 0)
        return -1;

    if (S_ISREG(fileStat.st_mode))
        return fileStat.st_size;
    else 
        return -1;
}

std::string longtostring(long num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

const std::string*     Get::get_buffer() const
{
    return( _buffer );
}   

void    Get::set_header(int status, std::string str)
{
    std::string title , content_type, content_lenght;
    long    lenght;

    if ( status != AUTO_INDEX and !req.get_error() and !req.get_header_set() and req.get_location()->is_cgi_existed() )
        return ;

    if ( status == AUTO_INDEX )
        status = 200;

    title = generate_title(status);

    content_type = get_content_type();

    if(str.empty())
        lenght = get_content_lenght();
    else
        lenght = str.size();
    
    content_lenght = longtostring(lenght);

    if(status != 200)
        content_type = "Content-Type: text/html\r\n";
    fill_header(title  + content_type  
            + "Content-Length: " + content_lenght + "\r\n\r\n");
}

void    Get::list_dir(std::string path)
{
        std::string output;
	    output.append("<html><body><ul>");
	    DIR *dir;
	    struct dirent *ent;
	    if ((dir = opendir(path.c_str())) != NULL)
	    {
		    while ((ent = readdir(dir)) != NULL)
		    {
			    output.append("<li><a href=\"");
			    output.append(req.get_url() + ent->d_name);
			    output.append("\">");
			    output.append(ent->d_name);
			    output.append("</a></li>");
		    }
		    closedir(dir);
        }
        
        set_header(AUTO_INDEX, output);
        
        delete _buffer;
        _buffer = new std::string( get_header() + output);
}

std::string Get::send_chunk()
{
    if ( !req.get_error() and req.get_location()->is_cgi_existed() )
    {
        try
        {
            delete _buffer;
            _buffer = new std::string( cgi->call_cgi() );

            return ("");
        }
        catch( int e )
        {
            _buffer = NULL;
            if (e == -1)
            {
                set_end_response( true );
                return ("");
            }
        }

        if ( req.get_error() == 404 )
        {
            _buffer = NULL;
            req.set_error( 404 );
            return ( "" );
        }

        set_end_response( true );
        return ( "" );
    }

    char buff[buffersize];
    file.read(buff, buffersize);

    delete _buffer;
    _buffer = new std::string(buff, file.gcount());

    if(file.eof())
    {
        set_end_response( true );
        file.close();
    }
    return ("");
}

void    Get::chunk_file(std::string path, int status)
{
    if(status == 200)
    {
        if(flag == 0)
        {
            file.open(path.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
            set_header(200, "");

            delete _buffer;
            _buffer = new std::string( get_header());

            flag = 1;
            return ;
        }
        else
            send_chunk();
    }
    else
    {
        if(flag == 0)
        {
            if((access(inf.get_error_page_custom(status).second.c_str(), F_OK)) == 0)
                req.set_resource(inf.get_error_page_custom(status).second);
            else if((access(inf.get_error_page_default(status).second.c_str(), F_OK) == 0))
                req.set_resource(inf.get_error_page_default(status).second);

            if ( file.is_open() )
                file.close();
            
            file.open(req.get_resource().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
            set_header(status, "");

            if ( !file.is_open() )
                req.set_error(403);

            delete _buffer;
            _buffer = new std::string( get_header());

            flag = 1;
            return ;
        }
        else
            send_chunk();
    }
}

std::string    Get::set_new_url(std::string str)
{
    int i = 1;
    return &str[i];
}


void    Get::get_method()
{
    if(req.get_error() != 0)
        chunk_file("", req.get_error());
    else if(req.get_header_set() != 0)
    {
        if(req.get_header_set() == 301 or req.get_header_set() == 302 )
        {
            delete _buffer;
            if ( req.get_location()->get_redirection().size() )
                _buffer = new std::string( "HTTP/1.1 302 Found\r\nLocation: https://" + req.get_location()->get_redirection()[1]  + "/\r\n\r\n" );
            else
                _buffer = new std::string( "HTTP/1.1 301 Moved Permanently\r\nLocation: https://" + req.get_url()  + "\r\n\r\n" );

            set_end_response( true );
        }
        else
        {
            delete _buffer;
            _buffer = new std::string (generate_title(req.get_header_set()));

            set_end_response( true );   
        }
    }
    else if(get_resource_type(req.get_resource()) == 1)
    {
        if(req.get_resource()[ req.get_resource().length() - 1 ] != '/')
        {
            delete _buffer;
            _buffer = new std::string( "HTTP/1.1 301 Moved Permanently\r\nLocation: " + req.get_url()  + "/\r\n\r\n" );

            set_end_response( true );

        }
        else if(req.get_location()->get_autoindex() == true)
        {
            if(dir_have_permission(req.get_resource()) == true)
            {
                set_auto_index(true);
                list_dir(req.get_resource());
                set_end_response( true );
                return ;
            }
            else
            {
                req.set_error(403);
                return ;
            }
        }
        else if(!req.get_location()->get_index().empty() && get_auto_index() == false)
        {
            if(access((req.get_resource() + req.get_location()->get_index()).c_str(), F_OK) == 0)
            {
                req.set_resource( req.get_resource() + req.get_location()->get_index() );
                chunk_file(req.get_resource(), 200);
            }
            else
            {
                req.set_error(404);
                return ;
            }
        }
        else
        {
            req.set_error(403);
            return ;
        }
    }
    else if(get_resource_type(req.get_resource()) == 2)
    {
        if(flag == 0)
            file.open(req.get_resource().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
            
        if(access(req.get_resource().c_str(), F_OK) != 0)
        {
            req.set_error(404);
            return ;
        }
        else if(!file.is_open() || file.rdstate() & std::ios::failbit)
        {
            file.close();
            req.set_error(403);
            return ;
        }
        else
        {
            if(flag == 0)
                file.close();

            chunk_file(req.get_resource(), 200);
        }
    }
    else
        req.set_error(404);

}

void                Get::set_end_response( bool end_response )
{
    __end_response = end_response;
}

bool                Get::get_end_response() const
{
    return ( __end_response );
}

const Cgi*          Get::get_cgi() const 
{
    return ( cgi );
}

Get::Get(Request &passed, const ServerInfo& info) : req(passed), inf(info)
{

    buffersize = 2048;
    __auto_index = false;
    __end_response  = false;
    _buffer         = NULL;
    flag            = 0;
    cgi             = new Cgi( passed );
}

Get::~Get()
{
    delete _buffer;
    delete cgi;
}

void            Get::set_flag(int _flag)
{
    flag = _flag;
}

int             Get::get_flag()
{
    return ( flag );
}


///
// try
// {
//     string s = ( call_cgi() );
//     return (s);
// }
// catch()
// {
//     return ("");
// }