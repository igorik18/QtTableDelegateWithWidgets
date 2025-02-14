#ifndef COMMON_H
#define COMMON_H

#include <QString>

#define _ICON_HEIGHT 20
#define _LINE_EDIT_HEIGHT 20

// Остальные названия объектов

const QString TEXT_ON_TABLE_DESELECTED = "txtOnTableDeselected";
const QString TEXT_ON_TABLE_SELECTED = "txtOnTableSelected";
const QString LINEEDIT_ON_TABLE = "lineeditOnTable";

// Названия объектов кнопок

const QString BUTTON_EDIT = "btnEdit";

const QString BUTTON_ALLOW = "btnAllow";
const QString BUTTON_DENY = "btnDeny";

enum TypeServerParameter
{
    IntegerTypeServerParameter = 1,
    BoolTypeServerParameter,
    PathToFileTypeServerParameter,
    PathToDirTypeServerParameter,
    IpTypeServerParameter,
    TextTypeServerParameter
};

enum ServerSettingsTableColumn
{
    ServerSettingsTableParameter,
    ServerSettingsTableName,
    ServerSettingsTableValue,
    ServerSettingsTableTypeValue,
    ServerSettingsTableDescription
};

using ColumnValue = QPair<int, QVariant>;



#endif // COMMON_H
