#ifndef GLOBAL_H
#define GLOBAL_H
#include <iostream>
#include <vector>

struct englishWord
{
        std::string word;
        std::string attr;
        std::vector<std::string> means;
};

class global
{
public:
    global();

    std::vector<englishWord> englishWordList_;
};

#endif // GLOBAL_H
