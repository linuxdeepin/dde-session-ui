#ifndef SIGNALMANAGER
#define SIGNALMANAGER
#include <QObject>

class SignalManager: public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject* parent=0);
    ~SignalManager();
    static SignalManager* getInstance();
signals:
    void setButtonCheck(QString buttonId);
    void setButtonHover(QString buttonId);
    void buttonStyleChanged();
private:
    static SignalManager* m_signalManager;

};
static SignalManager* signalManager = SignalManager::getInstance();
#endif // SIGNALMANAGER

