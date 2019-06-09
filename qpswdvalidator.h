#ifndef QPSWDVALIDATOR_H
#define QPSWDVALIDATOR_H
#include <QRegExpValidator>
#include <qregexp.h>

class QPswdValidator : public QRegExpValidator {
public:
    QPswdValidator();
    virtual ~QPswdValidator() Q_DECL_OVERRIDE;
};

#endif // QPSWDVALIDATOR_H
