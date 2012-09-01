#ifndef WEBPDECODER_H
#define WEBPDECODER_H

#include <QWidget>

class WebpDecoder : public QWidget
{
    Q_OBJECT

public:
    /**
      *constructor
      */
    WebpDecoder(QWidget *parent = 0);

    /**
      *pass the file \a name to decode, returns true if sucess, otherwise return false
      */
    bool setFile(const QString name);

    /**
      * @return the image passed as an QPixmap object
      */
    inline QPixmap getPixmap() const {
        return pixmap;
    }

private:
    QPixmap pixmap;
    QString name;

    bool decodeWebpImage(const QString fileName);
    bool readImage(unsigned char *rgb, int width, int height, QPixmap &pixmap);

};

#endif // WEBPDECODER_H
