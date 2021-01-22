#include <locale.h>
#include <stdio.h>
#include "./mbstrings.h"
#include "./strings.h"
#include "./wchar.h"
#include "test_runner.h"

#define FUN_STRING                                                  \
  "Oh joy! Strings are so fun. [\"hip\", \"hip\"]\n"                \
  "What did the String say to the Integer? You're not my type...\n" \
  "Eight bytes walk into a bar. The bartender asks,\n"              \
  "\"Can I get you anything?\""                                     \
  "\"Yeah,\" reply the bytes. \"Make us a Double.\""

#define FUN_STRING2                              \
  "Oh joy! Strings are so fun. Hip Hip Array!\n" \
  "What did the String say to the Integer? You're not my type...\n"

// test functions
void test_strlen() {
  // given tests
  assert_equal_int(0, strlen(""), "strlen(\"\")");
  assert_equal_int(4, strlen("asdf"), "strlen(\"asdf\")");
  assert_equal_int(5, strlen("asdf "), "strlen(\"asdf \")");
  assert_equal_int(2, strlen("\n\n"), "strlen(\"\n\n\")");
  assert_equal_int(223, strlen(FUN_STRING), "strlen(\"FUN_STRING\"");
}

void test_strncmp() {
  assert_same_sign(-2, strncmp("professor", "rpf", 3),
                   "strncmp(\"professor\", \"rpf\", 3)");
  assert_same_sign(17, strncmp("test", "case", 1),
                   "strncmp(\"test\", \"case\", 1)");
  assert_same_sign(0, strncmp("abc", "abc", 5), "strncmp(\"abc\", \"abc\", 5)");
  assert_same_sign(0, strncmp("asdf", "asde", 3),
                   "strncmp(\"asdf\", \"asde\", 3)");
  assert_same_sign(114, strncmp("rpf", "", 3), "strncmp(\"rpf\", \"\", 3)");
  assert_same_sign(0, strncmp("rpf", "", 0), "strncmp(\"rpf\", \"\", 0)");

  assert_same_sign(0, strncmp(FUN_STRING2, FUN_STRING, 5),
                   "strncmp(FUN_STRING2, FUN_STRING, 5)");
}

void test_strncpy() {
  char dest[5];
  char* src = "professor";
  strncpy(dest, src, 3);
  assert_equal_char(src[0], dest[0], 1, "strncpy(dest, src, 3) char 0");
  assert_equal_char(src[1], dest[1], 1, "strncpy(dest, src, 3) char 1");
  assert_equal_char(src[2], dest[2], 1, "strncpy(dest, src, 3) char 2");

  char buffer[10];
  strncpy(buffer, "brown", 10);
  assert_equal_str("brown", buffer, "strncpy(..., \"brown\", 10);");
  for (int i = 5; i < 10; ++i)
    assert_equal_char('\0', buffer[i], 1,
                      "strncpy(..., \"brown\", 10) null bytes at end");
}

void test_strchr() {
  assert_equal_str("defg", strchr("abcdefg", 'd'),
                   "strstr(\"abcdefg\", \"d\")");
  assert_equal_str("bbabab", strchr("abbabab", 'b'),
                   "strstr(\"abbabab\", \"b\")");
  assert_equal_str("eg", strchr("abcdeg", 'e'), "strstr(\"abcdeg\", \"e\")");
  assert_equal_str(NULL, strchr("abcdeg", 'f'), "strstr(\"abcdeg\", \"f\")");
  assert_equal_str("g", strchr("eg", 'g'), "strstr(\"eg\", \"g\")");
}

void test_strtok() {
  char str[80] = "1,2,,3,4";
  const char s[2] = ",";
  assert_equal_str("1", strtok(str, s),
                   "strtok(\"1,2,,3,4\", \",\") first time");
  assert_equal_str("2", strtok(NULL, s),
                   "strtok(\"1,2,,3,4\", \",\") second time");
  assert_equal_str("3", strtok(NULL, s),
                   "strtok(\"1,2,,3,4\", \",\") third time");
  assert_equal_str("4", strtok(NULL, s),
                   "strtok(\"1,2,,3,4\", \",\") fourth time");
  assert_equal_str(NULL, strtok(NULL, s),
                   "strtok(\"1,2,,3,4\", \",\") fifth time");

  char str2[80] = ";;;33;;6;;77;";
  const char d[3] = ";;";
  assert_equal_str("33", strtok(str2, d),
                   "strtok(\";;;33;;6;;77;\", \";;\") first time");
  assert_equal_str("6", strtok(NULL, d),
                   "strtok(\";;;33;;6;;77;\", \";;\") second time");
  assert_equal_str("77", strtok(NULL, d),
                   "strtok(\";;;33;;6;;77;\", \";;\") third time");
  assert_equal_str(NULL, strtok(NULL, d),
                   "strtok(\";;;33;;6;;77;\", \";;\") fourth time");
}

