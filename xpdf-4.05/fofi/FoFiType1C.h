//========================================================================
//
// FoFiType1C.h
//
// Copyright 1999-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef FOFITYPE1C_H
#define FOFITYPE1C_H

#include <aconf.h>

#include "gtypes.h"
#include "FoFiBase.h"

class GHash;
class GString;

//------------------------------------------------------------------------

struct Type1CIndex {
  int pos;			// absolute position in file
  int len;			// length (number of entries)
  int offSize;			// offset size
  int startPos;			// position of start of index data - 1
  int endPos;			// position one byte past end of the index
};

struct Type1CIndexVal {
  int pos;			// absolute position in file
  int len;			// length, in bytes
};

struct Type1CTopDict {
  int firstOp;

  int versionSID;
  int noticeSID;
  int copyrightSID;
  int fullNameSID;
  int familyNameSID;
  int weightSID;
  int isFixedPitch;
  double italicAngle;
  double underlinePosition;
  double underlineThickness;
  int paintType;
  int charstringType;
  double fontMatrix[6];
  GBool hasFontMatrix;		// CID fonts are allowed to put their
				//   FontMatrix in the FD instead of the
				//   top dict
  int uniqueID;
  double fontBBox[4];
  double strokeWidth;
  int charsetOffset;
  int encodingOffset;
  int charStringsOffset;
  int privateSize;
  int privateOffset;

  // CIDFont entries
  int registrySID;
  int orderingSID;
  int supplement;
  int fdArrayOffset;
  int fdSelectOffset;
};

#define type1CMaxBlueValues 14
#define type1CMaxOtherBlues 10
#define type1CMaxStemSnap   12

struct Type1CPrivateDict {
  double fontMatrix[6];
  GBool hasFontMatrix;
  int blueValues[type1CMaxBlueValues];
  int nBlueValues;
  int otherBlues[type1CMaxOtherBlues];
  int nOtherBlues;
  int familyBlues[type1CMaxBlueValues];
  int nFamilyBlues;
  int familyOtherBlues[type1CMaxOtherBlues];
  int nFamilyOtherBlues;
  double blueScale;
  int blueShift;
  int blueFuzz;
  double stdHW;
  GBool hasStdHW;
  double stdVW;
  GBool hasStdVW;
  double stemSnapH[type1CMaxStemSnap];
  int nStemSnapH;
  double stemSnapV[type1CMaxStemSnap];
  int nStemSnapV;
  GBool forceBold;
  GBool hasForceBold;
  double forceBoldThreshold;
  int languageGroup;
  double expansionFactor;
  int initialRandomSeed;
  int subrsOffset;
  double defaultWidthX;
  GBool defaultWidthXInt;
  double nominalWidthX;
  GBool nominalWidthXInt;
};

// operand kind
enum Type1COpKind {
  type1COpOperator,
  type1COpInteger,
  type1COpFloat,
  type1COpRational
};

// operand
struct Type1COp {
  Type1COpKind kind;
  union {
    int op;			// type1COpOperator
    int intgr;			// type1COpInteger
    double flt;			// type1COpFloat
    struct {
      int num, den;		// type1COpRational
    } rat;
  };
  GBool isZero() const;
  GBool isNegative() const;
  int toInt() const;
  double toFloat() const;
};

struct Type1CEexecBuf {
  FoFiOutputFunc outputFunc;
  void *outputStream;
  GBool ascii;			// ASCII encoding?
  Gushort r1;			// eexec encryption key
  int line;			// number of eexec chars left on current line
};

//------------------------------------------------------------------------
// FoFiType1C
//------------------------------------------------------------------------

class FoFiType1C: public FoFiBase {
public:

  // Create a FoFiType1C object from a memory buffer.
  static FoFiType1C *make(char *fileA, int lenA);

  // Create a FoFiType1C object from a file on disk.
  static FoFiType1C *load(char *fileName);

  virtual ~FoFiType1C();

  // Return the font name.
  char *getName();

  // Return the encoding, as an array of 256 names (any of which may
  // be NULL).  This is only useful with 8-bit fonts.
  char **getEncoding();

  // Get the glyph names.
  int getNumGlyphs() const { return nGlyphs; }
  GString *getGlyphName(int gid);

  // Returns a hash mapping glyph names to GIDs.  This is only useful
  // with 8-bit fonts.
  GHash *getNameToGIDMap();

  // Return the mapping from CIDs to GIDs, and return the number of
  // CIDs in *<nCIDs>.  This is only useful for CID fonts.
  int *getCIDToGIDMap(int *nCIDs);

  // Return the font matrix as an array of six numbers.
  void getFontMatrix(double *mat);

