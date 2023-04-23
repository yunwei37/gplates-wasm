// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

//! [0]
#include <QtWidgets>
#include <QtXml/QDomDocument>
#include <iostream>
#include <streambuf>

#include "src/reconstructions.h"

#include "mainwindow.h"
//! [0]

//! [1]
MainWindow::MainWindow()
    : textEdit(new QPlainTextEdit),
      outputLabel(new QLabel)
//! [1] //! [2]
{
    // Initialize Qt resources that exist in the static 'qt-resources' library.
    // NOTE: This is done here so that both the GUI and command-line-only paths have initialized resources.
    //
    // NOTE: According to the QtResources documentation calls to Q_INIT_RESOURCE are not needed if
    // the resources are compiled into a shared library (further, if resources only accessed from
    // within shared library then there's also no issue with the shared library not being loaded yet).
    // So Q_INIT_RESOURCE is not called for the python (API) shared library (since this source
    // file is not included in it) but that's no problem since the python shared library is used
    // externally (ie, used by an external python interpreter not the GPlates embedded interpreter)
    // and so the resources are only accessed internally by the shared library.
    //
    Q_INIT_RESOURCE(opengl);
    Q_INIT_RESOURCE(python);
    Q_INIT_RESOURCE(gpgim);
    Q_INIT_RESOURCE(qt_widgets);

    // 创建一个 QWidget 作为中心窗口部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 使用 QHBoxLayout 将 textEdit 和 outputLabel 水平排列
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(outputLabel);

    // 设置比例因子，使 outputLabel 的宽度占三分之二
    layout->setStretch(0, 1); // textEdit 的比例因子设为 1
    layout->setStretch(1, 2); // outputLabel 的比例因子设为 2

    centralWidget->setLayout(layout);

    // 为 outputLabel 添加边框样式
    outputLabel->setStyleSheet("border: 1px solid black;");
    outputLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    outputStreamBuffer.setOutputLabel(outputLabel);

    createActions();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    qDebug() << QString("Hello from qDebug()");
    std::streambuf *oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(&outputStreamBuffer);
    std::cout << "begin..." << std::endl;
}
//! [2]

void MainWindow::open_and_read_gpml(const QString &filename)
{
    qDebug() << "Attempting to read \"" << filename << "\".";
    std::cout << "read structural types from a GPML file." << std::endl;
    GPlatesModel::Gpgim::instance();
    // Used to read structural types from a GPML file.
    GPlatesFileIO::GpmlPropertyStructuralTypeReader::non_null_ptr_to_const_type gpml_structural_type_reader =
        GPlatesFileIO::GpmlPropertyStructuralTypeReader::create();
    std::cout << "create a new GpmlPropertyStructuralType" << std::endl;

    GPlatesFileIO::FileInfo fileinfo(filename);
    GPlatesModel::ModelInterface new_model;
    GPlatesFileIO::ReadErrorAccumulation accum;
    bool contains_unsaved_changes;

    // Create a file with an empty feature collection.
    GPlatesFileIO::File::non_null_ptr_type file = GPlatesFileIO::File::create_file(fileinfo);

    // Read new features from the file into the empty feature collection.
    GPlatesFileIO::GpmlReader::read_file(
        file->get_reference(),
        gpml_structural_type_reader,
        accum,
        contains_unsaved_changes);

    GPlatesModel::FeatureCollectionHandle::weak_ref features =
        file->get_reference().get_feature_collection();
    ::output_as_gpml(features);
}

void MainWindow::reconstruction()
{
    GPlatesMaths::assert_has_infinity_and_nan();

    GPlatesModel::ModelInterface model;

    std::cout << "read structural types from a GPML file." << std::endl;
    GPlatesModel::Gpgim::instance();
    // Used to read structural types from a GPML file.
    GPlatesFileIO::GpmlPropertyStructuralTypeReader::non_null_ptr_to_const_type gpml_structural_type_reader =
        GPlatesFileIO::GpmlPropertyStructuralTypeReader::create();
    std::cout << "create a new GpmlPropertyStructuralType" << std::endl;

    std::pair<GPlatesModel::FeatureCollectionHandle::weak_ref,
              GPlatesModel::FeatureCollectionHandle::weak_ref>
        isochrons_and_total_recon_seqs =
            ::populate_feature_store(model);
    std::cout << "populate_feature_store in model" << std::endl;

    GPlatesModel::FeatureCollectionHandle::weak_ref isochrons =
        isochrons_and_total_recon_seqs.first;
    GPlatesModel::FeatureCollectionHandle::weak_ref total_recon_seqs =
        isochrons_and_total_recon_seqs.second;

    ::output_as_gpml(isochrons);
    ::output_reconstructions(isochrons, total_recon_seqs);
}

//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    if (maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave())
    {
        textEdit->clear();
        setCurrentFile(QString());
    }
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
//! [8]

//! [9]
bool MainWindow::save()
//! [9] //! [10]
{
    if (curFile.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}
//! [10]

//! [11]
bool MainWindow::saveAs()
//! [11] //! [12]
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>Application</b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu bar, "
                          "toolbars, and a status bar."));
}
//! [14]

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
    setWindowModified(textEdit->document()->isModified());
}
//! [16]

//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    //! [19]
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
    //! [18] //! [19]

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    //! [20]

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    //! [20]
    exitAct->setStatusTip(tr("Exit the application"));

    //! [21]
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
//!
#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    //! [21]
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    //! [22]

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
//! [22]

    QMenu *testMenu = menuBar()->addMenu(tr("&Reconstruction"));
    QAction *testAct = testMenu->addAction(tr("&reconstruction"), this, &MainWindow::reconstruction);
    testAct->setStatusTip(tr("Test reconstruction for the gplates"));

//! [23]
#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    //! [23] //! [24]
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}
//! [24]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Application"),
                                                                 tr("The document has been modified.\n"
                                                                    "Do you want to save your changes?"),
                                                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
//! [41]

//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]

//! [44]
bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit())
        {
            errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    }
    else
    {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
    return QFileInfo(fullFileName).fileName();
}
//! [49]
#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction())
    {
        if (!maybeSave())
            manager.cancel();
    }
    else
    {
        // Non-interactive: save without asking
        if (textEdit->document()->isModified())
            save();
    }
}
#endif
