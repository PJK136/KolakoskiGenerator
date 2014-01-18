#ifndef GENERATOR_H
#define GENERATOR_H

#include <deque>
#include <bitset>
#include <QDebug>

class Generator : public QObject
{
    Q_OBJECT
public:
    Generator(QObject *parent = 0): QObject(parent), m_pos_lecture(0), m_pos_ecriture(0), m_prev(false), m_limite_lecture(100), m_ecriture(true), m_initialized(false)
    {
        m_count[0] = 0;
        m_count[1] = 0;
    }

    inline void init()
    {
        m_lecture.clear();
        m_count[0] = 0;
        m_count[1] = 0;
        m_pos_lecture = 0;
        m_pos_ecriture = 0;
        m_ecriture = true;

        m_lecture.push_back(std::bitset<64>(0));
        m_prev = false; //Remplacer par 1ère lettre
        qDebug(m_prev ? "2" : "1");
        if (m_prev)
        {
            for (int i = 1; i < m_prev+1; i++)
                ajouterLettre(m_prev);
            return;
        }
        else
        {
            invert();
            qDebug(m_prev ? "2" : "1");
            for (int i = 1; i < m_prev+1; i++)
                ajouterLettre(m_prev);
        }

        m_initialized = true;
    }

    inline void setLimitLecture(unsigned long long limit)
    {
        m_limite_lecture = limit;
    }

    inline void generer()
    {
        while (nextLettre());
        m_lecture.clear();
        emit finished();
    }

    inline void stopGeneration()
    {
        m_initialized = false;
    }

    inline unsigned long long getTailleLecture()
    {
        return m_lecture.size() ? (m_lecture.size() - 1ull)*64ull + m_pos_ecriture : 0;
    }

    inline unsigned long long getCount(bool lettre)
    {
        return m_count[lettre];
    }

signals:
    void finished();

protected:
    char m_pos_lecture;
    char m_pos_ecriture;
    bool m_prev;
    unsigned long long m_limite_lecture;
    bool m_ecriture;
    bool m_initialized;
    std::deque<std::bitset<64>> m_lecture;
    unsigned long long m_count[2];
    inline void invert() { m_prev = !m_prev; }

    inline bool nextLettre()
    {
        invert();
        if(!m_lecture.front()[m_pos_lecture])
        {
            ajouterLettre(m_prev);
        }
        else
        {
            ajouterLettre(m_prev);
            ajouterLettre(m_prev);
        }

        if (++m_pos_lecture >= 64)
        {
            m_lecture.pop_front();
            m_pos_lecture = 0;
            if (m_lecture.empty())
                m_initialized = false;
        }

        return m_initialized;
    }

    inline void ajouterLettre(bool lettre)
    {
        m_count[lettre]++;
        //qDebug(lettre ? "2" : "1");
        if (!m_ecriture)
            return;

        m_lecture.back()[m_pos_ecriture] = lettre;
        if (++m_pos_ecriture >= 64)
        {
            if (static_cast<unsigned long long>(m_lecture.size())*64ull < m_limite_lecture)
                m_lecture.push_back(std::bitset<64>(0));
            else
                m_ecriture = false;
            m_pos_ecriture = 0;
        }
    }
};

#endif // GENERATOR_H
