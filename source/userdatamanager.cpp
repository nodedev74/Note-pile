#include "userdatamanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUuid>
#include <QSettings>

UserDataManager::UserDataManager(QObject *parent)
    : QObject{parent}
{
    m_appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

UserDataManager& UserDataManager::instance() {
    static UserDataManager inst;
    return inst;
}

QJsonValue UserDataManager::getGlobalSettingValue(const QString& key)
{
    QString settingsPath = m_appDataPath + "/settings.json";

    QFile file(settingsPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << settingsPath;
        return QJsonValue();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "JSON is not an object!";
        return QJsonValue();
    }

    QJsonObject obj = doc.object();
    return obj.value(key).toString();
}

void UserDataManager::setGlobalSettingValue(const QString& key, const QJsonValue& value)
{
    QString settingsPath = m_appDataPath + "/settings.json";

    QFile file(settingsPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << settingsPath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root;
    if (doc.isObject()) {
        root = doc.object();
    } else {
        qWarning() << "JSON is not an object, creating a new one.";
    }

    root[key] = value;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Cannot open file for writing:" << settingsPath;
        return;
    }

    QJsonDocument newDoc(root);
    file.write(newDoc.toJson(QJsonDocument::Indented));
    file.close();
}

QList<UserProfile> UserDataManager::listProfiles()
{
    QList<UserProfile> profiles;
    QDir dir(m_appDataPath + "/profiles");

    if(dir.exists()) {
        QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QString &d : std::as_const(dirs)) {
            QString profilePath = dir.absoluteFilePath(d + "/.profile");

            if (QFile::exists(profilePath)) {
                QSettings settings(profilePath, QSettings::IniFormat);
                QString name = settings.value("NAME").toString();

                if(!name.isEmpty()) {
                    UserProfile profile;
                    profile.uuid = d;
                    profile.name = name;
                    profiles.append(profile);
                } else {
                    qWarning() << "Corrupted profile (missing NAME) at" << profilePath;
                }
            }
        }
    }
    return profiles;
}

UserProfile UserDataManager::findProfile(const QString& uuid)
{
    QList<UserProfile> profiles = listProfiles();
    for(UserProfile profile : std::as_const(profiles)) {
        if(profile.uuid == uuid) {
            return profile;
        }
    }
    return UserProfile();
}

void UserDataManager::initializeDefaults()
{
    QDir().mkdir(m_appDataPath + "/profiles");

    if(!QFile().exists(m_appDataPath + "/settings.json")) {
        QList<UserProfile> profiles = listProfiles();

        QJsonObject root;
        if(!profiles.empty()) {
            root.insert("profile", profiles.first().uuid);
        } else {
            root.insert("profile", createProfile("default"));
        }

        QFile jsonFile(m_appDataPath + "/settings.json");
        QJsonDocument document(root);

        if (jsonFile.open(QFile::WriteOnly)) {
            jsonFile.write(document.toJson());
            jsonFile.close();
        } else {
            qWarning() << "Could not write settings.json";
        }
    }
}

void UserDataManager::initialize()
{
    initializeDefaults();

    // restore last active profile OR fallback to first profile in list

    QString last_profile  = getGlobalSettingValue("profile").toString();

    if(last_profile.isEmpty()) {
        last_profile = listProfiles().first().uuid;
    }
    changeProfile(last_profile);
}

QString UserDataManager::createProfile(const QString& name)
{
    // profile dir

    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QDir().mkdir(m_appDataPath + "/profiles/" + uuid);

    // profile settings

    QString profilePath = QDir(m_appDataPath + "/profiles/" + uuid).absoluteFilePath(".profile");
    QSettings settings(profilePath, QSettings::IniFormat);
    settings.setValue("NAME", name);
    settings.sync();

    // profile subdirs

    QDir().mkdir(m_appDataPath + "/profiles/" + uuid + "/notes");
    QDir().mkdir(m_appDataPath + "/profiles/" + uuid + "/attachments");
    QDir().mkdir(m_appDataPath + "/profiles/" + uuid + "/boards");

    // session.json

    QFile sessionFile(m_appDataPath + "/profiles/" + uuid + "/session.json");
    if (sessionFile.open(QFile::WriteOnly)) {
        sessionFile.write(QJsonDocument(QJsonArray()).toJson());
        sessionFile.close();
    } else {
        qWarning() << "Could not write session.json";
    }
    return uuid;
}

void UserDataManager::changeProfile(const QString& uuid)
{
    UserProfile oldProfile = active_profile;
    UserProfile newProfile = findProfile(uuid);

    if (newProfile.uuid.isEmpty()) {
        qWarning() << "No profile with uuid " + uuid +" found.";
        return;
    }

    active_profile = newProfile;
    emit activeProfileChanged(oldProfile, active_profile);
}

void UserDataManager::deleteProfile(const QString& uuid)
{
    // TODO: Implement profile remove
}