  // Convert to a Type 1 font, suitable for embedding in a PostScript
  // file.  This is only useful with 8-bit fonts.  If <newEncoding> is
  // not NULL, it will be used in place of the encoding in the Type 1C
  // font.  If <ascii> is true the eexec section will be hex-encoded,
  // otherwise it will be left as binary data.  If <psName> is non-NULL,
  // it will be used as the PostScript font name.
  void convertToType1(char *psName, const char **newEncoding, GBool ascii,
		      FoFiOutputFunc outputFunc, void *outputStream);

  // Convert to a Type 0 CIDFont, suitable for embedding in a
  // PostScript file.  <psName> will be used as the PostScript font
  // name.  There are three cases for the CID-to-GID mapping:
  // (1) if <codeMap> is non-NULL, then it is the CID-to-GID mapping
  // (2) if <codeMap> is NULL and this is a CID CFF font, then the
  //     font's internal CID-to-GID mapping is used
  // (3) is <codeMap> is NULL and this is an 8-bit CFF font, then
  //     the identity CID-to-GID mapping is used
  void convertToCIDType0(char *psName, int *codeMap, int nCodes,
			 FoFiOutputFunc outputFunc, void *outputStream);

  // Convert to a Type 0 (but non-CID) composite font, suitable for
  // embedding in a PostScript file.  <psName> will be used as the
  // PostScript font name.  There are three cases for the CID-to-GID
  // mapping:
  // (1) if <codeMap> is non-NULL, then it is the CID-to-GID mapping
  // (2) if <codeMap> is NULL and this is a CID CFF font, then the
  //     font's internal CID-to-GID mapping is used
  // (3) is <codeMap> is NULL and this is an 8-bit CFF font, then
  //     the identity CID-to-GID mapping is used
  void convertToType0(char *psName, int *codeMap, int nCodes,
		      FoFiOutputFunc outputFunc, void *outputStream);

  // Write an OpenType file, encapsulating the CFF font.  <widths>
  // provides the glyph widths (in design units) for <nWidths> glyphs.
  // The cmap table must be supplied by the caller.
  void convertToOpenType(FoFiOutputFunc outputFunc, void *outputStream,
			 int nWidths, Gushort *widths,
			 Guchar *cmapTable, int cmapTableLen);

private:

  FoFiType1C(char *fileA, int lenA, GBool freeFileDataA);
  void eexecCvtGlyph(Type1CEexecBuf *eb, const char *glyphName,
		     int offset, int nBytes,
		     Type1CIndex *subrIdx,
		     Type1CPrivateDict *pDict);
  void cvtGlyph(int offset, int nBytes, GString *charBuf,
		Type1CIndex *subrIdx, Type1CPrivateDict *pDict,
		GBool top, int recursion);
  void cvtGlyphWidth(GBool useOp, GString *charBuf,
		     Type1CPrivateDict *pDict);
  void cvtNum(Type1COp op, GString *charBuf);
  void eexecWrite(Type1CEexecBuf *eb, const char *s);
  void eexecWriteCharstring(Type1CEexecBuf *eb, Guchar *s, int n);
  void writePSString(char *s, FoFiOutputFunc outputFunc, void *outputStream);
  Guint computeOpenTypeTableChecksum(Guchar *data, int length);
  GBool parse();
  void readTopDict();
  void readFD(int offset, int length, Type1CPrivateDict *pDict);
  void readPrivateDict(int offset, int length, Type1CPrivateDict *pDict);
  void readFDSelect();
  void buildEncoding();
  GBool readCharset();
  int getOp(int pos, GBool charstring, GBool *ok);
  int getDeltaIntArray(int *arr, int maxLen);
  int getDeltaFPArray(double *arr, int maxLen);
  void getIndex(int pos, Type1CIndex *idx, GBool *ok);
  void getIndexVal(Type1CIndex *idx, int i, Type1CIndexVal *val, GBool *ok);
  char *getString(int sid, char *buf, GBool *ok);

  GString *name;
  char **encoding;

  Type1CIndex nameIdx;
  Type1CIndex topDictIdx;
  Type1CIndex stringIdx;
  Type1CIndex gsubrIdx;
  Type1CIndex charStringsIdx;

  Type1CTopDict topDict;
  Type1CPrivateDict *privateDicts;

  int nGlyphs;
  int nFDs;
  Guchar *fdSelect;
  Gushort *charset;
  int gsubrBias;

  GBool parsedOk;

  Type1COp ops[49];		// operands and operator
  int nOps;			// number of operands
  int nHints;			// number of hints for the current glyph
  GBool firstOp;		// true if we haven't hit the first op yet
  GBool openPath;		// true if there is an unclosed path
};

#endif
