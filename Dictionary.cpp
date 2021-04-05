#include "Dictionary.hpp"

Dictionary::Dictionary(QObject* parent)
    : QObject(parent)
    , mState(State::DONE)
{}

void Dictionary::changeState(Dictionary::State newState)
{
    std::unique_lock<std::shared_mutex> lock(this->mStateMutex);
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
