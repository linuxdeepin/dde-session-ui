#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>

#define private public

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc,argv);
    qDebug() << "start dde-touchscreen-dialog test cases ..............";
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    qDebug() << "end dde-touchscreen-dialog test cases ..............";
    return ret;
}
