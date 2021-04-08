#include "MemDictionary.hpp"

MemDictionary::MemDictionary(const QString& sourceFile, QObject* parent)
        : Dictionary(parent)
        , mDic()
{
    std::filesystem::path path(sourceFile.toStdString());
    if (!std::filesystem::is_regular_file(path))
        throw std::runtime_error(FILE_IS_NOT_REGULAR + path.generic_string());
    std::ifstream stream(path);
    if (!stream.good())
        throw std::runtime_error("Cannot read the dictionary: " + path.generic_string());

    std::string line;
    while(!stream.eof()) {
        std::getline(stream, line);
        mDic.push_back(line);
    }
    stream.close();
}

void MemDictionary::quickSearch(const std::string& needle)
{
    int qsBc[ASIZE];
    preQsBc(needle, qsBc);

    for (const auto& word: mDic)
        if (QS(needle, word, qsBc))
            emitEntry(word);

    emitLastEntries();
}

void MemDictionary::subsequentSearch(const std::string& needle)
{
    for (const auto& word: mDic)
        if (SS(needle, word))
            emitEntry(word);

    emitLastEntries();
}
