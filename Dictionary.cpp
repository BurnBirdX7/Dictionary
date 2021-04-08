#include "Dictionary.hpp"

Dictionary::Dictionary(QObject* parent)
    : QObject(parent)
    , mState(State::DONE)
{}

void Dictionary::search(const QString& needle, Dictionary::SearchType type)
{
    if (mState == State::SEARCH)
        return;

    changeState(State::SEARCH);

    if (type == SearchType::QUICK)
        this->quickSearch(needle.toStdString());
    else
        this->subsequentSearch(needle.toStdString());

    changeState(State::DONE);

}

void Dictionary::changeState(Dictionary::State newState)
{
    if(mState != newState) {
        mState = newState;
        emit stateChanged(newState);
    }
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
        if (haystack.compare(j, m, needle) == 0)
            return true;

    return false;
}

bool Dictionary::SS(const std::string& needle, const std::string& haystack)
{
    size_t m = needle.length();
    if (haystack.length() < m)
        return false;

    size_t curr = 0;
    for(char ch: haystack) {
        if (ch == needle[curr])
            if (++curr == m)
                return true;
    }

    return false;
}

void Dictionary::emitEntry(const std::string& entry)
{
    static auto ts1 = std::chrono::steady_clock::now();

    mBuffer.push_back(QString::fromLocal8Bit(entry.c_str()));

    if (std::chrono::steady_clock::now() - ts1 > RESULT_EMISSION_DELAY) {
        QString blob = mBuffer.front();

        for (auto it = ++mBuffer.begin(); it != mBuffer.end(); ++it)
            blob += "\n" + *it;

        emit wordFound(blob);
        mBuffer.clear();
        ts1 = std::chrono::steady_clock::now();
    }

}

void Dictionary::emitLastEntries()
{
    if (!mBuffer.empty()) {
        QString blob = mBuffer.front();
        for (auto it = ++mBuffer.begin(); it != mBuffer.end(); ++it)
            blob += "\n" + *it;

        emit wordFound(blob);
        mBuffer.clear();
    }
}
