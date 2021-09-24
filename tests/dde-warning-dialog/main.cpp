#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>

#ifdef QT_DEBUG
#include <sanitizer/asan_interface.h>
#endif

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc,argv);
    qDebug() << "start dde-warning-dialog test cases ..............";
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    qDebug() << "end dde-warning-dialog test cases ..............";
    
#ifdef QT_DEBUG
    __sanitizer_set_report_path("asan_dde-warning-dialog.log");
#endif

    return ret;
}
