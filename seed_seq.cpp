
#include <array>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

int main( void ){
    random_device randDev;
    seed_seq seedSequence{ (unsigned int)randDev(), (unsigned int)time( nullptr ) };

    for( int i = 0; i < 5; ++i ){
        array< unsigned int, 5 > seeds;
        seedSequence.generate( seeds.begin(), seeds.end() );
        for( auto j : seeds ){
            cout << i << ": " << setw( 10 ) << j << " : ";
        }
        cout << endl << "--------------" << endl;
    }

    return 0;
}
