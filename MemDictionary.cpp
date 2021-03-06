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

void MemDictionary::quickSearch(const std::string& needle, int seed)
{
    int qsBc[ALPHABET_SIZE];
    quickSearchPreprocessing(needle, qsBc);

    for (const auto& word: mDic) {
        if (mSeed.load() != seed)
            break;

        if (quickSearchImplementation(needle, word, qsBc))
            addResult(word);
    }
}

void MemDictionary::sequenceSearch(const std::string& needle, int seed)
{
    for (const auto& word: mDic) {
        if (mSeed.load() != seed)
            break;

        if (sequenceSearchImplementation(needle, word))
            addResult(word);
    }
}
