// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "string_piece.h"

struct EvalString;

struct Lexer {
  Lexer() {}
  explicit Lexer(const char* input);
  void Start(StringPiece filename, StringPiece input);

  enum Token {
    ERROR,
    BUILD,
    RULE,
    DEFAULT,
    INCLUDE,
    SUBNINJA,
    IDENT,
    EQUALS,
    TEOF,
    INDENT,
    COLON,
    PIPE,
    PIPE2,
    NEWLINE,
  };

  static const char* TokenName(Token t);

  Token ReadToken();
  void UnreadToken();
  bool PeekToken(Token token);
  bool ExpectToken(Token token);
  void EatWhitespace();

  /// Read a simple identifier (a rule or variable name).
  /// Returns false if a name can't be read.
  bool ReadIdent(string* out);

  /// Read a path (complete with $escapes).
  /// Returns false only on error, returned path may be empty if a delimiter
  /// (space, newline) is hit.
  bool ReadPath(EvalString* path, string* err) {
    return ReadEvalString(path, true, err);
  }
  /// Read the value side of a var = value line (complete with $escapes).
  /// Returns false only on error.
  bool ReadVarValue(EvalString* value, string* err) {
    return ReadEvalString(value, false, err);
  }

  bool Error(const string& message, string* err);

private:
  bool ReadEvalString(EvalString* eval, bool path, string* err);

  StringPiece filename_;
  StringPiece input_;
  const char* ofs_;

  const char* last_token_;
};

