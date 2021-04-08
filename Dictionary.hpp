#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <stdexcept>
#include <atomic>
#include <string>
#include <list>

#include <QObject>


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

public slots:
    void search(const QString& word, SearchType type);

signals:
    void stateChanged(State);
    void wordFound(QString);

protected:
    void changeState(State newState);

    virtual void quickSearch(const std::string& needle) = 0;
    virtual void subsequentSearch(const std::string& needle) = 0;

    static void preQsBc(const std::string& needle, int qsBc[]);
    static bool QS(const std::string& needle, const std::string& haystack, const int qsBc[]);
    static bool SS(const std::string& needle, const std::string& haystack);

    // Function buffers several entries and emits them after a short time
    void emitEntry(const std::string& entry);
    // Emits last entries
    void emitLastEntries();

    const static size_t ASIZE = 256; // Alphabet's size | Assume ASCII

protected: // fields
    std::atomic<State> mState;
    std::list<QString> mBuffer;

};


#endif //DICTIONARY_DICTIONARY_HPP
