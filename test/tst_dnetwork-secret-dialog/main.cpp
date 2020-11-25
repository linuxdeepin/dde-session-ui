#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>

#define private public

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    qDebug() << "start dnetwork-secret-dialog test cases ..............";
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    qDebug() << "end dnetwork-secret-dialog test cases ..............";
    return ret;
}
