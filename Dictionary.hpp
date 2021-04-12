#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <stdexcept>
#include <atomic>
#include <string>
#include <mutex>

#include <QObject>
#include <QTimer>
#include <QEventLoop>

// Dictionary class provides interface and common functionality for different dictionary implementations.
// (e.g. implementations of search algorithms)
//
// has pure virtual methods
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

    int getNewSeed();           // Getting new seed cancels any ongoing search
    QString getResults() const; // You have to acquire ResultsLocker first
    void wipeResults();         // Safe to call without acquiring ResultsLocker

    ResultsLocker getResultsLocker(); // Returns mutex locker

public slots:
    void search(const QString& word, Dictionary::SearchType type, int seed);

signals:
    void stateChanged(Dictionary::State);

protected:
    void changeState(State newState);           // changes state if newState is different from the current state
    void addResult(const std::string& result);  // Adds result to the gettable result string

    virtual void quickSearch(const std::string& needle, int seed) = 0;      // Pure virtual
    virtual void sequenceSearch(const std::string& needle, int seed) = 0;   // Pure virtual

    static void quickSearchPreprocessing(const std::string& needle, int * qsBc);
    static bool quickSearchImplementation(const std::string& needle, const std::string& haystack, const int* qsBc);
    static bool sequenceSearchImplementation(const std::string& needle, const std::string& haystack);

    const static size_t   ALPHABET_SIZE = 256;
    constexpr static auto FILE_IS_NOT_REGULAR = "Specified dictionary file does not exist or "
                                                "not a regular file: ";

protected: // fields
    std::atomic<State> mState;
    std::atomic<int> mSeed;
    std::mutex mResultsMutex;
    QString mResults;

};


#endif //DICTIONARY_DICTIONARY_HPP
