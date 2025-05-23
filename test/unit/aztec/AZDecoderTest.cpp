/*
* Copyright 2017 Huy Cuong Nguyen
* Copyright 2014 ZXing authors
*/
// SPDX-License-Identifier: Apache-2.0

#include "aztec/AZDecoder.h"
#include "BitArray.h"
#include "BitMatrixIO.h"
#include "DecoderResult.h"
#include "aztec/AZDetectorResult.h"

#include "gtest/gtest.h"
#include <string_view>
#include <utility>

namespace ZXing::Aztec {

DecoderResult Decode(const BitArray& bits);

}

using namespace ZXing;

// Shorthand to call Decode()
static DecoderResult parse(BitMatrix&& bits, bool compact, int nbDatablocks, int nbLayers)
{
	return Aztec::Decode({{std::move(bits), {}}, compact, nbDatablocks, nbLayers, false /*readerInit*/, false /*isMirrored*/, 0 /*runeValue*/});
}

TEST(AZDecoderTest, AztecResult)
{
	auto bits = ParseBitMatrix(
		"X X X X X     X X X       X X X     X X X     \n"
		"X X X     X X X     X X X X     X X X     X X \n"
		"  X   X X       X   X   X X X X     X     X X \n"
		"  X   X X     X X     X     X   X       X   X \n"
		"  X X   X X         X               X X     X \n"
		"  X X   X X X X X X X X X X X X X X X     X   \n"
		"  X X X X X                       X   X X X   \n"
		"  X   X   X   X X X X X X X X X   X X X   X X \n"
		"  X   X X X   X               X   X X       X \n"
		"  X X   X X   X   X X X X X   X   X X X X   X \n"
		"  X X   X X   X   X       X   X   X   X X X   \n"
		"  X   X   X   X   X   X   X   X   X   X   X   \n"
		"  X X X   X   X   X       X   X   X X   X X   \n"
		"  X X X X X   X   X X X X X   X   X X X   X X \n"
		"X X   X X X   X               X   X   X X   X \n"
		"  X       X   X X X X X X X X X   X   X     X \n"
		"  X X   X X                       X X   X X   \n"
		"  X X X   X X X X X X X X X X X X X X   X X   \n"
		"X     X     X     X X   X X               X X \n"
		"X   X X X X X   X X X X X     X   X   X     X \n"
		"X X X   X X X X           X X X       X     X \n"
		"X X     X X X     X X X X     X X X     X X   \n"
		"    X X X     X X X       X X X     X X X X   \n"
		, 'X', true);

	DecoderResult result = parse(std::move(bits), false, 30, 2);
	EXPECT_EQ(result.isValid(), true);
	EXPECT_EQ(result.text(), L"88888TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT");
	EXPECT_EQ(result.symbologyIdentifier(), "]z0");
}

TEST(AZDecoderTest, DecodeTooManyErrors)
{
	auto bits = ParseBitMatrix(
		"X X . X . . . X X . . . X . . X X X . X . X X X X X . \n"
		"X X . . X X . . . . . X X . . . X X . . . X . X . . X \n"
		"X . . . X X . . X X X . X X . X X X X . X X . . X . . \n"
		". . . . X . X X . . X X . X X . X . X X X X . X . . X \n"
		"X X X . . X X X X X . . . . . X X . . . X . X . X . X \n"
		"X X . . . . . . . . X . . . X . X X X . X . . X . . . \n"
		"X X . . X . . . . . X X . . . . . X . . . . X . . X X \n"
		". . . X . X . X . . . . . X X X X X X . . . . . . X X \n"
		"X . . . X . X X X X X X . . X X X . X . X X X X X X . \n"
		"X . . X X X . X X X X X X X X X X X X X . . . X . X X \n"
		". . . . X X . . . X . . . . . . . X X . . . X X . X . \n"
		". . . X X X . . X X . X X X X X . X . . X . . . . . . \n"
		"X . . . . X . X . X . X . . . X . X . X X . X X . X X \n"
		"X . X . . X . X . X . X . X . X . X . . . . . X . X X \n"
		"X . X X X . . X . X . X . . . X . X . X X X . . . X X \n"
		"X X X X X X X X . X . X X X X X . X . X . X . X X X . \n"
		". . . . . . . X . X . . . . . . . X X X X . . . X X X \n"
		"X X . . X . . X . X X X X X X X X X X X X X . . X . X \n"
		"X X X . X X X X . . X X X X . . X . . . . X . . X X X \n"
		". . . . X . X X X . . . . X X X X . . X X X X . . . . \n"
		". . X . . X . X . . . X . X X . X X . X . . . X . X . \n"
		"X X . . X . . X X X X X X X . . X . X X X X X X X . . \n"
		"X . X X . . X X . . . . . X . . . . . . X X . X X X . \n"
		"X . . X X . . X X . X . X . . . . X . X . . X . . X . \n"
		"X . X . X . . X . X X X X X X X X . X X X X . . X X . \n"
		"X X X X . . . X . . X X X . X X . . X . . . . X X X . \n"
		"X X . X . X . . . X . X . . . . X X . X . . X X . . . \n"
		, 'X', true);

	DecoderResult result = parse(std::move(bits), true, 16, 4);
	EXPECT_EQ(result.error(), Error::Checksum);
}

