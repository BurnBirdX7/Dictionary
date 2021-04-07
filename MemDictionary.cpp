#include "MemDictionary.hpp"

MemDictionary::MemDictionary(const QString& sourceFile, QObject* parent)
        : Dictionary(parent)
        , mDic()
{
    QFile file(sourceFile);
    if (!file.exists())
        throw std::runtime_error("Specified dictionary file does not exist");
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Cannot open the dictionary");

    QTextStream in(&file);
    while(!in.atEnd())
        mDic.append(in.readLine());
}

void MemDictionary::search(const QString& word, Dictionary::SearchType type)
{
    if (mState == State::SEARCH)
        return;

    changeState(State::SEARCH);

    auto std_word = word.toStdString();
    if (type == SearchType::QUICK)
        quickSearch(std_word);
    else
        subsequentSearch(std_word);

    changeState(State::DONE);
}

void MemDictionary::quickSearch(const std::string& needle)
{
    int qsBc[ASIZE];
    preQsBc(needle, qsBc);

    for (const auto& word: mDic)
        if (QS(needle, word.toStdString(), qsBc))
            emitEntry(word);

    emitLastEntries();
}

void MemDictionary::subsequentSearch(const std::string& needle)
{
    for (const auto& word: mDic)
        if (SS(needle, word.toStdString()))
            emitEntry(word);

    emitLastEntries();
}
