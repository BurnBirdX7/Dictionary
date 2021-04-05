#include <QApplication>
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

    auto* mw = new MainWindow();
    QObject::connect(dic, &Dictionary::stateChanged, mw, &MainWindow::searchStateChanged);
    QObject::connect(dic, &Dictionary::wordFound, mw, &MainWindow::addResultEntry);
    QObject::connect(mw, &MainWindow::search, dic, &Dictionary::search);

    mw->show();
    return QApplication::exec();
}
