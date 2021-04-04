#include <QApplication>
#include <QPushButton>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    qRegisterMetaType<Dictionary::State>("State");

    QApplication app(argc, argv);

    QString dictionaryFile;
    if (app.arguments().size() > 1)
        dictionaryFile = app.arguments().at(1);
    else
        dictionaryFile = QStringLiteral("words.txt");

    auto* mw = new MainWindow();
    auto* dic = new Dictionary(dictionaryFile);

    QObject::connect(dic, &Dictionary::stateChanged, mw, &MainWindow::searchStateChanged);
    QObject::connect(dic, &Dictionary::wordFound, mw, &MainWindow::addResultEntry);
    QObject::connect(mw, &MainWindow::search, dic, &Dictionary::search);

    mw->show();

    return QApplication::exec();
}
