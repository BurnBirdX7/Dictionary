#include "Dictionary.hpp"

Dictionary::Dictionary(QObject* parent)
    : QObject(parent)
    , mState(State::DONE)
{}

int Dictionary::getNewSeed()
{
    return ++mSeed;
}

QString Dictionary::getResults() const
{
    return mResults;
}

void Dictionary::wipeResults()
{
    auto locker = getResultsLocker();
    mResults.clear();
}

Dictionary::ResultsLocker Dictionary::getResultsLocker()
{
    return ResultsLocker(mResultsMutex);
}

void Dictionary::search(const QString& needle, Dictionary::SearchType type, int seed)
{
    QEventLoop loop; // Wait some time before starting
    QTimer::singleShot(50, &loop, &QEventLoop::quit);
    loop.exec();

    if (mSeed.load() != seed)
        return; // If seed was reassigned, cancel search

    changeState(State::SEARCH);
    wipeResults();

    if (type == SearchType::QUICK)
        this->quickSearch(needle.toStdString(), seed);
    else
        this->subsequentSearch(needle.toStdString(), seed);

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
    int m = static_cast<int>(needle.length());
    for (int i = 0; i < ASIZE; ++i)
        qsBc[i] = m + 1;
    for (int i = 0; i < m; ++i)
        qsBc[needle[i]] = m - i;
}

bool Dictionary::QS(const std::string& needle, const std::string& haystack, const int qsBc[]) {
    int m = static_cast<int>(needle.length());
    int n = static_cast<int>(haystack.length());
    for (int j = 0; j <= n - m; j += qsBc[static_cast<unsigned char>(haystack[j + m])])
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

void Dictionary::addResult(const std::string& result)
{
    auto locker = getResultsLocker();
    if(!mResults.isEmpty())
        mResults.append('\n');
    mResults.append(QString::fromStdString(result));
}
