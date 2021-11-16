#include <string>
#include <vector>

namespace LZW {
    std::vector<int> compress(std::string);

    std::string decompress(std::vector<int>);
}