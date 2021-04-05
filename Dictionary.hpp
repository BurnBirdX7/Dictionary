#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <stdexcept>
#include <thread>
#include <shared_mutex>
#include <string>

#include <QObject>
#include <QFile>
#include <QTextStream>


class Dictionary
        : public QObject
{
Q_OBJECT

public:
    enum class State {
        SEARCH, DONE
    };
    enum class SearchType {
        QUICK, SUBSEQUENT
    };

    explicit Dictionary(QObject* parent = nullptr);

public slots:
    virtual void search(const QString& word, SearchType type) = 0;

signals:
    void stateChanged(State);
    void wordFound(QString);

protected:
    void changeState(State newState);

    static void preQsBc(const std::string& needle, int qsBc[]);
    static bool QS(const std::string& needle, const std::string& haystack, const int qsBc[]);
    static bool SS(const std::string& needle, const std::string& haystack);

    const static size_t ASIZE = 256; // Alphabet's size | Assume ASCII

protected: // fields
    State mState;
    std::shared_mutex mStateMutex;
};


#endif //DICTIONARY_DICTIONARY_HPP
