#include "longlongvalidator.h"


QValidator::State LongLongValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos); // Не используем параметр pos в данном случае

    // Проверяем, является ли ввод числом типа long long
    bool isLongLong;
    input.toLongLong(&isLongLong);

    if (isLongLong) {
        return Acceptable;
    } else {
        return Invalid;
    }
}
