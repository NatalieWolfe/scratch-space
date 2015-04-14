
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main( void ){
    std::map< std::string, std::vector< std::string > > mapVectorString = {
        { "one",    { "foobar"              } },
        { "two",    { "foo", "bar"          } },
        { "three",  { "fiz", "biz", "baz"   } }
    };

    for( auto&& val : mapVectorString ){
        std::cout << val.first << ": ";
        for( auto&& str : val.second ){
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }
}
