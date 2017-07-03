
#ifndef __AUTO_GENERATED_DBUS__
#define __AUTO_GENERATED_DBUS__
#include <QtDBus>

namespace dbus {
	namespace common {

template<int index, typename T1=uchar, typename T2=uchar, typename T3=uchar, typename T4=uchar, typename T5=uchar, typename T6=uchar, typename T7=uchar, typename T8=uchar>
struct SelectBase {
    typedef void Type;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<0, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T1 Type;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<1, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T2 Type;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<2, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T3 Type;
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<3, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T4 Type;
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<4, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T5 Type;
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<5, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T6 Type;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct SelectBase<-1, T1, T2, T3, T4, T5, T6, T7, T8> {
    typedef T7 Type;
};



typedef QVariant (*DataConverter)(QVariant);

inline QVariant NormalConverter(QVariant v)
{
    return v;
}
inline QVariant PropertyConverter(QVariant v)
{
    QDBusVariant vv = v.value<QDBusVariant>();
    return vv.variant();
}


template<typename T1=uchar, typename T2=uchar, typename T3=uchar, typename T4=uchar, typename T5=uchar, typename T6=uchar, typename T7=uchar, typename T8=uchar> class R
    : public QDBusPendingReply<> {
    template<int index>
    struct Select: SelectBase<index, T1, T2, T3, T4, T5, T6, T7, QDBusError> {
    };
private:
    void waitForFinished() {
        QDBusPendingReply::waitForFinished();
	if (!isValid() || isError()) {
            m_hasError = true;
            m_error = error();
	    return;
        }
        m_hasError = false;
        m_error = QDBusError();
    }
    QDBusError m_error;
    DataConverter m_converter;
    bool m_hasError;
public:
    R(QDBusPendingReply<> r, DataConverter c=NormalConverter):
        QDBusPendingReply(r), m_converter(c),m_hasError(false)
    {
    }

    bool hasError() {
        if (!isFinished()) {
            waitForFinished();
        }
        return m_hasError;
    }
    QDBusError Error() {
        if (!isFinished()) {
            waitForFinished();
        }
        return m_error;
    }

    template<int index>
    typename Select<index>::Type Value() {
        if (!isFinished()) {
            waitForFinished();
            if (m_hasError) {
                return typename Select<index>::Type();
            }
        }
        QList<QVariant> args = reply().arguments();
        if (args.size() <= index) {
            m_hasError = true;
            m_error = QDBusError(QDBusError::InvalidArgs, QString("can't fetch the %1th argument, because only up to %2 arguments.").arg(index).arg(args.size()));
            return typename Select<index>::Type();
        }
        QVariant r = args[index];

        return qdbus_cast<typename Select<index>::Type>(m_converter(r));
    }

    QList<QVariant> Values() {
        QList<QVariant> ret;

        if (!isFinished()) {
            waitForFinished();
            if (m_hasError) {
                return ret;
            }
        }

        QList<QVariant> args = reply().arguments();

        switch (args.size()) {
            case 8:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T8>(m_converter(args[7]))));
            case 7:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T7>(m_converter(args[6]))));
            case 6:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T6>(m_converter(args[5]))));
            case 5:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T5>(m_converter(args[4]))));
            case 4:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T4>(m_converter(args[3]))));
            case 3:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T3>(m_converter(args[2]))));
            case 2:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T2>(m_converter(args[1]))));
            case 1:
		    ret.push_front(QVariant::fromValue(qdbus_cast<T1>(m_converter(args[0]))));
        }
        return ret;
    }

};



static QDBusConnection detectConnection(QString addr) {
    if (addr == "session") {
	    return QDBusConnection::sessionBus();
    } else if (addr == "system") {
	    return QDBusConnection::systemBus();
    } else {
            qDebug() << "W: Are you sure using '" << addr << "' as the private dbus session?";
	    return *(new QDBusConnection(addr));
    }
}

class DBusObject: public QDBusAbstractInterface {
    Q_OBJECT
public:
    DBusObject(QObject* parent, QString service, QString path, const char* interface, const QString addr);
    ~DBusObject();

    Q_SLOT void propertyChanged(const QDBusMessage& msg);
protected:
    QDBusPendingReply<> fetchProperty(const char* name);

};



inline
DBusObject::DBusObject(QObject* parent, QString service, QString path, const char* interface, const QString addr)
:QDBusAbstractInterface(service, path, interface, detectConnection(addr), parent)
{
        if (!isValid()) {
            qDebug() << "The remote dbus object may not exists, try launch it. " << lastError();
        }
	connection().connect(this->service(), this->path(), "org.freedesktop.DBus.Properties",  "PropertiesChanged",
	    "sa{sv}as", this, SLOT(propertyChanged(QDBusMessage)));
}

