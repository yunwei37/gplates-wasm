// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
#include <cassert>
#include <streambuf>
#include <QListWidget>
#include <QPushButton>
#include <QSlider>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE


class OutputLabelStreamBuffer : public std::streambuf
{
public:
    void setOutputLabel(QPlainTextEdit *label)
    {
        outputTextEdit = label;
    }
    void clear()
    {
        buffer.clear();
    }
    int overflow(int c) override
    {
        if (c != EOF)
        {
            char ch = static_cast<char>(c);
            if (ch == '\n')
            {
                assert(outputTextEdit);
                buffer.push_back(ch);
                outputTextEdit->setPlainText(buffer);
            }
            else
            {
                buffer.push_back(ch);
            }
        }
        return c;
    }

private:
    QPlainTextEdit  *outputTextEdit;
    QString buffer;
};

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void loadFile(const QString &fileName);
    void loadFileContent(const QString &fileName, const QString &fileContent);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();

    // reconstruction times
    void restoreReconTimesDefaultValues();
    void addReconTimesValue();
    void deleteReconTimesValue();

    // read gpml data
    void open_and_read_gpml();
    // reconstruction with gpml
    void reconstruction();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QPlainTextEdit  *outputTextEdit;
    QString curFile;
    OutputLabelStreamBuffer outputStreamBuffer;
    QLabel *inputLabel;
    QLabel *outputLabel;

     // reconstruction lists
    QListWidget *reconTimesListWidget;

     // reconstruction times control
    QPushButton *restoreTimesListsDefaultsButton;
    QPushButton *addTimesValueButton;
    QPushButton *deleteTimesValueButton;
    QPushButton *startReconstructionButton;
    QSlider *valueSlider;
};
//! [0]

#endif
