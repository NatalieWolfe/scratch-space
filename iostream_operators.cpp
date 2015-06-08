
#include <iomanip>
#include <sstream>
#include <string>

struct GetTime {
    mutable std::tm* time_struct;
    const std::string format;

    // std::istream& operator()( std::istream& istream  ){
    //     return istream >> std::get_time( time_struct, format.c_str() );
    // }
};

GetTime my_get_time( std::tm* time_struct, const char* format ){
    return GetTime{ time_struct, format };
}

std::istream& operator>>( std::istream& istream, const GetTime& gt ){
    return istream >> std::get_time( gt.time_struct, gt.format.c_str() );
}


int main( void ){
    std::stringstream sstream;
    std::tm time_struct;

    sstream.str( "1986-09-15" );
    sstream >> my_get_time( &time_struct, "%Y-%m-%d" );

    return 0;
}
