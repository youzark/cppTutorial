#include <vector>
#include <map>
#include <tuple>
#include <ranges>

std::tuple<int,int,int> ret(int i) {
    return {1,1,i};

}

int main() {
    std::map<int,int> mp{{1,2},{2,3}};
    for(auto [key,val]: mp) {
    }
    auto [i,j,fs] = ret(1);

}
