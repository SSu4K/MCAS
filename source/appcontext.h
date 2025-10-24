#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QPointer>
#include <QSettings>
#include <memory>

// Forward declarations until actual implementations are present
class MicrocodeModel;
class JumpTableModel;
class MemoryModel;

class AppContext : public QObject
{
    Q_OBJECT

public:
    static AppContext* instance();

    MicrocodeModel* microcodeModel() const { return m_microcodeModel; }
    JumpTableModel* jumpTableModel() const { return m_jumpTableModel; }

    QSettings* settings() const { return m_settings.get(); }

private:
    explicit AppContext(QObject* parent = nullptr);
    ~AppContext() override = default;

    static QPointer<AppContext> s_instance;

    MicrocodeModel* m_microcodeModel = nullptr;
    JumpTableModel* m_jumpTableModel = nullptr;
    std::unique_ptr<QSettings> m_settings;
};
#endif // APPCONTEXT_H
