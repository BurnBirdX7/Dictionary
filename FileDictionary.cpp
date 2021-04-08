#include "FileDictionary.hpp"

FileDictionary::FileDictionary(const QString& sourceFile, QObject* parent)
    : Dictionary(parent)
    , mPath(sourceFile.toStdString())
{
    if (!std::filesystem::is_regular_file(mPath))
        throw std::runtime_error(FILE_IS_NOT_REGULAR + mPath.generic_string());
}

void FileDictionary::quickSearch(const std::string& needle)
{
    if (!openStream()) {
        showCantOpenError();
        return;
    }

    int qsBc[ASIZE];
    preQsBc(needle, qsBc);

    std::string word;
    while (!mStream.eof()) {
        std::getline(mStream, word);
        if (QS(needle, word, qsBc))
            emitEntry(word);
    }
    mStream.close();

    emitLastEntries();
}

void FileDictionary::subsequentSearch(const std::string& needle)
{
    if (!openStream()) {
        showCantOpenError();
        return;
    }

    std::string word;
    while (!mStream.eof()) {
        std::getline(mStream, word);
        if (SS(needle, word))
            emitEntry(word);
    }
    mStream.close();

    emitLastEntries();
}

bool FileDictionary::openStream()
{
    mStream.open(mPath);
    return mStream.good();
}

void FileDictionary::showCantOpenError() const
{
    QMessageBox::critical(nullptr,
                          QStringLiteral("Critical Error!"),
                          "Could not open the dictionary file: " +
                          QString::fromStdString(mPath.generic_string()));
}
