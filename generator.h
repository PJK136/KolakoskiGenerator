#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <vector>

class Generator : public QObject
{
    Q_OBJECT
public:
    virtual void init(std::vector<unsigned char> lettres) = 0;
    virtual std::size_t getTailleLecture() = 0;
    virtual std::vector<unsigned long long> getCounts() = 0;

signals:
    void progression(std::vector<unsigned long long>);
    void finished();
    void finishedOutput(unsigned char*);

public slots:
    virtual void startGeneration() = 0;
    virtual void stopGeneration() = 0;
    virtual std::vector<unsigned long long> getProgression() = 0;
    virtual void askProgression() = 0;
    virtual void setLimitLecture(unsigned long long limit) = 0;

protected:
    enum {INTERRUPTED = 0,
          GENERATING = 1,
          UPDATING_DATA = 2};

    virtual bool nextLettre() = 0;
    virtual unsigned char toOutput(unsigned char lecture) = 0;
};

template <class Container>
class GeneratorBase : public Generator
{
public:
    explicit GeneratorBase(): m_lecture(), m_limite_lecture(100), m_ecriture(true), m_status(INTERRUPTED), m_prev(0), m_position(0) { }


    inline std::size_t getTailleLecture() { return m_lecture.size(); }

    inline std::vector<unsigned long long> getCounts()
    {
        return m_count;
    }

    inline void startGeneration() {
        if (!m_lecture.empty())
        {
            m_output[m_position] = toOutput(m_lecture.front());
            m_position++;
        }

        for (; m_position < 9999 && m_status && nextLettre(); m_position++)
            m_output[m_position] = toOutput(m_lecture.front());
        m_output[m_position] = 0;
        m_status = UPDATING_DATA;
        emit finishedOutput(m_output);
        emitProgression();

        while (nextLettre())
        {
            if (m_status == GENERATING)
                continue;
            else if (m_status == UPDATING_DATA)
            {
                emitProgression();
                if (m_lecture.size() > m_limite_lecture)
                    m_ecriture = false;
            }
            else
                break;
        }
        m_lecture = Container();
        m_status = INTERRUPTED;
        emit finished();
    }

    inline void stopGeneration()
    {
        m_status = INTERRUPTED;
    }

    inline std::vector<unsigned long long> getProgression()
    {
        std::vector<unsigned long long> data(m_count);
        if (m_ecriture)
            data.push_back(getTailleLecture());
        else
            data.push_back(0);
        return data;
    }

    inline void askProgression()
    {
        m_status = UPDATING_DATA;
    }

    inline void setLimitLecture(unsigned long long limit)
    {
        m_limite_lecture = limit;
    }

protected:
    inline void ajouterLettre(unsigned char lettre) {
        m_count[lettre]++;
        if (!m_ecriture)
            return;

        m_lecture.push(lettre);
    }

    inline void emitProgression()
    {
        std::vector<unsigned long long> progress = getProgression();
        m_status = GENERATING;
        emit progression(progress);
    }

    Container m_lecture;
    unsigned long long m_limite_lecture;
    bool m_ecriture;
    int m_status;
    std::vector<unsigned long long> m_count;
    unsigned char m_prev;
    unsigned int m_position;
    unsigned char m_output[10000];
};

#endif // GENERATOR_H
