#ifndef BINARYCONTAINER_H
#define BINARYCONTAINER_H

#include <deque>
#include <bitset>
#include "container.h"

#ifdef INHERIT_CONTAINER
class BinaryContainer : public Container
#else
class BinaryContainer
#endif
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

    inline unsigned char front()
    {
        return m_array.front()[m_pos_lecture];
    }

    inline void pop()
    {
        if (++m_pos_lecture >= 64)
        {
            m_array.pop_front();
            m_pos_lecture = 0;
        }
    }

    inline void clear()
    {
        m_array.clear();
        m_array.push_back(std::bitset<64>(0));
        m_pos_lecture = 0;
        m_pos_ecriture = 0;
    }

    inline std::size_t size()
    {
        return !empty() ? (m_array.size() - 1)*64 + m_pos_ecriture - m_pos_lecture : 0;
    }

    inline std::size_t approximateSize()
    {
        return m_array.size()*64;
    }

    inline bool empty()
    {
        return m_array.size() <= 1 ? (m_array.empty() || m_pos_lecture >= m_pos_ecriture) : false;
    }

    inline bool hasNext()
    {
        return m_array.size() > 1 || (!m_array.empty() && m_pos_lecture < m_pos_ecriture);
    }

protected:
    std::deque<std::bitset<64>> m_array;
    unsigned char m_pos_lecture;
    unsigned char m_pos_ecriture;
};

#endif // BINARYCONTAINER_H
