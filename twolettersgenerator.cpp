#include "twolettersgenerator.h"
#include "binarycontainer.h"

void TwoLettersGenerator::init(std::vector<char> letters)
{
    m_lecture->clear();
    m_count[0] = 0;
    m_count[1] = 0;
    m_ecriture = true;
    if (letters.size() < 2 || !letters[0] || !letters[1] || letters[0] == letters[1])
    {
        m_status = INTERRUPTED;
        return;
    }

    m_lettres[0] = letters[0];
    m_lettres[1] = letters[1];

    m_count[0]++;
    m_position = 0;
    m_output[0] = letters[0];
    m_position++;

    if (m_lettres[0] > 1)
    {
        for (int i = 1; i < m_lettres[0]; i++)
            ajouterLettre(false);
        m_prev = false;
    }
    else
    {
        m_count[1]++;
        m_output[m_position] = letters[1];
        m_position++;
        for (int i = 1; i < m_lettres[1]; i++)
            ajouterLettre(true);
        m_prev = true;
    }

    m_status = GENERATING;
}

bool TwoLettersGenerator::nextLettre()
{
    invert();
    if(!m_lecture->pop())
    {
        for (char i = 0; i < m_lettres[0]; i++)
            ajouterLettre(m_prev);
    }
    else
    {
        for (char i = 0; i < m_lettres[1]; i++)
            ajouterLettre(m_prev);
    }

    return !m_lecture->isEmpty();
}
