#ifndef DICTIONARY_FILEDICTIONARY_HPP
#define DICTIONARY_FILEDICTIONARY_HPP

#include "Dictionary.hpp"

#include <QMessageBox>

#include <filesystem>
#include <fstream>

class FileDictionary
        : public Dictionary
{
Q_OBJECT

public:
    explicit FileDictionary(const QString& sourceFile, QObject* parent = nullptr);

private:
    void quickSearch(const std::string& needle) override;
    void subsequentSearch(const std::string& needle) override;

    bool openStream();

    void showCantOpenError() const;

    std::filesystem::path mPath;
    std::ifstream mStream;
};


#endif //DICTIONARY_FILEDICTIONARY_HPP
