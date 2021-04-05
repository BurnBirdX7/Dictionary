#ifndef DICTIONARY_FILEDICTIONARY_HPP
#define DICTIONARY_FILEDICTIONARY_HPP

#include "Dictionary.hpp"

#include <QMessageBox>


class FileDictionary
        : public Dictionary
{
Q_OBJECT

public:
    explicit FileDictionary(const QString& sourceFile, QObject* parent = nullptr);

public slots:
    void search(const QString& word, SearchType type) override;

private:
    void quickSearch(const std::string& needle, QTextStream& stream);
    void subsequentSearch(const std::string& needle, QTextStream& stream);

    QFile mFile;
};


#endif //DICTIONARY_FILEDICTIONARY_HPP
