#pragma once
#include <vector>
#include <assert.h>

namespace dblogger
{

// 缓冲区默认大小
const size_t defaultBufferSize = 1 * 1024 * 1024;
// 阈值
const size_t threshold = 4 * 1024 * 1024;
// 大于阈值以后每次扩容的自增值
const size_t increament = 1 * 1024 * 1024;

class Buffer
{
public:
    Buffer(size_t size = defaultBufferSize)
        : m_buffer(size)
    {}

    void push(const char* data, size_t len)
    {
        if(len > writeAbleSize())
            resize(len);
        assert(len <= writeAbleSize());
        std::copy(data, data + len, &m_buffer[m_writeIdx]);
        moveWriteIx(len);
    }

    const char* start()
    {
        return &m_buffer[m_readIdx];
    }

    size_t writeAbleSize()
    {
        return m_buffer.size() - m_writeIdx;
    }

    size_t readAbleSize()
    {
        return m_writeIdx - m_readIdx;
    }

    void reset()
    {
        m_readIdx = 0;
        m_writeIdx = 0;
    }

    void moveReadIx(size_t len)
    {
        if(len < readAbleSize())
            m_readIdx += len;
        else
            m_readIdx = readAbleSize();
    }

    void moveWriteIx(size_t len)
    {
        assert(m_writeIdx + len <= m_buffer.size());
        m_writeIdx += len;
    }

    void resize(size_t len)
    {
        size_t newSize;
        if(m_buffer.size() < threshold)
            newSize = m_buffer.size() * 2 + len;
        else
            newSize = m_buffer.size() + increament + len;

        m_buffer.resize(newSize);
    }

    void swap(Buffer &buf)
    {
        m_buffer.swap(buf.m_buffer);
        std::swap(m_readIdx, buf.m_readIdx);
        std::swap(m_writeIdx, buf.m_writeIdx);
    }

    size_t size()
    {
        return m_buffer.size();
    }

    bool isEmpty() const
    {
        return m_readIdx == m_writeIdx;
    }

private:
    std::vector<char> m_buffer;
    size_t m_readIdx = 0;
    size_t m_writeIdx = 0;
};

}
    