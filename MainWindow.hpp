#ifndef DICTIONARY_MAINWINDOW_HPP
#define DICTIONARY_MAINWINDOW_HPP

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "Dictionary.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow
        : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(Dictionary* dictionary, QWidget* parent = nullptr);

    ~MainWindow() override;

public slots:
    void initiateSearch(const QString& searchLine);
    void updateResults();
    void searchStateChanged(Dictionary::State newState);

signals:
    void search(QString, Dictionary::SearchType, int);

private:
    Ui::MainWindow* mUi;
    QTimer* mUpdateTimer;
    Dictionary* mDictionary;
    QThread mDictionaryThread;

};

#endif //DICTIONARY_MAINWINDOW_HPP
