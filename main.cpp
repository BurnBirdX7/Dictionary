#include <QApplication>
#include <QThread>
#include <QMessageBox>

#include "MainWindow.hpp"
#include "MemDictionary.hpp"
#include "FileDictionary.hpp"

struct Args{
    bool do_not_store = false;
    QString dictionary_path = "words.txt";

    inline void load()
    {
        auto list = QApplication::arguments();
        size_t size = list.size();

        switch (size) {
        case 1:
            break;
        case 2:
            if (list.at(1) == "--dont_store")
                do_not_store = true;
            else // if isn't "dont_store", try as dictionary name
                dictionary_path = list.at(1);
            break;
        case 3:
            if (list.at(1) == "--dont_store")
                do_not_store = true;
            else
                throw std::runtime_error("Unknown argument");
            dictionary_path = list.at(2);
            break;
        default:
            throw std::runtime_error("Incorrect number of arguments");
        }
    }

};

int main(int argc, char* argv[])
{
    qRegisterMetaType<Dictionary::State>("State");
    qRegisterMetaType<Dictionary::SearchType>("Dictionary::SearchType");

    QApplication app(argc, argv);

    Dictionary* dic;
    try {
        Args args;
        args.load();

        if (!args.do_not_store)
            dic = new MemDictionary(args.dictionary_path);
        else
            dic = new FileDictionary(args.dictionary_path);
    }
    catch (std::runtime_error& error) {
        QMessageBox::critical(nullptr, "Exception was thrown", error.what());
        return -1;
    }

    QThread dictionaryThread;
    dic->moveToThread(&dictionaryThread);
    QObject::connect(&dictionaryThread, &QThread::finished, dic, &QObject::deleteLater);

    // dic -> main window
    auto* mw = new MainWindow();
    QObject::connect(dic, &Dictionary::stateChanged, mw, &MainWindow::searchStateChanged);
    QObject::connect(dic, &Dictionary::wordFound, mw, &MainWindow::addResultEntry);

    // main window -> dic
    QObject::connect(mw, &MainWindow::search, dic, &Dictionary::search);

    dictionaryThread.start();
    mw->show();

    auto ret = QApplication::exec();

    dictionaryThread.quit();
    dictionaryThread.wait();

    return ret;
}
