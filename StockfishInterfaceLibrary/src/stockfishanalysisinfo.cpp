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

#include "stockfishanalysisinfo.h"

StockfishAnalysisInfo::StockfishAnalysisInfo()
{
    _depth = SF_ANALYSIS_UNSET_VALUE;
    _selDepth = SF_ANALYSIS_UNSET_VALUE;
    _time = SF_ANALYSIS_UNSET_VALUE;
    _nodes = SF_ANALYSIS_UNSET_VALUE;
    _multipv = SF_ANALYSIS_UNSET_VALUE;
    _score.cp = SF_ANALYSIS_UNSET_VALUE;
    _score.mate = SF_ANALYSIS_UNSET_VALUE;
    _score.lowerBound = false;
    _score.upperBound = false;
    _nps = SF_ANALYSIS_UNSET_VALUE;
}

StockfishAnalysisInfo::~StockfishAnalysisInfo()
{
    
}

int StockfishAnalysisInfo::GetDepth()
{
    return _depth;
}

int StockfishAnalysisInfo::GetSelDepth()
{
    return _selDepth;
}

int StockfishAnalysisInfo::GetTime()
{
    return _time;
}

int StockfishAnalysisInfo::GetNodes()
{
    return _nodes;
}

const char* StockfishAnalysisInfo::GetMoves()
{
    return _moves.c_str();
}

int StockfishAnalysisInfo::GetMultiPV()
{
    return _multipv;
}

StockfishAnalysisScore StockfishAnalysisInfo::GetScore()
{
    return _score;
}

const char* StockfishAnalysisInfo::GetCurMove()
{
    return _curMove.c_str();
}

int StockfishAnalysisInfo::GetCurMoveNumber()
{
    return _curMoveNumber;
}

int StockfishAnalysisInfo::GetNodesPerSecond()
{
    return _nps;
}