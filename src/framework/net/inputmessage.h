/*
 * Copyright (c) 2010-2017 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef INPUTMESSAGE_H
#define INPUTMESSAGE_H

#include "declarations.h"
#include <framework/luaengine/luaobject.h>

// @bindclass
class InputMessage : public LuaObject
{
public:
    enum {
        BUFFER_MAXSIZE = 327680,
        MAX_HEADER_SIZE = 12
    };

    InputMessage();

    void setBuffer(const std::string& buffer);
    std::string getBuffer() { return std::string((char*)m_buffer + m_headerPos, m_messageSize); }
    std::string getBodyBuffer() { return std::string((char*)m_buffer + MAX_HEADER_SIZE, m_messageSize - getHeaderSize()); }

    void skipBytes(uint32 bytes) { m_readPos += bytes; }
    void setReadPos(uint32 readPos) { m_readPos = readPos; }
    uint8 getU8();
    uint16 getU16();
    uint32 getU32();
    uint64 getU64();
    std::string getString();
    double getDouble();

    uint8 peekU8() { uint8 v = getU8(); m_readPos-=1; return v; }
    uint16 peekU16() { uint16 v = getU16(); m_readPos-=2; return v; }
    uint32 peekU32() { uint32 v = getU32(); m_readPos-=4; return v; }
    uint64 peekU64() { uint64 v = getU64(); m_readPos-=8; return v; }

    bool decryptRsa(int size);

    uint32 getHeaderPos() { return m_headerPos; }
    uint32 getHeaderSize() { return (MAX_HEADER_SIZE - m_headerPos); }
    int getReadSize() { return m_readPos - m_headerPos; }
    int getReadPos() { return m_readPos; }
    int getUnreadSize() { return m_messageSize - (m_readPos - m_headerPos); }
    uint32 getMessageSize() { return m_messageSize; }

    bool eof() { return (m_readPos - m_headerPos) >= m_messageSize; }

protected:
    void reset();
    void fillBuffer(uint8 *buffer, uint32 size);

    void setHeaderSize(uint32 size);
    void setMessageSize(uint32 size) { m_messageSize = size; }

    uint8* getReadBuffer() { return m_buffer + m_readPos; }
    uint8* getHeaderBuffer() { return m_buffer + m_headerPos; }
    uint8* getDataBuffer() { return m_buffer + MAX_HEADER_SIZE; }

    uint32 readSize(bool bigSize) { return bigSize ? getU32() : getU16(); }
    bool readChecksum();

    void addZlibFooter();

    friend class Protocol;

private:
    bool canRead(int bytes);
    void checkRead(int bytes);
    void checkWrite(int bytes);

    uint32 m_headerPos;
    uint32 m_readPos;
    uint32 m_messageSize;
    uint8 m_buffer[BUFFER_MAXSIZE];
};

#endif
