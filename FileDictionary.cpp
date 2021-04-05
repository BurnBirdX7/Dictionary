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
    std::shared_lock<std::shared_mutex> lock(this->mStateMutex);
    if (mState == State::SEARCH)
        return;

    if (!mFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr,
                              "Critical Error!",
                              QStringLiteral("Could not open the dictionary file: ") + mFile.fileName());
        return;
    }


    std::thread searchThread(
            [this, word, type]() {
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
    );

    searchThread.detach();
}

void FileDictionary::quickSearch(const std::string& needle, QTextStream& stream)
{
    int qsBc[ASIZE];
    preQsBc(needle, qsBc);

    while (!stream.atEnd()) {
        auto dic_word = stream.readLine();
        if (QS(needle, dic_word.toStdString(), qsBc))
                emit wordFound(dic_word);
    }
}

void FileDictionary::subsequentSearch(const std::string& needle, QTextStream& stream)
{
    while (!stream.atEnd()) {
        auto dic_word = stream.readLine();
        if (SS(needle, dic_word.toStdString()))
                emit wordFound(dic_word);
    }
}