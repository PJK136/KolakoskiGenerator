#ifndef MULTILETTERSGENERATOR_H
#define MULTILETTERSGENERATOR_H

#include <queue>
#include "generator.h"

class MultiLettersGenerator : public GeneratorBase<std::queue<unsigned char>>
{
public:
    void init(std::vector<unsigned char> lettres);

protected:
    bool nextLettre();
    inline void invert() { m_prev++;
                           if (m_prev >= m_lettres.size())
                               m_prev = 0;
                         }
    inline unsigned char toOutput(unsigned char lecture) { return m_lettres[lecture]; }

    std::vector<unsigned char> m_lettres;
};

#endif // MULTILETTERSGENERATOR_H
