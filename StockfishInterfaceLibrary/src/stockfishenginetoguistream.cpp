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

#include "stockfishenginetoguistream.h"
#include "stockfishenginetoguiinterface.h"
#include "stockfishanalysisinfo.h"
#include "aptrie.h"

#include <sstream>
#include <iostream>
#include <assert.h>
#include <cstring>

const char* StockfishEngineToGUIStream::_responseStarterTokens[TOTAL_RESPONSE_STARTER_TOKENS] = {
    "id",
    "uciok",
    "readyok",
    "bestmove",
    "copyprotection",
    "registration",
    "info",
    "option"
};

const char* StockfishEngineToGUIStream::_infoTokens[TOTAL_INFO_TOKENS] = {
    "depth",
    "seldepth",
    "time",
    "nodes",
    "pv",
    "multipv",
    "score",
    "currmove",
    "currmovenumber",
    "nps"
};

const char* StockfishEngineToGUIStream::_infoScoreTokens[TOTAL_INFO_SCORE_TOKENS] = {
    "cp",
    "mate",
    "lowerbound",
    "upperbound"
};

StockfishEngineToGUIStream::StockfishEngineToGUIStream(std::ostream &sink) :
    _sink(sink)
{
    _coutBuff = _sink.rdbuf();
    _sink.rdbuf(this);
    _engineToGUIDelegate = NULL;
    
    _keywordTrie = new APTrie();
    for (int i = 0; i < TOTAL_RESPONSE_STARTER_TOKENS; i++) {
        _keywordTrie->AddToken(_responseStarterTokens[i], StockfishResponseTokenId + i);
    }
    for (int i = 0; i < TOTAL_INFO_TOKENS; i++) {
        _keywordTrie->AddToken(_infoTokens[i], StockfishInfoToken_DEPTH + i);
    }
    for (int i = 0; i < TOTAL_INFO_SCORE_TOKENS; i++) {
        _keywordTrie->AddToken(_infoScoreTokens[i], StockfishInfoScore_CP + i);
    }
    _firstWord = true;
    _capturingResponse = -1;
}

StockfishEngineToGUIStream::~StockfishEngineToGUIStream()
{
    _sink.rdbuf(_coutBuff);
    delete _keywordTrie;
}

void StockfishEngineToGUIStream::SetEngineToGUIDelegate(StockfishEngineToGUIInterface* engineToGUIDelegate)
{
    _engineToGUIDelegate = engineToGUIDelegate;
}

void StockfishEngineToGUIStream::parseCurrentLineAndSendGUIResponse()
{
    char *saveptr = NULL;
    char* line = (char*)calloc(_curLine.length() + 1, sizeof(char));
    strcpy(line, _curLine.c_str());
    char* tok = strtok_r(line, " ", &saveptr);
    char* val = NULL;
    if (_keywordTrie->IdForToken(tok) == StockfishResponseTokenINFO) {
        StockfishAnalysisInfo* info = new StockfishAnalysisInfo();
        while (true) {
            tok = strtok_r(NULL, " ", &saveptr);
            if (tok != NULL) {
                StockfishInfoTokenType infoType = (StockfishInfoTokenType)_keywordTrie->IdForToken(tok);
                switch (infoType) {
                    case StockfishInfoToken_DEPTH:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_depth = (int)strtol(val, NULL, 10);
                        }
                        break;
                    case StockfishInfoToken_SELDEPTH:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_selDepth = (int)strtol(val, NULL, 10);
                        }
                        break;
                    case StockfishInfoToken_TIME:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_time = (int)strtol(val, NULL, 10);
                        }
                        break;
                    case StockfishInfoToken_NODES:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_nodes = (int)strtol(val, NULL, 10);
                        }
                        break;
                    case StockfishInfoToken_PV:
                        val = strtok_r(NULL, "", &saveptr);
                        if (val != NULL) {
                            info->_moves = val;
                        }
                        break;
                    case StockfishInfoToken_MULTIPV:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_multipv = (int)strtol(val, NULL, 10);
                        }
                        break;
                    case StockfishInfoToken_SCORE:
                        tok = strtok_r(NULL, " ", &saveptr);
                        if (tok != NULL) {
                            StockfishInfoScoreTokenType scoreType = (StockfishInfoScoreTokenType)_keywordTrie->IdForToken(tok);
                            switch (scoreType) {
                                case StockfishInfoScore_CP:
                                    val = strtok_r(NULL, " ", &saveptr);
                                    if (val != NULL) {
                                        info->_score.cp = (int)strtol(val, NULL, 10);
                                    }
                                    break;
                                case StockfishInfoScore_MATE:
                                    val = strtok_r(NULL, " ", &saveptr);
                                    if (val != NULL) {
                                        info->_score.mate = (int)strtol(val, NULL, 10);
                                    }
                                    break;
                                case StockfishInfoScore_LOWERBOUND:
                                    val = strtok_r(NULL, " ", &saveptr);
                                    if (val != NULL) {
                                        info->_score.lowerBound = (int)strtol(val, NULL, 10);
                                    }
                                    break;
                                case StockfishInfoScore_UPPERBOUND:
                                    val = strtok_r(NULL, " ", &saveptr);
                                    if (val != NULL) {
                                        info->_score.upperBound = (int)strtol(val, NULL, 10);
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                        break;
                    case StockfishInfoToken_CURRMOVE:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_curMove = val;
                        }
                        break;
                    case StockfishInfoToken_CURRMOVENUMBER:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_curMoveNumber = (int)strtol(val, NULL, 10);
                        }
                        break;
                    case StockfishInfoToken_NPS:
                        val = strtok_r(NULL, " ", &saveptr);
                        if (val != NULL) {
                            info->_nps = (int)strtol(val, NULL, 10);
                        }
                        break;
                    default:
                        break;
                }
            } else {
                break;
            }
        }
        if (_engineToGUIDelegate != NULL) {
            _engineToGUIDelegate->OnEngineToGUIInfoEvent(info);
        }
        delete info;
    } else {
        if (_engineToGUIDelegate != NULL) {
            _engineToGUIDelegate->OnEngineToGUIEvent((StockfishResponseTokenType)_capturingResponse, _curLine.c_str());
        }
    }
    
    free(line);
    
}

StockfishEngineToGUIStream::int_type StockfishEngineToGUIStream::overflow(int_type ch)
{
    if (!traits_type::eq_int_type(ch, traits_type::eof())) {
        if (_capturingResponse >= StockfishResponseTokenId) {
            _curLine += ch;
            if (ch == '\n') {
                parseCurrentLineAndSendGUIResponse();
                _capturingResponse = -1;
                _firstWord = true;
                _curLine = "";
            }
        }
        else if (_firstWord) {
            if (ch == ' ') {
                _firstWord = false;
                _capturingResponse = _keywordTrie->IdForToken(_curLine.c_str());
                if (_capturingResponse >= StockfishResponseTokenId) {
                    _curLine += ch;
                } else {
                    _curLine = "";
                }
            } else {
                _curLine += ch;
            }
        }
        ch = _coutBuff->sputc(ch);
        return ch;
    }
    return traits_type::not_eof(ch);
}

int StockfishEngineToGUIStream::sync()
{
    return _coutBuff->pubsync();
}