TEST(AZDecoderTest, DecodeTooManyErrors2)
{
	auto bits = ParseBitMatrix(
		". X X . . X . X X . . . X . . X X X . . . X X . X X . \n"
		"X X . X X . . X . . . X X . . . X X . X X X . X . X X \n"
		". . . . X . . . X X X . X X . X X X X . X X . . X . . \n"
		"X . X X . . X . . . X X . X X . X . X X . . . . . X . \n"
		"X X . X . . X . X X . . . . . X X . . . . . X . . . X \n"
		"X . . X . . . . . . X . . . X . X X X X X X X . . . X \n"
		"X . . X X . . X . . X X . . . . . X . . . . . X X X . \n"
		". . X X X X . X . . . . . X X X X X X . . . . . . X X \n"
		"X . . . X . X X X X X X . . X X X . X . X X X X X X . \n"
		"X . . X X X . X X X X X X X X X X X X X . . . X . X X \n"
		". . . . X X . . . X . . . . . . . X X . . . X X . X . \n"
		". . . X X X . . X X . X X X X X . X . . X . . . . . . \n"
		"X . . . . X . X . X . X . . . X . X . X X . X X . X X \n"
		"X . X . . X . X . X . X . X . X . X . . . . . X . X X \n"
		"X . X X X . . X . X . X . . . X . X . X X X . . . X X \n"
		"X X X X X X X X . X . X X X X X . X . X . X . X X X . \n"
		". . . . . . . X . X . . . . . . . X X X X . . . X X X \n"
		"X X . . X . . X . X X X X X X X X X X X X X . . X . X \n"
		"X X X . X X X X . . X X X X . . X . . . . X . . X X X \n"
		". . X X X X X . X . . . . X X X X . . X X X . X . X . \n"
		". . X X . X . X . . . X . X X . X X . . . . X X . . . \n"
		"X . . . X . X . X X X X X X . . X . X X X X X . X . . \n"
		". X . . . X X X . . . . . X . . . . . X X X X X . X . \n"
		"X . . X . X X X X . X . X . . . . X . X X . X . . X . \n"
		"X . . . X X . X . X X X X X X X X . X X X X . . X X . \n"
		". X X X X . . X . . X X X . X X . . X . . . . X X X . \n"
		"X X . . . X X . . X . X . . . . X X . X . . X . X . X \n"
		, 'X', true);

	DecoderResult result = parse(std::move(bits), true, 16, 4);
	EXPECT_EQ(result.error(), Error::Checksum);
}

// Helper taking bit string to call GetEncodedData()
static DecoderResult getData(std::string_view bitStr)
{
	BitArray bits;

	for (auto b : bitStr)
		bits.appendBit(b == '1');

	return Aztec::Decode(bits);
}

