#include "FileDictionary.hpp"

FileDictionary::FileDictionary(const QString& sourceFile, QObject* parent)
    : Dictionary(parent)
    , mPath(sourceFile.toStdString())
{
    if (!std::filesystem::is_regular_file(mPath))
        throw std::runtime_error(FILE_IS_NOT_REGULAR + mPath.generic_string());
}

void FileDictionary::quickSearch(const std::string& needle, int seed)
{
    if (!openStream()) {
        showCantOpenError();
        return;
    }

    int qsBc[ALPHABET_SIZE];
    quickSearchPreprocessing(needle, qsBc);

    std::string word;
    while (!mStream.eof()) {
        if (mSeed.load() != seed)
            break; // Cancel job

        std::getline(mStream, word);
        if (quickSearchImplementation(needle, word, qsBc))
            addResult(word);
    }

    mStream.close();
}

void FileDictionary::sequenceSearch(const std::string& needle, int seed)
{
    if (!openStream()) {
        showCantOpenError();
        return;
    }

    std::string word;
    while (!mStream.eof()) {

        if (mSeed.load() != seed)
            break; // Cancel job

        std::getline(mStream, word);
        if (sequenceSearchImplementation(needle, word))
            addResult(word);
    }

    mStream.close();
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
