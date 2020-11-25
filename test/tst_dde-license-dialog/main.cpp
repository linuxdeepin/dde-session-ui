#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>

#define private public

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    qDebug() << "start dde-license-dialog test cases ..............";
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    qDebug() << "end dde-license-dialog test cases ..............";
    return ret;
}
