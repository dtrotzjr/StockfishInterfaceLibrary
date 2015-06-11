/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Copyright David Trotz david.trotz@gmail.com
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
 *
 */

#include "stockfishguitoenginestream.h"
#include <sstream>

#define ISS_BUFFER_SIZE 256
#define ISS_PUT_BACK_SIZE 16

StockfishGUIToEngineStream::StockfishGUIToEngineStream() : _buffer(ISS_BUFFER_SIZE)
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
    char *end = &_buffer.front() + _buffer.size();
    setg(end, end, end);
}

std::streambuf::int_type StockfishGUIToEngineStream::underflow()
{
    int_type c = traits_type::eof();
    pthread_mutex_lock(&_mutex);
    c = _next_char();
    while (c == traits_type::eof()){
        if (_staged_bytes.length() == 0)
            pthread_cond_wait(&_cond, &_mutex);
        _fill();
        c = _next_char();
    }
    pthread_mutex_unlock(&_mutex);

    return c;
}

std::streambuf::int_type StockfishGUIToEngineStream::_next_char() {
    int_type c = traits_type::eof();
    if (gptr() < egptr()) // buffer not exhausted
        c = traits_type::to_int_type(*gptr());
    return c;
}

void StockfishGUIToEngineStream::_fill() {
    char *base = &_buffer.front();
    char *start = base;
    
    if (eback() == base) // true when this isn't the first fill
    {
        // Make arrangements for putback characters
        std::memmove(base, egptr() - ISS_PUT_BACK_SIZE, ISS_PUT_BACK_SIZE);
        start += ISS_PUT_BACK_SIZE;
    }
    
    size_t n = _staged_bytes.length() < ISS_PUT_BACK_SIZE ? _staged_bytes.length() : ISS_PUT_BACK_SIZE;
    std::string tmp = _staged_bytes.substr(0, n);
    _staged_bytes = _staged_bytes.substr(n, _staged_bytes.length() - n);
    for (long i = 0; i < n; i++) {
        _buffer[i + (start - base)] = tmp[i];
    }
    // Set buffer pointers
    setg(base, start, start + n);
}


void StockfishGUIToEngineStream::add(const char* str)
{
    pthread_mutex_lock(&_mutex);
    _staged_bytes += str;
    pthread_cond_signal(&_cond);
    pthread_mutex_unlock(&_mutex);
}