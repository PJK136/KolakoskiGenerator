#ifndef BINARYCONTAINER_H
#define BINARYCONTAINER_H

#include <deque>
#include <bitset>
#include "container.h"

class BinaryContainer : public Container
{
public:
    BinaryContainer() : m_pos_lecture(0), m_pos_ecriture(0)
    {
        m_array.push_back(std::bitset<64>(0));
    }

    inline void push(unsigned char letter)
    {
        m_array.back()[m_pos_ecriture] = letter;
        if (++m_pos_ecriture >= 64)
        {
            m_array.push_back(std::bitset<64>(0));
            m_pos_ecriture = 0;
        }
    }

    inline unsigned char first()
    {
        return m_array.front()[m_pos_lecture];
    }

    inline unsigned char pop()
    {
        unsigned char result = m_array.front()[m_pos_lecture];
        if (++m_pos_lecture >= 64)
        {
            m_array.pop_front();
            m_pos_lecture = 0;
        }
        return result;
    }

    inline void clear()
    {
        m_array.clear();
        m_array.push_back(std::bitset<64>(0));
        m_pos_lecture = 0;
        m_pos_ecriture = 0;
    }

    inline unsigned long long size()
    {
        return !isEmpty() ? (m_array.size() - 1ull)*64ull + m_pos_ecriture - m_pos_lecture : 0;
    }

    inline unsigned long long approximateSize()
    {
        return m_array.size()*64ull;
    }

    inline bool isEmpty()
    {
        return m_array.size() <= 1 ? (m_array.empty() || m_pos_lecture >= m_pos_ecriture) : false;
    }

protected:
    std::deque<std::bitset<64>> m_array;
    unsigned char m_pos_lecture;
    unsigned char m_pos_ecriture;
};

#endif // BINARYCONTAINER_H