inline
DBusObject::~DBusObject()
{
	connection().disconnect(service(), path(), interface(),  "PropertiesChanged",
            "sa{sv}as", this, SLOT(propertyChanged(QDBusMessage)));
}

inline
QDBusPendingReply<> DBusObject::fetchProperty(const char* name)
{
    QDBusMessage msg = QDBusMessage::createMethodCall(service(), path(),
            QLatin1String("org.freedesktop.DBus.Properties"),
            QLatin1String("Get"));
    msg << interface() << QString::fromUtf8(name);

    QDBusPendingReply<> r = connection().asyncCall(msg);

    return r;
}

inline
void DBusObject::propertyChanged(const QDBusMessage& msg)
{
    QList<QVariant> arguments = msg.arguments();
    if (3 != arguments.count())
        return;

    QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
    Q_FOREACH(const QString &prop, changedProps.keys()) {
        const QMetaObject* self = metaObject();
        for (int i=self->propertyOffset(); i < self->propertyCount(); ++i) {
            QMetaProperty p = self->property(i);
            if (p.name() == prop) {
                Q_EMIT p.notifySignal().invoke(this);
            }
        }
    }
}

	}
	namespace types {

	template<typename T1, typename T2=char, typename T3=char, typename T4=char, typename T5=char, typename T6=char, typename T7=char, typename T8=char, typename T9=char, typename T10=char, typename T11=char, typename T12=char, typename T13=char, typename T14=char>
	struct BaseStruct {
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;
		T6 m6;
		T7 m7;
		T8 m8;
		T9 m9;
		T10 m10;
		T11 m11;
		T12 m12;
		T13 m13;
		T14 m14;
	};
	int getTypeId(const QString& s);


	typedef QStringList  as;

	typedef QList<QDBusObjectPath > ao;

	typedef dbus::types::BaseStruct<QString, QString, QString > r_sss_;

	typedef QList<dbus::types::BaseStruct<QString, QString, QString > > ar_sss_;

	typedef dbus::types::BaseStruct<QString, QString, QString, QString, QString > r_sssss_;

	typedef QList<dbus::types::BaseStruct<QString, QString, QString, QString, QString > > ar_sssss_;



	inline QDBusArgument& operator<<(QDBusArgument &argument, const dbus::types::as& v)
	{
		argument.beginArray(getTypeId("s"));
for (int i=0; i < v.size(); ++i)
    argument << v.at(i);
argument.endArray();
return argument;

	}
	inline const QDBusArgument& operator>>(const QDBusArgument &argument, dbus::types::as& v)
	{
		argument.beginArray();
while (!argument.atEnd()) {
    QString ele;
    argument >> ele;
    v.append(ele);
}
argument.endArray();
return argument;

	}
	inline QDBusArgument& operator<<(QDBusArgument &argument, const dbus::types::ao& v)
	{
		argument.beginArray(getTypeId("o"));
for (int i=0; i < v.size(); ++i)
    argument << v.at(i);
argument.endArray();
return argument;

	}
	inline const QDBusArgument& operator>>(const QDBusArgument &argument, dbus::types::ao& v)
	{
		argument.beginArray();
while (!argument.atEnd()) {
    QDBusObjectPath ele;
    argument >> ele;
    v.append(ele);
}
argument.endArray();
return argument;

	}
	inline QDBusArgument& operator<<(QDBusArgument &argument, const dbus::types::r_sss_& v)
	{
		argument.beginStructure();
argument << v.m1 << v.m2 << v.m3;
argument.endStructure();
return argument;

	}
	inline const QDBusArgument& operator>>(const QDBusArgument &argument, dbus::types::r_sss_& v)
	{
		argument.beginStructure();
argument >> v.m1 >> v.m2 >> v.m3;
argument.endStructure();
return argument;

	}
	inline QDBusArgument& operator<<(QDBusArgument &argument, const dbus::types::ar_sss_& v)
	{
		argument.beginArray(getTypeId("(sss)"));
for (int i=0; i < v.size(); ++i)
    argument << v.at(i);
argument.endArray();
return argument;

	}
	inline const QDBusArgument& operator>>(const QDBusArgument &argument, dbus::types::ar_sss_& v)
	{
		argument.beginArray();
while (!argument.atEnd()) {
    dbus::types::BaseStruct<QString, QString, QString > ele;
    argument >> ele;
    v.append(ele);
}
argument.endArray();
return argument;

	}
	inline QDBusArgument& operator<<(QDBusArgument &argument, const dbus::types::r_sssss_& v)
	{
		argument.beginStructure();
argument << v.m1 << v.m2 << v.m3 << v.m4 << v.m5;
argument.endStructure();
return argument;

	}
	inline const QDBusArgument& operator>>(const QDBusArgument &argument, dbus::types::r_sssss_& v)
	{
		argument.beginStructure();
argument >> v.m1 >> v.m2 >> v.m3 >> v.m4 >> v.m5;
argument.endStructure();
return argument;

	}
	inline QDBusArgument& operator<<(QDBusArgument &argument, const dbus::types::ar_sssss_& v)
	{
		argument.beginArray(getTypeId("(sssss)"));
for (int i=0; i < v.size(); ++i)
    argument << v.at(i);
argument.endArray();
return argument;

	}
	inline const QDBusArgument& operator>>(const QDBusArgument &argument, dbus::types::ar_sssss_& v)
	{
		argument.beginArray();
while (!argument.atEnd()) {
    dbus::types::BaseStruct<QString, QString, QString, QString, QString > ele;
    argument >> ele;
    v.append(ele);
}
argument.endArray();
return argument;

	}

	inline int getTypeId(const QString& s) {
	if (0) {
	}  else if (s == "as") {
		return qDBusRegisterMetaType<dbus::types::as>();
	} else if (s == "ao") {
		return qDBusRegisterMetaType<dbus::types::ao>();
	} else if (s == "(sss)") {
		return qDBusRegisterMetaType<dbus::types::r_sss_>();
	} else if (s == "a(sss)") {
		return qDBusRegisterMetaType<dbus::types::ar_sss_>();
	} else if (s == "(sssss)") {
		return qDBusRegisterMetaType<dbus::types::r_sssss_>();
	} else if (s == "a(sssss)") {
		return qDBusRegisterMetaType<dbus::types::ar_sssss_>();
	}
	}

	}
	namespace objects {

namespace com {namespace deepin {namespace lastore {

class Job : public dbus::common::DBusObject
{
	Q_OBJECT
	private:
	static const char *defaultService() { return "com.deepin.lastore";}
	static const QDBusObjectPath defaultPath() { return QDBusObjectPath("/com/deepin/lastore/Job");}
	public:
        Job(QString addr="session", QObject* parent=0)
        :DBusObject(parent, defaultService(), defaultPath().path(), "com.deepin.lastore.Job", addr)
        {
        }
	Job(QString addr, QString service, QString path, QObject* parent=0)
	:DBusObject(parent, service, path, "com.deepin.lastore.Job", addr)
	{
	}
	~Job(){}


	Q_PROPERTY(dbus::common::R<QString > Id READ id NOTIFY idChanged)
	dbus::common::R<QString > id () {
		QDBusPendingReply<> call = fetchProperty("Id");
		return dbus::common::R<QString >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<QString > Name READ name NOTIFY nameChanged)
	dbus::common::R<QString > name () {
		QDBusPendingReply<> call = fetchProperty("Name");
		return dbus::common::R<QString >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<dbus::types::as > Packages READ packages NOTIFY packagesChanged)
	dbus::common::R<dbus::types::as > packages () {
		QDBusPendingReply<> call = fetchProperty("Packages");
		return dbus::common::R<dbus::types::as >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<QString > Type READ type NOTIFY typeChanged)
	dbus::common::R<QString > type () {
		QDBusPendingReply<> call = fetchProperty("Type");
		return dbus::common::R<QString >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<QString > Status READ status NOTIFY statusChanged)
	dbus::common::R<QString > status () {
		QDBusPendingReply<> call = fetchProperty("Status");
		return dbus::common::R<QString >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<double > Progress READ progress NOTIFY progressChanged)
	dbus::common::R<double > progress () {
		QDBusPendingReply<> call = fetchProperty("Progress");
		return dbus::common::R<double >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<qlonglong > Speed READ speed NOTIFY speedChanged)
	dbus::common::R<qlonglong > speed () {
		QDBusPendingReply<> call = fetchProperty("Speed");
		return dbus::common::R<qlonglong >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<QString > Description READ description NOTIFY descriptionChanged)
	dbus::common::R<QString > description () {
		QDBusPendingReply<> call = fetchProperty("Description");
		return dbus::common::R<QString >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<bool > Cancelable READ cancelable NOTIFY cancelableChanged)
	dbus::common::R<bool > cancelable () {
		QDBusPendingReply<> call = fetchProperty("Cancelable");
		return dbus::common::R<bool >(call, dbus::common::PropertyConverter);
	}





	Q_SIGNALS:



	void idChanged ();
	void nameChanged ();
	void packagesChanged ();
	void typeChanged ();
	void statusChanged ();
	void progressChanged ();
	void speedChanged ();
	void descriptionChanged ();
	void cancelableChanged ();

};
}}}

namespace com {namespace deepin {namespace lastore {

class Manager : public dbus::common::DBusObject
{
	Q_OBJECT
	private:
	static const char *defaultService() { return "com.deepin.lastore.Manager";}
	static const QDBusObjectPath defaultPath() { return QDBusObjectPath("/com/deepin/lastore/Manager");}
	public:
        Manager(QString addr="session", QObject* parent=0)
        :DBusObject(parent, defaultService(), defaultPath().path(), "com.deepin.lastore.Manager", addr)
        {
        }
	Manager(QString addr, QString service, QString path, QObject* parent=0)
	:DBusObject(parent, service, path, "com.deepin.lastore.Manager", addr)
	{
	}
	~Manager(){}


	Q_PROPERTY(dbus::common::R<dbus::types::ao > JobList READ jobList NOTIFY jobListChanged)
	dbus::common::R<dbus::types::ao > jobList () {
		QDBusPendingReply<> call = fetchProperty("JobList");
		return dbus::common::R<dbus::types::ao >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<dbus::types::as > SystemArchitectures READ systemArchitectures NOTIFY systemArchitecturesChanged)
	dbus::common::R<dbus::types::as > systemArchitectures () {
		QDBusPendingReply<> call = fetchProperty("SystemArchitectures");
		return dbus::common::R<dbus::types::as >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<dbus::types::as > UpgradableApps READ upgradableApps NOTIFY upgradableAppsChanged)
	dbus::common::R<dbus::types::as > upgradableApps () {
		QDBusPendingReply<> call = fetchProperty("UpgradableApps");
		return dbus::common::R<dbus::types::as >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<bool > SystemOnChanging READ systemOnChanging NOTIFY systemOnChangingChanged)
	dbus::common::R<bool > systemOnChanging () {
		QDBusPendingReply<> call = fetchProperty("SystemOnChanging");
		return dbus::common::R<bool >(call, dbus::common::PropertyConverter);
	}






	dbus::common::R<void> PauseJob (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("PauseJob"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<void> CleanJob (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("CleanJob"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<QDBusObjectPath> DistUpgrade () {
		QList<QVariant> argumentList;
		;
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("DistUpgrade"), argumentList);
		return dbus::common::R<QDBusObjectPath>(call);
	}





	dbus::common::R<QDBusObjectPath> PrepareDistUpgrade () {
		QList<QVariant> argumentList;
		;
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("PrepareDistUpgrade"), argumentList);
		return dbus::common::R<QDBusObjectPath>(call);
	}





	dbus::common::R<QDBusObjectPath> InstallPackage (QString arg0, QString arg1) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0) << QVariant::fromValue(arg1);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("InstallPackage"), argumentList);
		return dbus::common::R<QDBusObjectPath>(call);
	}





	dbus::common::R<QString> PackageDesktopPath (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("PackageDesktopPath"), argumentList);
		return dbus::common::R<QString>(call);
	}





	dbus::common::R<bool> PackageInstallable (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("PackageInstallable"), argumentList);
		return dbus::common::R<bool>(call);
	}





	dbus::common::R<bool> PackageExists (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("PackageExists"), argumentList);
		return dbus::common::R<bool>(call);
	}





	dbus::common::R<qlonglong> PackagesDownloadSize (dbus::types::as arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("PackagesDownloadSize"), argumentList);
		return dbus::common::R<qlonglong>(call);
	}





	dbus::common::R<QDBusObjectPath> RemovePackage (QString arg0, QString arg1) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0) << QVariant::fromValue(arg1);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("RemovePackage"), argumentList);
		return dbus::common::R<QDBusObjectPath>(call);
	}





	dbus::common::R<void> SetRegion (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("SetRegion"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<void> RecordLocaleInfo (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("RecordLocaleInfo"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<void> StartJob (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("StartJob"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<QDBusObjectPath> UpdatePackage (QString arg0, QString arg1) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0) << QVariant::fromValue(arg1);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("UpdatePackage"), argumentList);
		return dbus::common::R<QDBusObjectPath>(call);
	}





	dbus::common::R<QDBusObjectPath> UpdateSource () {
		QList<QVariant> argumentList;
		;
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("UpdateSource"), argumentList);
		return dbus::common::R<QDBusObjectPath>(call);
	}




	Q_SIGNALS:



	void jobListChanged ();
	void systemArchitecturesChanged ();
	void upgradableAppsChanged ();
	void systemOnChangingChanged ();

};
}}}

