#ifndef BINARYGENERATOR_H
#define BINARYGENERATOR_H

#include "generator.h"
#include "binarycontainer.h"

class BinaryGenerator : public GeneratorBase<BinaryContainer>
{
    Q_OBJECT
public:
    explicit BinaryGenerator();
    virtual void init(std::vector<unsigned char> letters);

protected:
    virtual bool nextLettre();
    inline void invert() { m_prev = !m_prev; }
    virtual inline unsigned char toOutput(unsigned char lecture) { return ++lecture; }
};

#endif // BINARYGENERATOR_H