TEST(AZDecoderTest, InitialGS)
{
	// Issue #916 The first byte is lost when recognizing the Aztec code
	{
		// Initial <GS>
		auto data = getData("1111101000000111010101010100010000100101001110001011100111000101001111111111111");
		EXPECT_EQ(data.symbologyIdentifier(), "]z0");
		EXPECT_EQ(data.content().text(TextMode::Hex), "1D 55 10 94 E2 E7 14 FF");
	}
	{
		// Initial FNC1 (invalid GS1 data)
		auto data = getData("00000000000001011011111001100001000010010100111000101110011100010100111111111111");
		EXPECT_EQ(data.symbologyIdentifier(), "]z1");
		EXPECT_EQ(data.content().text(TextMode::Hex), "55 10 94 E2 E7 14 FF");
	}
}

// Shorthand to check SymbologyIdentifier result
static void check_si(int line, const DecoderResult& res, const std::string& si, const std::string& text,
					 int sa_index = -1, int sa_count = -1,
					 const std::string& textECI = {}, const std::string& bytesECI = {})
{
	EXPECT_EQ(res.symbologyIdentifier(), si) << "line:" << line;
	EXPECT_EQ(res.content().text(TextMode::Plain), text) << "line:" << line;
	EXPECT_EQ(res.structuredAppend().index, sa_index) << "line:" << line;
	EXPECT_EQ(res.structuredAppend().count, sa_count) << "line:" << line;
	if (!textECI.empty())
		EXPECT_EQ(res.content().text(TextMode::ECI), textECI) << "line:" << line;
	if (!bytesECI.empty())
		EXPECT_EQ(ToHex(res.content().bytesECI()), bytesECI) << "line:" << line;
}

TEST(AZDecoderTest, SymbologyIdentifier)
{
	// Plain
	check_si(__LINE__, getData("00010"), "]z0", "A", -1, -1, "]z3\\000026A", "5D 7A 30 41");

	// GS1 ("PS FLGN(0) DL (20)01")
	check_si(__LINE__, getData("0000000000000111100100001000100011"), "]z1", "2001");

	// AIM ("A PS FLGN(0) B")
	check_si(__LINE__, getData("00010000000000000000011"), "]z2", "AB");

	// AIM ("DL 99 UL PS FLGN(0) B")
	check_si(__LINE__, getData("11110101110111110000000000000000011"), "]z2", "99B");

	// Structured Append (no ID) ("UL ML A D A")
	check_si(__LINE__, getData("1110111101000100010100010"), "]z6", "A", 0, 4);

	// Structured Append (no ID) with GS1 ("UL ML A D PS FLGN(0) DL (20)01")
	check_si(__LINE__, getData("111011110100010001010000000000000111100100001000100011"), "]z7", "2001", 0, 4);

	// Structured Append (no ID) with AIM ("UL ML A D A PS FLGN(0) B")
	check_si(__LINE__, getData("1110111101000100010100010000000000000000011"), "]z8", "AB", 0, 4);

	// Plain with FNC1 not in first/second position ("A B PS FLGN(0) C")
	check_si(__LINE__, getData("0001000011000000000000000100"), "]z0", "AB\u001DC"); // "AB<GS>C"

	// Plain with FNC1 not in first/second position ("A B C PS FLGN(0) D")
	check_si(__LINE__, getData("000100001100100000000000000000101"), "]z0", "ABC\u001DD"); // "ABC<GS>D"

	// Plain with FNC1 not in first/second position ("DL 1 UL PS FLGN(0) A")
	check_si(__LINE__, getData("1111000111110000000000000000010"), "]z0", "1\u001DA"); // "1<GS>D"

	// ECI 3 with Plain - `res.symbologyIdentifier()` would be "]z3" if used `toString(hasECI())`
	check_si(__LINE__, getData("0000000000001010100010"), "]z0", "A", -1, -1, "]z3\\000026A", "5D 7A 33 5C 30 30 30 30 30 33 41");

	// ECI 3 with Plain, showing doubled backslash and ISO/IEC 8859-1 `bytesECI()` - "]z3" ditto
	check_si(__LINE__, getData("000000000000101010001011101101011110100011111110000111101001"), "]z0", "A\\Bé", -1, -1,
			 "]z3\\000026A\\\\Bé", "5D 7A 33 5C 30 30 30 30 30 33 41 5C 5C 42 E9");

	// ECI 3 with GS1 - "]z4" ditto
	check_si(__LINE__, getData("000000000000000000000000010101111100100001000100011"), "]z1", "2001");

	// ECI 3 with AIM - "]z5" ditto
	check_si(__LINE__, getData("0000000000001010100010000000000000000011"), "]z2", "AB");

	// ECI 3 with Structured Append (no ID) - "]z9" ditto
	check_si(__LINE__, getData("111011110100010001010000000000001010100010"), "]z6", "A", 0, 4);

	// ECI 3 with Structured Append (no ID) with GS1 - "]zA" ditto
	check_si(__LINE__, getData("11101111010001000101000000000000000000000000010101111100100001000100011"), "]z7", "2001", 0, 4, "]zA\\0000262001");

	// ECI 3 with Structured Append (no ID) with AIM - "]zB" ditto
	check_si(__LINE__, getData("111011110100010001010000000000001010100010000000000000000011"), "]z8", "AB", 0, 4, "]zB\\000026AB");
}

