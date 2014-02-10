#include "twolettersgenerator.h"
#include "binarycontainer.h"

void TwoLettersGenerator::init(std::vector<unsigned char> lettres)
{
    m_lecture.clear();
    m_count[0] = 0;
    m_count[1] = 0;
    m_ecriture = true;
    if (lettres.size() < 2 || !lettres[0] || !lettres[1] || lettres[0] == lettres[1])
    {
        m_status = INTERRUPTED;
        return;
    }

    m_lettres[0] = lettres[0];
    m_lettres[1] = lettres[1];

    m_count[0]++;
    m_position = 0;
    m_output[0] = lettres[0];
    m_position++;

    if (m_lettres[0] > 1)
    {
        for (unsigned char i = 1; i < m_lettres[0]; i++)
            ajouterLettre(false);
        m_prev = false;
    }
    else
    {
        m_count[1]++;
        m_output[m_position] = lettres[1];
        m_position++;

        for (unsigned char i = 1; i < m_lettres[1]; i++)
            ajouterLettre(true);
        m_prev = true;
    }

    m_status = GENERATING;
}

bool TwoLettersGenerator::nextLettre()
{
    invert();
    if(!m_lecture.front())
    {
        for (unsigned char i = 0; i < m_lettres[0]; i++)
            ajouterLettre(m_prev);
    }
    else
    {
        for (unsigned char i = 0; i < m_lettres[1]; i++)
            ajouterLettre(m_prev);
    }

    m_lecture.pop();

    return m_lecture.hasNext();
}
