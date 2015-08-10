
#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <unordered_map>

struct Item {
    std::int64_t id;
};

typedef std::list< Item > List;

struct DiffItem {
    const Item& item;
    bool remote;
    bool local;
};

template<typename Map>
void diff(const List& a, const List& b, List& toAdd, List& toRemove){
    // Prepare to diff.
    Map diff;

    // Loop through the provided list and load the diff.
    for(const auto& item : a){
        diff.emplace(item.id, DiffItem{ item, true, false });
    }

    // Loop through our list and finish the diff.
    for(const auto& item : b){
        auto itr = diff.find(item.id);
        if(itr == diff.end()){
            diff.emplace(item.id, DiffItem{ item, false, true });
        }
        else {
            itr->second.local = true;
        }
    }

    // Now separate the diff into things to be removed and things to be added.
    for(const auto& diffItem : diff){
        // Remote == true && Local == false -> add
        if(diffItem.second.remote && !diffItem.second.local){
            toAdd.push_back(diffItem.second.item);
        }

        // Remote == false && Local == true -> remove
        else if(!diffItem.second.remote && diffItem.second.local){
            toRemove.push_back(diffItem.second.item);
        }
    }
}

void performDiff(const int count, const float rangeMultiplier = 2){
    using namespace std::chrono;
    typedef high_resolution_clock Clock;
    List a;
    List b;
    List add;
    List remove;

    const int idLimit = (int)((float)count * rangeMultiplier);
    std::random_device device;
    std::mt19937_64 rand(device());
    std::uniform_int_distribution<std::int64_t> dist(0, idLimit);
    const auto& gen = [&](){ return dist(rand); };
    for(int i = 0; i < count; ++i){
        a.push_back(Item{ gen() });
        b.push_back(Item{ gen() });
    }

    auto start = Clock::now();
    diff< std::map<std::int64_t, DiffItem> >(a, b, add, remove);
    auto end = Clock::now();

    auto fastStart = Clock::now();
    diff< std::unordered_map<std::int64_t, DiffItem> >(a, b, add, remove);
    auto fastEnd = Clock::now();

    std::cout << count << " count, range 0 to " << idLimit << "." << std::endl;
    std::cout << "std::map:           " << duration_cast<milliseconds>(end - start).count() << "ms" << std::endl;
    std::cout << "std::unordered_map: " << duration_cast<milliseconds>(fastEnd - fastStart).count() << "ms" << std::endl;
    std::cout << std::endl;
}

int main(){
    performDiff(    1000);
    performDiff(   10000);
    performDiff(  100000);
    performDiff( 1000000);
    performDiff(10000000);

    performDiff(1000000,  0.1);
    performDiff(1000000,  1);
    performDiff(1000000, 10);
}
