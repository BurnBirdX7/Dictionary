#ifndef DICTIONARY_MEMDICTIONARY_HPP
#define DICTIONARY_MEMDICTIONARY_HPP

#include "Dictionary.hpp"


class MemDictionary
        : public Dictionary
{
Q_OBJECT

public:
    explicit MemDictionary(const QString& filename, QObject* parent = nullptr);

public slots:
    void search(const QString& word, SearchType type) override;

private:
    void quickSearch(const std::string& needle);
    void subsequentSearch(const std::string& needle);

    QStringList mDic;

};


#endif //DICTIONARY_MEMDICTIONARY_HPP
