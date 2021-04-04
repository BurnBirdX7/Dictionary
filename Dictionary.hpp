#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <stdexcept>
#include <thread>
#include <shared_mutex>
#include <string>

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>


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

    explicit Dictionary(const QString& sourceFile, QObject* parent = nullptr);

public slots:
    void search(const QString& word, SearchType type = SearchType::QUICK);

signals:
    void stateChanged(State);
    void wordFound(QString);

private:
    void changeState(State newState);

    void quickSearch(const std::string& word);
    void subsequentSearch(const std::string& word);

    static void preQsBc(const std::string& needle, int qsBc[]);
    static bool QS(const std::string& needle, const std::string& haystack, const int qsBc[]);
    static bool SS(const std::string& needle, const std::string& haystack);

    const static size_t ASIZE = 256; // Alphabet's size | Assume ASCII

private:
    State mState;
    std::shared_mutex mStateMutex;

    QStringList mDic;
};


#endif //DICTIONARY_DICTIONARY_HPP
