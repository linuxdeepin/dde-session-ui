#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

class WallpaperListView;
class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(QFrame *parent = 0);
    ~Frame();

protected:
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    WallpaperListView *m_wallpaperList;

    void initSize();
    void initListView();
};

#endif // FRAME_H
