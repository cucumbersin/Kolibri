#ifndef LONGLONGVALIDATOR_H
#define LONGLONGVALIDATOR_H

#include <QValidator>



class LongLongValidator : public QValidator
{
public:
    State validate(QString &input, int &pos) const override;
};

#endif // LONGLONGVALIDATOR_H
