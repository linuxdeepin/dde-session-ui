#include <QApplication>
#include <QDebug>

#include <gtest/gtest.h>
#ifdef QT_DEBUG
#include <sanitizer/asan_interface.h>
#endif

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc,argv);
    qDebug() << "start dde-full-filesystem test cases ..............";
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    qDebug() << "end dde-full-filesystem  test cases ..............";

#ifdef QT_DEBUG
    __sanitizer_set_report_path("asan_dde-full-filesystem.log");
#endif

    return ret;
}
