#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Dictionary* dictionary, QWidget* parent)
        : QMainWindow(parent)
        , mUi(new Ui::MainWindow)
        , mDictionary(dictionary)
        , mDictionaryThread(this)
{
    mUi->setupUi(this);
    connect(mUi->inputEdit, &QLineEdit::textEdited, this, &MainWindow::initiateSearch);
    connect(mUi->subsequentCheckBox, &QCheckBox::stateChanged, this, [this](int) {
        this->initiateSearch(this->mUi->inputEdit->text());
    } );

    mDictionary->moveToThread(&mDictionaryThread);
    connect(&mDictionaryThread, &QThread::finished, mDictionary, &QObject::deleteLater);

    connect(mDictionary, &Dictionary::stateChanged, this, &MainWindow::searchStateChanged);
    connect(mDictionary, &Dictionary::wordFound, this, &MainWindow::addResultEntry);
    connect(this, &MainWindow::search, mDictionary, &Dictionary::search);

    mDictionaryThread.start();
}

MainWindow::~MainWindow()
{
    delete mUi;
    mDictionaryThread.quit();
    mDictionaryThread.wait();
}

void MainWindow::initiateSearch(QString searchLine)
{
    if (searchLine.isEmpty()) {
        mDictionary->getNewSeed(); // Cancel last search
        mUi->statusbar->showMessage(QStringLiteral("Input field is empty!"));
        mUi->resultEdit->clear();
        return;
    }

    this->clearResults();
    Dictionary::SearchType type;
    if (mUi->subsequentCheckBox->checkState() == Qt::Checked)
        type = Dictionary::SearchType::SUBSEQUENT;
    else
        type = Dictionary::SearchType::QUICK;

    emit search(searchLine, type, mDictionary->getNewSeed());
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
    if (newState == Dictionary::State::SEARCH) {
        mUi->subsequentCheckBox->setEnabled(false);
        mUi->statusbar->showMessage("Searching...");
    }
    else {
        mUi->subsequentCheckBox->setEnabled(true);
        mUi->statusbar->showMessage("Done.");
    }
}
