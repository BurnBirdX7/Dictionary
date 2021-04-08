#ifndef DICTIONARY_MEMDICTIONARY_HPP
#define DICTIONARY_MEMDICTIONARY_HPP

#include "Dictionary.hpp"

#include <filesystem>
#include <fstream>

class MemDictionary
        : public Dictionary
{
Q_OBJECT

public:
    explicit MemDictionary(const QString& filename, QObject* parent = nullptr);

private:
    void quickSearch(const std::string& needle, int seed) override;
    void subsequentSearch(const std::string& needle, int seed) override;

    std::list<std::string> mDic;

};


#endif //DICTIONARY_MEMDICTIONARY_HPP
