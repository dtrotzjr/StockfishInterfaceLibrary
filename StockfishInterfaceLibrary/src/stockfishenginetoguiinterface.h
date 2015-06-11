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

#ifndef __StockfishAnalysis_stockfishenginetoguiinterface_h
#define __StockfishAnalysis_stockfishenginetoguiinterface_h

#include "stockfishenginetoguistream.h"
class StockfishAnalysisInfo;

class StockfishEngineToGUIInterface {
public:
    virtual void OnEngineToGUIEvent(StockfishResponseTokenType eventTypeId, const char* responseLine) = 0;
    virtual void OnEngineToGUIInfoEvent(StockfishAnalysisInfo* info) = 0;
};

#endif /* defined(__StockfishAnalysis_stockfishenginetoguiinterface_h) */
