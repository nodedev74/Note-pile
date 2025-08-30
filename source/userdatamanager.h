#ifndef USERDATAMANAGER_H
#define USERDATAMANAGER_H

#include <QObject>

struct UserProfile {
    QString uuid;
    QString name;
};

class UserDataManager : public QObject
{
    Q_OBJECT

private:
    explicit UserDataManager(QObject *parent = nullptr);

    QString m_appDataPath;

    UserProfile active_profile;

    void initializeDefaults();

public:
    static UserDataManager& instance();

    QString appDataPath() const { return m_appDataPath; }

    QString notesFilePath() const { return m_appDataPath + "/profiles/" + active_profile.uuid + "/notes"; }
    QString attachmentFilePath() const { return m_appDataPath + "/profiles/" + active_profile.uuid + "/attachments"; }
    QString boardFilePath() const { return m_appDataPath + "/profiles/" + active_profile.uuid + "/boards"; }

    void initialize();

public:
    QJsonValue getGlobalSettingValue(const QString& key);
    void setGlobalSettingValue(const QString& key, const QJsonValue& value);

    QList<UserProfile> listProfiles();
    UserProfile findProfile(const QString& uuid);
    void changeProfile(const QString& uuid);
    QString createProfile(const QString& name);
    void deleteProfile(const QString& uuid);

signals:
    void activeProfileChanged(UserProfile oldProfile, UserProfile newProfile);

};

#endif // USERDATAMANAGER_H
