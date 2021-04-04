#include "mainwindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::on_searchButton_clicked()
{
    auto text = mUi->inputEdit->text();
    if (text.isEmpty()) {
        mUi->statusbar->showMessage(QStringLiteral("Input field is empty!"));
        return;
    }

    this->clearResults();
    auto type = Dictionary::SearchType::QUICK;

    emit search(text, type);
}

void MainWindow::addResultEntry(const QString& entry)
{
    mUi->resultEdit->appendPlainText(entry);
}

void MainWindow::clearResults()
{
    mUi->resultEdit->clear();
}

void MainWindow::searchStateChanged(Dictionary::State newState)
{
    if (newState == Dictionary::State::SEARCH)
        mUi->statusbar->showMessage("Searching...");
    else
        mUi->statusbar->showMessage("Done.");
}
