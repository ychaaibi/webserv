#ifndef UTILS_HPP
# define UTILS_HPP

# include   <string>
# define    WHITESPACES "\t "

# define    SPACE ' '
# define    TAB '\t'

# include <string>
# include <vector>
# include <sstream>
# include <time.h>


typedef std::vector<std::string> split_t;

# define VALID_URL_CHAR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~!$&'()*+,;=:@/?#[]%"
# define MAX_VALID_URL_LENGTH 2048

class Utils
{
    public:
        static split_t              split_string( const std::string& str, std::string seps );
        static bool                 is_num( const std::string& str );
        static std::string          to_string( size_t num );
        static int                  count_reps( std::string s, char c );
        static size_t               s_to_size_t( std::string str );
        static bool                 valid_url( const std::string );
        static std::string          decode_url( const std::string );
        static int                  hex( char c );
        static std::string          extention( std::string path );
        
        static size_t               c;

        static std::string          name_file( std::string ext );
        static size_t               to_hex( std::string hx );
        static time_t               get_curr_time();
};

#endif