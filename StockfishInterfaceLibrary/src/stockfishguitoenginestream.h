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

#ifndef __StockfishAnalysis__infinitestringstream__
#define __StockfishAnalysis__infinitestringstream__

#include <sstream>
#include <vector>
#include <pthread.h>


class StockfishGUIToEngineStream: public std::streambuf {
public:
    StockfishGUIToEngineStream();
    
    void add(const char* string);
    
    virtual std::streambuf::int_type underflow();
private:
    std::streambuf::int_type _next_char();
    void _fill();
    std::vector<char> _buffer;
    std::string _staged_bytes;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};

#endif /* defined(__StockfishAnalysis__infinitestringstream__) */
