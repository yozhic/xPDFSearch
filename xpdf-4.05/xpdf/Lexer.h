//========================================================================
//
// Lexer.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef LEXER_H
#define LEXER_H

#include <aconf.h>

#include "Object.h"
#include "Stream.h"

class XRef;

#define tokBufSize 128		// size of token buffer

//------------------------------------------------------------------------
// Lexer
//------------------------------------------------------------------------

class Lexer {
public:

  // Construct a lexer for a single stream.  Deletes the stream when
  // lexer is deleted.
  Lexer(XRef *xref, Stream *str);

  // Construct a lexer for a stream or array of streams (assumes obj
  // is either a stream or array of streams).
  Lexer(XRef *xref, Object *obj);
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;
  // Destructor.
  ~Lexer();

  // Get the next object from the input stream.
  Object *getObj(Object *obj);

  // Skip to the beginning of the next line in the input stream.
  void skipToNextLine();

  // Skip to the end of the input stream.
  void skipToEOF();

  // Skip over one character.
  void skipChar() { getChar(); }

  // Get stream index (for arrays of streams).
  int getStreamIndex() { return strPtr; }

  // Get stream.
  Stream *getStream()
    { return curStr.isNone() ? (Stream *)NULL : curStr.getStream(); }

  // Get current position in file.
  GFileOffset getPos()
    { return curStr.isNone() ? -1 : curStr.streamGetPos(); }

  // Set position in file.
  void setPos(GFileOffset pos, int dir = 0)
    { if (!curStr.isNone()) curStr.streamSetPos(pos, dir); }

  // Returns true if <c> is a whitespace character.
  static GBool isSpace(int c);

private:

  int getChar();
  int lookChar();

  Array* streams{ nullptr };		// array of input streams
  int strPtr{ 0 };			// index of current stream
  Object curStr;		// current stream
  GBool freeArray{ gTrue };		// should lexer free the streams array?
  char tokBuf[tokBufSize]{ };	// temporary token buffer
};

#endif
