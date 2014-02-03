#include "binarygenerator.h"
#include "binarycontainer.h"

BinaryGenerator::BinaryGenerator() :
    GeneratorBase()
{
    m_count.push_back(0);
    m_count.push_back(0);
}

void BinaryGenerator::init(std::vector<unsigned char> letters)
{
    m_lecture.clear();
    m_count[0] = 0;
    m_count[1] = 0;
    m_ecriture = true;
    if (letters.size() < 2 || !letters[0] || !letters[1] || letters[0] == letters[1] || letters[0] > 2 || letters[1] > 2)
    {
        m_status = INTERRUPTED;
        return;
    }

    m_position = 0;
    if (letters[0] == 1)
    {
        m_count[0]++;
        m_output[0] = 1;
        m_position++;
    }

    m_count[1]++;
    m_output[m_position] = 2;
    m_position++;
    ajouterLettre(true);
    m_prev = true;
    m_status = GENERATING;
}

bool BinaryGenerator::nextLettre()
{
    invert();
    if(!m_lecture.pop())
    {
        ajouterLettre(m_prev);
    }
    else
    {
        ajouterLettre(m_prev);
        ajouterLettre(m_prev);
    }

    return !m_lecture.isEmpty();
}
