#include "table.h"
#include <QMap>
#include <cassert>

namespace Table
{
    static const char * const tableNames[] = {
        "Id",
        "Account",
        "Sign"
    };

    QString GetTableName(int i)
    {
        assert(i > 0 && i < GetArraySize(tableNames));
        return tableNames[i];
    }

    QString GetTableName(const Table &table)
    {
        return GetTableName(table.Type());
    }

    QMap<QString, int> Table::NameToIdxMap() const
    {
        static QMap<QString, int> map;
        if(!map.isEmpty())
            return map;
        QStringList stringList = ColNameList();
        int size = stringList.size();
        for(int i = 0; i < size; ++i)
        {
            map[stringList[i]] = i;
        }
        return map;
    }

}

QString Table::Id::getId() const
{
    return id;
}

void Table::Id::setId(const QString &value)
{
    id = value;
}

//int Table::Id::GetIth(QString name) const
//{
//    static QMap<QString, int> map = {{"id", 0}};
//    return map.contains(name) ? map.value(name) : -1;
//}

//QString Table::Id::GetColName(int i) const
//{
//    static QString map[] = {"id"};
//    return map[i];
//}

//QStringList Table::Id::GetColNames() const
//{
//    static const QStringList list = {"id"};
//    return list;
//}

QString Table::Account::getId() const
{
    return id;
}

void Table::Account::setId(const QString &value)
{
    id = value;
}

QString Table::Account::getPwd() const
{
    return pwd;
}

void Table::Account::setPwd(const QString &value)
{
    pwd = value;
}

QString Table::Sign::getId() const
{
    return id;
}

void Table::Sign::setId(const QString &value)
{
    id = value;
}

QString Table::Sign::getSig() const
{
    return sig;
}

void Table::Sign::setSig(const QString &value)
{
    sig = value;
}
