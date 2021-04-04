#include "Dictionary.hpp"

Dictionary::Dictionary(const QString& sourceFile, QObject* parent)
    : QObject(parent)
    , mState(State::DONE)
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

void Dictionary::search(const QString& word, Dictionary::SearchType type)
{
    std::shared_lock<std::shared_mutex> lock(this->mStateMutex);
    if (mState == State::SEARCH)
        return;

    std::thread searchThread(
            [this, word, type]() {
                changeState(State::SEARCH);

                auto std_word = word.toStdString();
                if (type == SearchType::QUICK)
                    quickSearch(std_word);
                else
                    subsequentSearch(std_word);

                changeState(State::DONE);
            }
    );

    searchThread.detach();
}

void Dictionary::changeState(Dictionary::State newState)
{
    std::unique_lock<std::shared_mutex> lock(this->mStateMutex);
    if(mState != newState) {
        mState = newState;
        emit stateChanged(newState);
    }
}

void Dictionary::quickSearch(const std::string& needle)
{
    int qsBc[ASIZE];

    preQsBc(needle, qsBc);

    for (const auto& dic_word: mDic)
        if (QS(needle, dic_word.toStdString(), qsBc))
            emit wordFound(dic_word);
}

void Dictionary::subsequentSearch(const std::string& needle)
{
    for (const auto& dic_word: mDic)
        if (SS(needle, dic_word.toStdString()))
            emit wordFound(dic_word);
}

void Dictionary::preQsBc(const std::string& needle, int qsBc[]) {
    int m = needle.length();
    for (int i = 0; i < ASIZE; ++i)
        qsBc[i] = m + 1;
    for (int i = 0; i < m; ++i)
        qsBc[needle[i]] = m - i;
}

bool Dictionary::QS(const std::string& needle, const std::string& haystack, const int qsBc[]) {
    int m = needle.length();
    int n = haystack.length();
    for (int j = 0; j <= n - m; j += qsBc[haystack[j + m]])
        if (memcmp(needle.c_str(), haystack.c_str() + j, m) == 0)
            return true;

    return false;
}

bool Dictionary::SS(const std::string& needle, const std::string& haystack)
{
    size_t m = needle.length();
    if (haystack.length() < m)
        return false;

    size_t curr = 0;
    for(const auto& ch: haystack) {
        if (ch == needle[curr])
            curr++;
        if (curr == m)
            return true;
    }

    return false;
}
