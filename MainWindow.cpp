#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Dictionary* dictionary, QWidget* parent)
        : QMainWindow(parent)
        , mUi(new Ui::MainWindow)
        , mDictionary(dictionary)
        , mDictionaryThread(this)
        , mUpdateTimer(new QTimer(this))
{
    mUi->setupUi(this);
    connect(mUi->inputEdit, &QLineEdit::textEdited, this, &MainWindow::initiateSearch);
    connect(mUi->subsequentCheckBox, &QCheckBox::stateChanged, this, [this](int) {
        this->initiateSearch(this->mUi->inputEdit->text());
    } );

    // Dictionary:
    mDictionary->moveToThread(&mDictionaryThread);
    connect(&mDictionaryThread, &QThread::finished, mDictionary, &QObject::deleteLater);
    connect(mDictionary, &Dictionary::stateChanged, this, &MainWindow::searchStateChanged);
    connect(this, &MainWindow::search, mDictionary, &Dictionary::search);

    mDictionaryThread.start();

    mUpdateTimer->start(100);
}

MainWindow::~MainWindow()
{
    delete mUi;
    mDictionaryThread.quit();
    mDictionaryThread.wait();
}

void MainWindow::initiateSearch(const QString& searchLine)
{
    if (searchLine.isEmpty()) {
        mDictionary->getNewSeed(); // Cancel the last search
        mDictionary->wipeResults(); // Wipe results of that search
        mUi->resultEdit->clear();
        return;
    }

    mDictionary->wipeResults(); // Wipe previous search's results
    Dictionary::SearchType type;
    if (mUi->subsequentCheckBox->checkState() == Qt::Checked)
        type = Dictionary::SearchType::SUBSEQUENT;
    else
        type = Dictionary::SearchType::QUICK;

    emit search(searchLine, type, mDictionary->getNewSeed());
}

void MainWindow::updateResults()
{
    auto locker = mDictionary->getResultsLocker();
    mUi->resultEdit->setPlainText(mDictionary->getResults());
}

void MainWindow::searchStateChanged(Dictionary::State newState)
{
    if (newState == Dictionary::State::SEARCH) {
        mUi->statusbar->showMessage("Searching...");
        connect(mUpdateTimer, &QTimer::timeout, this, &MainWindow::updateResults);
    }
    else {
        mUi->statusbar->showMessage("Done.");
        disconnect(mUpdateTimer, &QTimer::timeout, this, &MainWindow::updateResults);
        updateResults();
    }
}
