#ifndef TWOLETTERSGENERATOR_H
#define TWOLETTERSGENERATOR_H

#include "binarygenerator.h"

class TwoLettersGenerator : public BinaryGenerator
{
    Q_OBJECT
public:
    virtual void init(std::vector<char> letters);

protected:
    virtual bool nextLettre();
    virtual inline char toOutput(char lettre) { return m_lettres[lettre ? 1 : 0]; }
    char m_lettres[2];
};

#endif // TWOLETTERSGENERATOR_H
