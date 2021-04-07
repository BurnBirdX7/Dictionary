#include "FileDictionary.hpp"

FileDictionary::FileDictionary(const QString& sourceFile, QObject* parent)
    : Dictionary(parent)
    , mFile(sourceFile)
{
    if (!mFile.exists())
        throw std::runtime_error("Specified dictionary file does not exist");
}


void FileDictionary::search(const QString& word, Dictionary::SearchType type)
{
    if (mState == State::SEARCH)
        return;

    if (!mFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr,
                              "Critical Error!",
                              QStringLiteral("Could not open the dictionary file: ") + mFile.fileName());
        return;
    }

    changeState(State::SEARCH);
    QTextStream stream(&mFile);

    auto std_word = word.toStdString();
    if (type == SearchType::QUICK)
        quickSearch(std_word, stream);
    else
        subsequentSearch(std_word, stream);

    mFile.close();
    changeState(State::DONE);
}

void FileDictionary::quickSearch(const std::string& needle, QTextStream& stream)
{
    int qsBc[ASIZE];
    preQsBc(needle, qsBc);

    while (!stream.atEnd()) {
        auto word = stream.readLine();
        if (QS(needle, word.toStdString(), qsBc))
            emitEntry(word);
    }

    emitLastEntries();
}

void FileDictionary::subsequentSearch(const std::string& needle, QTextStream& stream)
{
    while (!stream.atEnd()) {
        auto word = stream.readLine();
        if (SS(needle, word.toStdString()))
            emitEntry(word);
    }

    emitLastEntries();
}