#include "Delete.hpp"

Delete::Delete( Request &passed, const ServerInfo& info ) : _req(passed) , _inf(info)
{
    flag = 0;
}

Delete::~Delete()
{
}

struct subdirectory {
    std::string name;
    bool isDirectory;

    subdirectory(const std::string& n, bool isDir) : name(n), isDirectory(isDir) {}
};

void Get_dir_subs(const char* path, std::vector<subdirectory>& entries) 
{
    DIR* dir = opendir(path);
    if (!dir) 
        return ;
    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) 
    {
        if (std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0) 
        {
            std::string entryPath = std::string(path) + entry->d_name;
            bool isDirectory = entry->d_type == DT_DIR;
            entries.push_back(subdirectory(entryPath, isDirectory));
        }
    }
    closedir(dir);
}

void  Delete::delete_file(std::string path)
{
    if (access(path.c_str(), W_OK) == 0)
    {
        if(remove(path.c_str()) != 0)
            _req.set_error(404);
    }
    else
        _req.set_error(403);
}

std::string    Delete::set_new_url(std::string str)
{
    int i = 1;
    return &str[i];
}

void  Delete::delete_dir(std::string path)
{
    std::string new_url;
    if (rmdir(path.c_str()) != 0) 
        _req.set_error(403);
    if(_req.get_url().at(0) == '/')
    {
        new_url = set_new_url(_req.get_url());
        _req.set_resource(_req.get_location()->get_root() + new_url);
    }
    else
        _req.set_resource(_req.get_location()->get_root() + _req.get_url());
}

bool    dir_have_permission(std::string path)
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

int get_resource_type(std::string res)
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

void  Delete::delete_method()
{
    if(get_resource_type(_req.get_resource()) == 1)
    {
        if(_req.get_resource()[_req.get_resource().length() - 1 ] != '/')
        {
          _req.set_error(409);
          return ;
        }
        
        if(dir_have_permission(_req.get_resource()))
        {
            flag = 1;
            std::vector<subdirectory> entries;
            Get_dir_subs(_req.get_resource().c_str(), entries);
            if(!entries.empty())
            {
               for (size_t i = 0; i < entries.size(); ++i)
               {
                    if(entries[i].isDirectory)
                    {
                        _req.set_resource(entries[i].name + "/");
                        delete_method();
                    }
                    else
                    {
                        delete_file(entries[i].name);
                    }
               }
            }
            else
            {
                delete_dir(_req.get_resource());
            }
        }
        else
        {
            _req.set_error(403);
            return ;
        }

    }
    else if( get_resource_type(_req.get_resource()) == 2 )
    {
        delete_file(_req.get_resource());
        if(flag == 0)
            _req.set_header_set(204);
    }
    else if(flag == 1 && get_resource_type(_req.get_resource()) == 0)
    {
       _req.set_header_set(204);
    }
    else
       _req.set_error(404);
}