void test_mbslen() {
  setlocale(LC_ALL, "en_US.UTF-8");

  // https://www.w3.org/2001/06/utf-8-test/UTF-8-demo.html
  // https://mothereff.in/byte-counter

  // ASCII strings have the obvious lengths
  assert_equal_int(0, mbslen(""), "ascii0");
  assert_equal_int(0, mbslen("\0"), "ascii0");
  assert_equal_int(1, mbslen("1"), "ascii1");
  assert_equal_int(16, mbslen("Robert Baratheon"), "ascii16");

  // Some wacky examples
  assert_equal_int(1, mbslen("€"), "Euro");
  assert_equal_int(10, mbslen("∮ E⋅da = Q"), "Maxwell");
  assert_equal_int(46, mbslen("Οὐχὶ ταὐτὰ παρίσταταί μοι γιγνώσκειν, ὦ ἄνδρες"),
                   "Greek");

  assert_equal_int(
      56, mbslen("ᚻᛖ ᚳᚹᚫᚦ ᚦᚫᛏ ᚻᛖ ᛒᚢᛞᛖ ᚩᚾ ᚦᚫᛗ ᛚᚪᚾᛞᛖ ᚾᚩᚱᚦᚹᛖᚪᚱᛞᚢᛗ ᚹᛁᚦ ᚦᚪ ᚹᛖᛥᚫ"),
      "Runes");
  assert_equal_int(43, mbslen("⡍⠜⠇⠑⠹ ⠺⠁⠎ ⠙⠑⠁⠙⠒ ⠞⠕ ⠃⠑⠛⠔ ⠺⠊⠹⠲ ⡹⠻⠑ ⠊⠎ ⠝⠕ ⠙⠳⠃⠞"),
                   "Braille");
  assert_equal_int(19, mbslen("እግርህን በፍራሽህ ልክ ዘርጋ።"), "Ethiopia");
  assert_equal_int(70,
                   mbslen("صثيسصلرتهغلىنهقءتععععقي "
                          "اترتارىنخاىججفقلببيشضص111صيسغ ىنؤؤربام ظظظزمطت"),
                   "Arabic");
  assert_equal_int(1, mbslen("﷽"), "Allah..gracious..merciful");
  assert_equal_int(7, mbslen("𒀀𒀁𒀂𒀃𒀄𒁃𒁂"), "Sumerian Cuneiform");

  assert_equal_int(66,
                   mbslen("Bro 😳 is ➖ it possible ❓ to somehow 🧙‍♂️ "
                          "port this ☝️ to android 🍎"),
                   "Emoji");

  // Invalid encoding tests
  assert_equal_int(-1, mbslen(NULL), "NULL");
  assert_equal_int(-1, mbslen("\x80"), "unexpected-cont-byte1");
  assert_equal_int(-1, mbslen("\x84"), "unexpected-cont-byte2");
  assert_equal_int(-1, mbslen("abc\x88 123"), "unexpected-cont-byte3");
  assert_equal_int(-1, mbslen("abc 123 xyz \x8D"), "unexpected-cont-byte4");

  assert_equal_int(-1, mbslen("abc\xCA"), "expected-1-cont-byte-got-0");
  assert_equal_int(-1, mbslen("abc\xCA\xA8\xB1"), "expected-1-cont-byte-got-2");
  assert_equal_int(-1, mbslen("abc\xEB"), "expected-2-cont-byte-got-0");
  assert_equal_int(-1, mbslen("abc\xEB\x81\x83\xB3"),
                   "expected-2-cont-byte-got-3");
  assert_equal_int(-1, mbslen("\xF5\x81\x83\xB3\xB4"),
                   "expected-3-cont-byte-got-4");
  assert_equal_int(-1, mbslen("abc\xF7\xB4"), "expected-3-cont-byte-got-1");
  assert_equal_int(-1, mbslen("\xF6 ut"), "expected-3-cont-byte-got-0");
}
