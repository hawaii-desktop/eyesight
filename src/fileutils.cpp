#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "fileutils.h"

FileUtils::FileUtils(QObject *parent) :
    QObject(parent)
{
    exist = false;

    //the directory hasn't been explored since no file has been setted
    dirLoaded = false;

    //
    files.clear();

    emptyDir = true;
    sortBy = FileUtils::Name;

    connect(this, SIGNAL(filePathChanged(QString)), this, SLOT(newPath(QString)));
}

bool FileUtils::openFile(const QString fileName)
{
    dirLoaded = false;
    qDebug()<<"open file:"<<fileName;
    //check if the new file name is valid
    if(fileName.isEmpty() || fileName.isNull()){
        qDebug()<<tr("Empty file name, i can't do anything");
        return false;
    }

    if(!QFile(fileName).exists()){
        return false;
    }

    if(QFileInfo(fileName) != currentFilePath)
    {
        emit filePathChanged(QFileInfo(fileName).absolutePath());
    }

    this->clear();
    this->setFileInfo(fileName);
    bool d = this->sortAndGetFiles(currentFilePath);
    if(d){
        dirLoaded = true;
        this->setCurrentPosition();
    }

    return true;
}

bool FileUtils::openFolder(const QString folder)
{
    dirLoaded = false;
    qDebug()<<"open folder:"<<folder;
    if(folder.isEmpty() || folder.isNull()) {
        qDebug()<<tr("Empty path, i can't do anything");
        return false;
    }

    else if(!QDir(folder).exists()){
        return false;
    }

    else if(QDir(folder).absolutePath() == currentFilePath){
        qDebug()<<tr("The folder is the same than the current");
        return false;
    }

    else{
        this->clear();
        bool d = sortAndGetFiles(QDir(folder).absolutePath());
        if(d){
            currentPosition = 0;
            currentFileName = files.at(0);
            currentFilePath = QDir(folder).absolutePath();
            completeFileName = currentFilePath + QDir::separator() + currentFileName;
            qDebug()<<currentFilePath<<currentFileName;
            dirLoaded = true;
            emit filePathChanged(currentFilePath);
        }
    }

    return true;
}

void FileUtils::reload()
{
    this->sortAndGetFiles(currentFilePath);
    this->setCurrentPosition();
}

bool FileUtils::sortAndGetFiles(QString path)
{
    files.clear();

    QDir dir(path);
    files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::SortFlags(sortFalg()));

    return !files.isEmpty();
}

void FileUtils::setSorting(Order sort)
{
    sortBy = sort;

    //if the folder has been explored, re explore it, else, do nothing
    if(dirLoaded){
        this->reload();
    }
}

void FileUtils::newPath(QString d)
{
    qDebug()<<"new path:"<<d;
}

void FileUtils::setCurrentPosition()
{
    if(files.isEmpty()){
        return;
    }

    currentPosition = files.indexOf(currentFileName);
    qDebug()<<"set position";
    qDebug()<<"position of"<<currentFileName<<":"<<currentPosition;
}

bool FileUtils::goTo(Operation operation, int position)
{
    int amountOfFiles = files.size();
    if(amountOfFiles == 1){
      qDebug()<<"only one file in folder";
      return false;
    }

    //qDebug()<<operation;
    QString temp;
    switch(operation){
    case Next:
        if((currentPosition + 1) == amountOfFiles){
          currentPosition = 0;
          temp=files.at(0);
        }
        else{
          temp=files.at(currentPosition + 1);
          currentPosition += 1;
        }
        break;
    case Previous:
        if((currentPosition) == 0){
          temp=files.at(amountOfFiles - 1);
          currentPosition = amountOfFiles - 1;
        }
        else{
          temp=files.at(currentPosition - 1);
          currentPosition -= 1;
        }
        break;
    case Last:
        if((currentPosition + 1) == amountOfFiles){
	  qDebug()<<"this is already the last one";
          return false;
        }
        else{
          temp=files.at(amountOfFiles - 1);
          currentPosition = amountOfFiles - 1;
        }
        break;
    case First:
        if(currentPosition == 0){
	  qDebug()<<"this is already the first one";
          return false;
        }
        else{
          temp=files.at(0);
          currentPosition = 0;
        }
        break;
    case UserDefined:
        if(position == currentPosition){
            return false;
        }
        if(position < 0 || position == amountOfFiles){
            return false;
        }
        else{
            temp = files.at(position);
            currentPosition = position;
        }
        break;
    }

    currentFileName = temp;
    temp.prepend(currentFilePath + "/");
    completeFileName = temp;
    return true;
}

void FileUtils::setNameFilters(const QStringList filters)
{
    nameFilters = filters;
}

void FileUtils::clear()
{
    currentFileName.clear();
    currentFilePath.clear();
    completeFileName.clear();
    files.clear();
    currentPosition = 0;
    exist = false;
    dirLoaded = false;
    emptyDir = true;
}

bool FileUtils::goToFile(Operation operation, int position)
{
    return goTo(operation,position);
}

void FileUtils::setFileInfo(const QString name)
{
    QFileInfo f(name);
    currentFileName = f.fileName();
    currentFilePath = f.absolutePath();
    completeFileName = f.absoluteFilePath();
    f.~QFileInfo();
    //qDebug()<<currentFileName<<currentFilePath;
}

bool FileUtils::moveFile(const QString path)
{
    if(currentFilePath == QDir(path).path()){
        qDebug()<<tr("The image wasn't moved\nThe new folder is iqual to the older");
        newFolder = false;
        return false;
    }

    //the new file name
    QString newFileName = path + QDir::separator() + getFileName();

    //check if there is an existing file with the same name
    if(QFile(newFileName).exists()){
        //ask if wish to overwrite
        int result = QMessageBox::question(0, tr("Overwrite?"),
                    tr("File %1 already exists. Do you want to overwrite it?").arg(getFileName()),
                    QMessageBox::Yes|QMessageBox::No);

        //if doesn't want return;
        if (result == QMessageBox::No){
            return false;
        }

        //else remove the previous file
        QFile::remove(newFileName);
    }

    bool d = QFile(absoluteFilePath()).rename(newFileName);
    if(d){
        this->openFile(newFileName);
    }

    return d;
}

bool FileUtils::deleteFile()
{
    bool d = false;

    d = QFile::remove(completeFileName);
    if(!d){
        return d;
    }

    files.removeAt(currentPosition);
    if(files.isEmpty()){
        currentFileName.clear();
        currentFilePath.clear();
        completeFileName.clear();
        emptyDir = true;
    }
    else{
        currentPosition--;
        goTo(FileUtils::Next);
    }

    return d;
}

bool FileUtils::goToFile(const QString name, const int position)
{
    int gtPos;

    if(files.contains(name)){
        gtPos = files.lastIndexOf(name);
    }
    else{
        gtPos = position;
    }

    return goTo(FileUtils::UserDefined, gtPos);
}

int FileUtils::sortFalg()
{
    QDir::SortFlags f;
    switch(sortBy){
    case Name:
        f = QDir::Name|QDir::IgnoreCase;
        break;
    case Type:
        f = QDir::Type;
        break;
    case Time:
        f = QDir::Time;
        break;
    case Size:
        f = QDir::Size;
        break;
    default:
        f = QDir::Name|QDir::IgnoreCase;
        break;
    }

    return int(f);
}