namespace com {namespace deepin {namespace lastore {

class Updater : public dbus::common::DBusObject
{
	Q_OBJECT
	private:
	static const char *defaultService() { return "com.deepin.lastore.Updater";}
	static const QDBusObjectPath defaultPath() { return QDBusObjectPath("/com/deepin/lastore/Updater");}
	public:
        Updater(QString addr="session", QObject* parent=0)
        :DBusObject(parent, defaultService(), defaultPath().path(), "com.deepin.lastore.Updater", addr)
        {
        }
	Updater(QString addr, QString service, QString path, QObject* parent=0)
	:DBusObject(parent, service, path, "com.deepin.lastore.Updater", addr)
	{
	}
	~Updater(){}


	Q_PROPERTY(dbus::common::R<bool > AutoCheckUpdates READ autoCheckUpdates NOTIFY autoCheckUpdatesChanged)
	dbus::common::R<bool > autoCheckUpdates () {
		QDBusPendingReply<> call = fetchProperty("AutoCheckUpdates");
		return dbus::common::R<bool >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<bool > AutoDownloadUpdates READ autoDownloadUpdates NOTIFY autoDownloadUpdatesChanged)
	dbus::common::R<bool > autoDownloadUpdates () {
		QDBusPendingReply<> call = fetchProperty("AutoDownloadUpdates");
		return dbus::common::R<bool >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<QString > MirrorSource READ mirrorSource NOTIFY mirrorSourceChanged)
	dbus::common::R<QString > mirrorSource () {
		QDBusPendingReply<> call = fetchProperty("MirrorSource");
		return dbus::common::R<QString >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<dbus::types::as > UpdatableApps READ updatableApps NOTIFY updatableAppsChanged)
	dbus::common::R<dbus::types::as > updatableApps () {
		QDBusPendingReply<> call = fetchProperty("UpdatableApps");
		return dbus::common::R<dbus::types::as >(call, dbus::common::PropertyConverter);
	}

