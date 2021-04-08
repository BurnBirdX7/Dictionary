#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <stdexcept>
#include <atomic>
#include <string>
#include <list>

#include <QObject>

// Dictionary is a thread worker
// It's search() slot can be called multiple times from multiple threads,
//   but only one at a time will be executed.
class Dictionary
        : public QObject
{
Q_OBJECT

public:
    enum class State : char {
        SEARCH, DONE
    };
    enum class SearchType : char {
        QUICK, SUBSEQUENT
    };

    explicit Dictionary(QObject* parent = nullptr);

    constexpr static auto RESULT_EMISSION_DELAY = std::chrono::milliseconds(100);
    constexpr static auto FILE_IS_NOT_REGULAR = "Specified dictionary file does not exist or "
                                                "not a regular file: ";

    int getNewSeed();

public slots:
    void search(const QString& word, Dictionary::SearchType type, int seed);

signals:
    void stateChanged(Dictionary::State);
    void wordFound(QString);

protected:
    void changeState(State newState);

    virtual void quickSearch(const std::string& needle, int seed) = 0;
    virtual void subsequentSearch(const std::string& needle, int seed) = 0;

    static void preQsBc(const std::string& needle, int qsBc[]);
    static bool QS(const std::string& needle, const std::string& haystack, const int qsBc[]);
    static bool SS(const std::string& needle, const std::string& haystack);

    // Function buffers several entries and emits them after a short time
    void emitEntry(const std::string& entry);

    void emitLastEntries();

    void wipeLastEntries();

    const static size_t ASIZE = 256; // Alphabet's size | Assume ASCII

protected: // fields
    std::atomic<State> mState;
    std::atomic<int> mSeed;
    std::list<QString> mBuffer;

};


#endif //DICTIONARY_DICTIONARY_HPP
