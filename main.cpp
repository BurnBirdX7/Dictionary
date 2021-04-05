#include <QApplication>
#include <QMessageBox>

#include "MainWindow.hpp"
#include "MemDictionary.hpp"

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
            else
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

    MainWindow* mw;
    Dictionary* dic;

    try {
        Args args;
        args.load();

        mw = new MainWindow();

        if (!args.do_not_store)
            dic = new MemDictionary(args.dictionary_path);
        else
            dic = new MemDictionary(args.dictionary_path); // TODO: Change to FileDictionary
        }
    catch (std::runtime_error& error) {
        QMessageBox::critical(nullptr, "Exception was thrown", error.what());
        return 0;
    }

    QObject::connect(dic, &Dictionary::stateChanged, mw, &MainWindow::searchStateChanged);
    QObject::connect(dic, &Dictionary::wordFound, mw, &MainWindow::addResultEntry);
    QObject::connect(mw, &MainWindow::search, dic, &Dictionary::search);

    mw->show();

    return QApplication::exec();
}