	Q_PROPERTY(dbus::common::R<dbus::types::as > UpdatablePackages READ updatablePackages NOTIFY updatablePackagesChanged)
	dbus::common::R<dbus::types::as > updatablePackages () {
		QDBusPendingReply<> call = fetchProperty("UpdatablePackages");
		return dbus::common::R<dbus::types::as >(call, dbus::common::PropertyConverter);
	}






	dbus::common::R<dbus::types::ar_sssss_> ApplicationUpdateInfos (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("ApplicationUpdateInfos"), argumentList);
		return dbus::common::R<dbus::types::ar_sssss_>(call);
	}





	dbus::common::R<dbus::types::ar_sss_> ListMirrorSources (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("ListMirrorSources"), argumentList);
		return dbus::common::R<dbus::types::ar_sss_>(call);
	}





	dbus::common::R<void> SetAutoCheckUpdates (bool arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("SetAutoCheckUpdates"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<void> SetAutoDownloadUpdates (bool arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("SetAutoDownloadUpdates"), argumentList);
                return dbus::common::R<void>(call);
	}





	dbus::common::R<void> SetMirrorSource (QString arg0) {
		QList<QVariant> argumentList;
		argumentList << QVariant::fromValue(arg0);
		QDBusPendingReply<> call = asyncCallWithArgumentList(QLatin1String("SetMirrorSource"), argumentList);
                return dbus::common::R<void>(call);
	}




	Q_SIGNALS:



	void autoCheckUpdatesChanged ();
	void autoDownloadUpdatesChanged ();
	void mirrorSourceChanged ();
	void updatableAppsChanged ();
	void updatablePackagesChanged ();

};
}}}

	}
}

Q_DECLARE_METATYPE(dbus::types::as);
Q_DECLARE_METATYPE(dbus::types::ao);
Q_DECLARE_METATYPE(dbus::types::r_sss_);
Q_DECLARE_METATYPE(dbus::types::ar_sss_);
Q_DECLARE_METATYPE(dbus::types::r_sssss_);
Q_DECLARE_METATYPE(dbus::types::ar_sssss_);

#endif
