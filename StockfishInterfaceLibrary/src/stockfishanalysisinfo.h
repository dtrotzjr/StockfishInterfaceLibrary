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

#ifndef __StockfishAnalysis__stockfishinfo__
#define __StockfishAnalysis__stockfishinfo__

#include <stdio.h>
#include "stockfishenginetoguistream.h"

typedef struct {
    int cp;
    int mate;
    bool lowerBound;
    bool upperBound;
} StockfishAnalysisScore;

#define SF_ANALYSIS_UNSET_VALUE 0x80000000

class StockfishAnalysisInfo {
    friend class StockfishEngineToGUIStream;
public:
    StockfishAnalysisInfo();
    ~StockfishAnalysisInfo();
    
    int GetDepth();
    int GetSelDepth();
    int GetTime();
    int GetNodes();
    const char* GetMoves();
    int GetMultiPV();
    StockfishAnalysisScore GetScore();
    const char* GetCurMove();
    int GetCurMoveNumber();
    int GetNodesPerSecond();
    
private:
    int _depth;
    int _selDepth;
    int _time;
    int _nodes;
    std::string _moves;
    std::string _curMove;
    int _curMoveNumber;
    int _multipv;
    StockfishAnalysisScore _score;
    int _nps;
};


#endif /* defined(__StockfishAnalysis__stockfishinfo__) */
