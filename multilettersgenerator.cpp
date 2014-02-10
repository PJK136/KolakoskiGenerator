#include "multilettersgenerator.h"

void MultiLettersGenerator::init(std::vector<unsigned char> lettres)
{
    m_lecture = std::queue<unsigned char>();
    m_count.resize(lettres.size());
    m_ecriture = true;

    if (lettres.size() < 2)
    {
        m_status = INTERRUPTED;
        return;
    }

    m_lettres = lettres;

    for (int i = 0; i < m_lettres.size(); i++)
    {
        m_count[i] = 0;
        if (m_lettres[i] == 0)
        {
            m_status = INTERRUPTED;
            return;
        }
    }

    m_count[0]++;
    m_position = 0;
    m_output[0] = lettres[0];
    m_position++;

    if (m_lettres[0] > 1)
    {
        for (unsigned char i = 1; i < m_lettres[0]; i++)
            ajouterLettre(0);
        m_prev = 0;
    }
    else
    {
        m_count[1]++;
        m_output[m_position] = lettres[1];
        m_position++;

        for (unsigned char i = 1; i < m_lettres[1]; i++)
            ajouterLettre(1);
        m_prev = 1;
    }

    m_status = GENERATING;
}

bool MultiLettersGenerator::nextLettre()
{
    invert();

    unsigned char lecture = m_lecture.front();
    for (unsigned char i = 0; i < m_lettres[lecture]; i++)
        ajouterLettre(m_prev);

    m_lecture.pop();
    return !m_lecture.empty();
}
