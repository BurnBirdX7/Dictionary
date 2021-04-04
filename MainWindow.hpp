#ifndef DICTIONARY_MAINWINDOW_HPP
#define DICTIONARY_MAINWINDOW_HPP

#include <QMainWindow>

#include "Dictionary.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow
        : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

public slots:
    void on_searchButton_clicked();

    void addResultEntry(const QString& entry);
    void clearResults();

    void searchStateChanged(Dictionary::State newState);

signals:
    void search(QString, Dictionary::SearchType);

private:
    Ui::MainWindow* mUi;

};

#endif //DICTIONARY_MAINWINDOW_HPP