// Helper taking 5-bit word array to call GetEncodedData()
static DecoderResult getData(const ByteArray& bytes)
{
	BitArray bits; // 5-bit words (assuming no digits/binary)

	for (auto b : bytes)
		bits.appendBits(b, 5);

	return Aztec::Decode(bits);
}

// Shorthand to return Structured Append given 5-bit word array
static StructuredAppendInfo sai(const ByteArray& bytes)
{
	return getData(bytes).structuredAppend();
}

// Shorthand to return string result given 5-bit word array
static std::wstring text(const ByteArray& bytes)
{
	return getData(bytes).text();
}

TEST(AZDecoderTest, StructuredAppend)
{
	// Null
	EXPECT_EQ(sai({2}).index, -1);
	EXPECT_EQ(sai({2}).count, -1);
	EXPECT_TRUE(sai({2}).id.empty());
	EXPECT_EQ(text({2}), L"A");

	// Example from ISO/IEC 24778:2008 Section 8
	EXPECT_EQ(sai({29, 29, 2, 5, 2}).index, 0); // AD
	EXPECT_EQ(sai({29, 29, 2, 5, 2}).count, 4);
	EXPECT_TRUE(sai({29, 29, 2, 5, 2}).id.empty());
	EXPECT_EQ(text({29, 29, 2, 5, 2}), L"A");

	EXPECT_EQ(sai({29, 29, 3, 5, 2}).index, 1); // BD
	EXPECT_EQ(sai({29, 29, 3, 5, 2}).count, 4);
	EXPECT_TRUE(sai({29, 29, 3, 5, 2}).id.empty());
	EXPECT_EQ(text({29, 29, 3, 5, 2}), L"A");

	EXPECT_EQ(sai({29, 29, 4, 5, 2}).index, 2); // CD
	EXPECT_EQ(sai({29, 29, 4, 5, 2}).count, 4);
	EXPECT_TRUE(sai({29, 29, 4, 5, 2}).id.empty());
	EXPECT_EQ(text({29, 29, 4, 5, 2}), L"A");

	EXPECT_EQ(sai({29, 29, 5, 5, 2}).index, 3); // DD
	EXPECT_EQ(sai({29, 29, 5, 5, 2}).count, 4);
	EXPECT_TRUE(sai({29, 29, 5, 5, 2}).id.empty());
	EXPECT_EQ(text({29, 29, 5, 5, 2}), L"A");

	// Sequencing field
	EXPECT_EQ(sai({29, 29, 2, 27, 2}).index, 0); // AZ
	EXPECT_EQ(sai({29, 29, 2, 27, 2}).count, 26);

	EXPECT_EQ(sai({29, 29, 14, 27, 2}).index, 12); // MZ
	EXPECT_EQ(sai({29, 29, 14, 27, 2}).count, 26);

	EXPECT_EQ(sai({29, 29, 27, 27, 2}).index, 25); // ZZ
	EXPECT_EQ(sai({29, 29, 27, 27, 2}).count, 26);

	// Id
	EXPECT_EQ(sai({29, 29, 1, 10, 5, 1, 2, 5, 2}).id, "ID");
	EXPECT_EQ(text({29, 29, 1, 10, 5, 1, 2, 5, 2}), L"A");

	// Invalid sequencing
	EXPECT_EQ(sai({29, 29, 2, 2, 2}).index, 0); // AA
	EXPECT_EQ(sai({29, 29, 2, 2, 2}).count, 0); // Count 1 so set to 0
	EXPECT_EQ(text({29, 29, 2, 2, 2}), L"A");

	EXPECT_EQ(sai({29, 29, 6, 5, 2}).index, 4); // ED
	EXPECT_EQ(sai({29, 29, 6, 5, 2}).count, 0); // Count 4 <= index 4 so set to 0
	EXPECT_EQ(text({29, 29, 6, 5, 2}), L"A");

	EXPECT_EQ(sai({29, 29, 1, 5, 2}).index, -1); // Index < 'A'
	EXPECT_EQ(sai({29, 29, 1, 5, 2}).count, -1);
	EXPECT_EQ(text({29, 29, 1, 5, 2}), L" DA"); // Bad sequencing left in result

	EXPECT_EQ(sai({29, 29, 28, 5, 2}).index, -1); // Index > 'Z' (LL)
	EXPECT_EQ(sai({29, 29, 28, 5, 2}).count, -1);
	EXPECT_EQ(text({29, 29, 28, 5, 2}), L"da");

	EXPECT_EQ(sai({29, 29, 2, 1, 2}).index, -1); // Count < 'A'
	EXPECT_EQ(sai({29, 29, 2, 1, 2}).count, -1);
	EXPECT_EQ(text({29, 29, 2, 1, 2}), L"A A");

	EXPECT_EQ(sai({29, 29, 2, 28, 2}).index, -1); // Count > 'Z'
	EXPECT_EQ(sai({29, 29, 2, 28, 2}).count, -1);
	EXPECT_EQ(text({29, 29, 2, 28, 2}), L"Aa");

	EXPECT_EQ(sai({29, 29, 2, 5}).index, -1); // Sequencing but no data
	EXPECT_EQ(sai({29, 29, 2, 5}).count, -1);
	EXPECT_EQ(text({29, 29, 2, 5}), L"AD");

	// Invalid Ids
	{
		auto data = getData({29, 29, 1, 10, 5, 2, 5, 2}); // No terminating space
		EXPECT_TRUE(data.structuredAppend().id.empty());
		EXPECT_EQ(data.structuredAppend().index, -1); // Not recognized as sequence
		EXPECT_EQ(data.structuredAppend().count, -1);
		EXPECT_EQ(data.text(), L" IDADA"); // Bad ID and sequencing left in result
	}
	{
		auto data = getData({29, 29, 1, 1, 2, 5, 2}); // Blank
		EXPECT_TRUE(data.structuredAppend().id.empty());
		EXPECT_EQ(data.structuredAppend().index, 0); // Recognized as sequence
		EXPECT_EQ(data.structuredAppend().count, 4);
		EXPECT_EQ(data.text(), L"A");
	}
	{
		auto data = getData({29, 29, 1, 10, 1, 5, 1, 2, 5, 2}); // Space in "I D"
		EXPECT_TRUE(data.structuredAppend().id.empty());
		EXPECT_EQ(data.structuredAppend().index, -1); // Not recognized as sequence as sequence count invalid (space)
		EXPECT_EQ(data.structuredAppend().count, -1);
		EXPECT_EQ(data.text(), L" I D ADA"); // Bad ID and sequencing left in result
	}
	{
		auto data = getData({29, 29, 1, 10, 1, 2, 5, 1, 2, 5, 2}); // "I AD" (happens to have valid sequencing at end)
		EXPECT_EQ(data.structuredAppend().id, "I");
		EXPECT_EQ(data.structuredAppend().index, 0);
		EXPECT_EQ(data.structuredAppend().count, 4);
		EXPECT_EQ(data.text(), L" ADA"); // Trailing space and "real" sequencing left in result
	}
}
