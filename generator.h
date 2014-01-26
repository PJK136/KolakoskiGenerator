#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <vector>

class Container
{
public:
    virtual ~Container() {}
    virtual void push(char) = 0;
    virtual char first() = 0;
    virtual char pop() = 0;
    virtual void clear() = 0;
    virtual unsigned long long size() = 0;
    virtual unsigned long long approximateSize() = 0;
    virtual bool isEmpty() = 0;
};

class Generator : public QObject
{
    Q_OBJECT
public:
    explicit Generator(QObject *parent = 0): QObject(parent), m_lecture(0), m_limite_lecture(100), m_ecriture(true), m_status(INTERRUPTED), m_prev(false), m_position(0) { }

    virtual void init(std::vector<char> letters) = 0;

    virtual unsigned long long getTailleLecture() { return m_lecture->size(); }

    inline std::vector<unsigned long long> getCounts()
    {
        return m_count;
    }

signals:
    void progression(std::vector<unsigned long long>);
    void finished();
    void finishedOutput(char*);

public slots:
    virtual inline void stopGeneration()
    {
        m_status = INTERRUPTED;
    }

    virtual inline std::vector<unsigned long long> getProgression()
    {
        if (m_status == GENERATING)
        {
            m_status = UPDATING_DATA;
            return std::vector<unsigned long long>();
        }
        else
        {
            std::vector<unsigned long long> data(m_count);
            data.push_back(getTailleLecture());
            return data;
        }
    }

    virtual inline void setLimitLecture(unsigned long long limit)
    {
        m_limite_lecture = limit;
    }

    virtual inline void startGeneration() {
        if (!m_lecture->isEmpty())
        {
            m_output[m_position] = toOutput(m_lecture->first());
            m_position++;
        }

        for (; m_position < 9999 && m_status && nextLettre(); m_position++)
            m_output[m_position] = toOutput(m_lecture->first());
        m_output[m_position] = 0;
        m_status = UPDATING_DATA;
        emit finishedOutput(m_output);
        emitProgression();

        while (nextLettre())
        {
            if (m_status == GENERATING)
                continue;
            else if (m_status == UPDATING_DATA)
                emitProgression();
            else
                break;
        }
        m_lecture->clear();
        m_status = INTERRUPTED;
        emit finished();
    }

protected:
    enum {INTERRUPTED = 0,
          GENERATING = 1,
          UPDATING_DATA = 2};

    virtual bool nextLettre() = 0;
    virtual inline void ajouterLettre(char lettre) {
        m_count[lettre]++;
        if (!m_ecriture)
            return;

        m_lecture->push(lettre);

        if (m_lecture->approximateSize() > m_limite_lecture)
            m_ecriture = false;
    }
    virtual char toOutput(char lecture) = 0;

    virtual inline void emitProgression()
    {
        emit progression(getProgression());
        m_status = GENERATING;
    }

    Container *m_lecture;
    unsigned long long m_limite_lecture;
    bool m_ecriture;
    int m_status;
    std::vector<unsigned long long> m_count;
    char m_prev;
    unsigned int m_position;
    char m_output[10000];
};

#endif // GENERATOR_H
