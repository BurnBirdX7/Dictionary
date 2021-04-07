#include <QApplication>
#include <QMessageBox>

#include "MainWindow.hpp"
#include "MemDictionary.hpp"
#include "FileDictionary.hpp"

struct AppArguments{
    static constexpr const char* DONT_STORE_OPT = "--dont-store";

    bool do_not_store;
    QString dictionary_path;

    inline AppArguments()
        : do_not_store(false)
        , dictionary_path("words.txt")
    {

        auto list = QApplication::arguments();
        size_t size = list.size();

        switch (size) {
        case 1:
            break;
        case 2:
            if (list.at(1) == DONT_STORE_OPT)
                do_not_store = true;
            else // if isn't "dont_store", try as dictionary name
                dictionary_path = list.at(1);
            break;
        case 3:
            if (list.at(1) == DONT_STORE_OPT)
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
    QApplication app(argc, argv);

    qRegisterMetaType<Dictionary::State>("State");
    qRegisterMetaType<Dictionary::SearchType>("Dictionary::SearchType");

    // Dictionary
    Dictionary* dictionary;
    try {
        AppArguments args;

        if (args.do_not_store)
            dictionary = new FileDictionary(args.dictionary_path);
        else
            dictionary = new MemDictionary(args.dictionary_path);
    }
    catch (std::runtime_error& error) {
        QMessageBox::critical(nullptr, "Exception was thrown", error.what());
        return -1;
    }

    auto* mainWindow = new MainWindow(dictionary);
    mainWindow->show();

    return QApplication::exec();
}
