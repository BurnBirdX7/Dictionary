#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <stdexcept>
#include <atomic>
#include <string>
#include <list>
#include <mutex>

#include <QObject>
#include <QTimer>
#include <QEventLoop>

// Dictionary is a thread worker
// It's search() slot can be called multiple times from multiple threads,
//   but only one at a time will be executed.
class Dictionary
        : public QObject
{
Q_OBJECT

public:
    using ResultsLocker = std::lock_guard<std::mutex>;

    enum class State : char {
        SEARCH, DONE
    };
    enum class SearchType : char {
        QUICK, SUBSEQUENT
    };

    explicit Dictionary(QObject* parent = nullptr);

    int getNewSeed();
    QString getResults() const; // You have to acquire ResultsLocker
    void wipeResults();

    ResultsLocker getResultsLocker();

public slots:
    void search(const QString& word, Dictionary::SearchType type, int seed);

signals:
    void stateChanged(Dictionary::State);

protected:
    void changeState(State newState);

    virtual void quickSearch(const std::string& needle, int seed) = 0;
    virtual void subsequentSearch(const std::string& needle, int seed) = 0;

    static void preQsBc(const std::string& needle, int qsBc[]);
    static bool QS(const std::string& needle, const std::string& haystack, const int qsBc[]);
    static bool SS(const std::string& needle, const std::string& haystack);

    void addResult(const std::string& result);

    const static size_t ASIZE = 256; // Alphabet's size
    constexpr static auto RESULT_EMISSION_DELAY = std::chrono::milliseconds(100);
    constexpr static auto FILE_IS_NOT_REGULAR = "Specified dictionary file does not exist or "
                                                "not a regular file: ";

protected: // fields
    std::atomic<State> mState;
    std::atomic<int> mSeed;
    std::mutex mResultsMutex;
    QString mResults;

};


#endif //DICTIONARY_DICTIONARY_HPP
