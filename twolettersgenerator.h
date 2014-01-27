#ifndef TWOLETTERSGENERATOR_H
#define TWOLETTERSGENERATOR_H

#include "binarygenerator.h"

class TwoLettersGenerator : public BinaryGenerator
{
    Q_OBJECT
public:
    virtual void init(std::vector<unsigned char> letters);

protected:
    virtual bool nextLettre();
    virtual inline unsigned char toOutput(unsigned char lettre) { return m_lettres[lettre ? 1 : 0]; }
    unsigned char m_lettres[2];
};

#endif // TWOLETTERSGENERATOR_H
