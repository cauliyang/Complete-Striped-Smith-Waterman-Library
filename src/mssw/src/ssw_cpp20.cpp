
// ssw_cpp.cpp
// Created by Wan-Ping Lee
// Last revision by Mengyao Zhao on 2017-05-30
// Last revision by Yangyang Li on 2022-10-05

#include "ssw_cpp20.hpp"

#include <array>
#include <sstream>

#include "ssw.h"

namespace {

  constexpr const std::array<int8_t, 128> kBaseTranslation{
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      4, 4,
      //   A     C            G
      4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
      //             T
      4, 4, 4, 4, 3, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
      //   a     c            g
      4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
      //             t
      4, 4, 4, 4, 3, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

  void BuildSwScoreMatrix(uint8_t match_score, uint8_t mismatch_penalty, int8_t* matrix) {
    // The score matrix looks like
    //                 // A,  C,  G,  T,  N
    //  score_matrix_ = { 2, -2, -2, -2, -2, // A
    //                   -2,  2, -2, -2, -2, // C
    //                   -2, -2,  2, -2, -2, // G
    //                   -2, -2, -2,  2, -2, // T
    //                   -2, -2, -2, -2, -2};// N

    int id = 0;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        matrix[id] = ((i == j) ? static_cast<int8_t>(match_score)
                               : static_cast<int8_t>(-mismatch_penalty));
        ++id;
      }
      matrix[id] = static_cast<int8_t>(-mismatch_penalty);  // For N
      ++id;
    }

    for (int i = 0; i < 5; ++i) matrix[id++] = static_cast<int8_t>(-mismatch_penalty);  // For N
  }

  void ConvertAlignment(const s_align& s_al, const int& query_len,
                        StripedSmithWaterman::Alignment* al) {
    al->sw_score = s_al.score1;
    al->sw_score_next_best = s_al.score2;
    al->ref_begin = s_al.ref_begin1;
    al->ref_end = s_al.ref_end1;
    al->query_begin = s_al.read_begin1;
    al->query_end = s_al.read_end1;
    al->ref_end_next_best = s_al.ref_end2;

    al->cigar.clear();
    al->cigar_string.clear();

    if (s_al.cigarLen > 0) {
      std::ostringstream cigar_string;
      if (al->query_begin > 0) {
        uint32_t cigar = to_cigar_int(al->query_begin, 'S');
        al->cigar.push_back(cigar);
        cigar_string << al->query_begin << 'S';
      }

      for (int i = 0; i < s_al.cigarLen; ++i) {
        al->cigar.push_back(s_al.cigar[i]);
        cigar_string << cigar_int_to_len(s_al.cigar[i]) << cigar_int_to_op(s_al.cigar[i]);
      }

      if (int end = query_len - al->query_end - 1; end > 0) {
        uint32_t cigar = to_cigar_int(end, 'S');
        al->cigar.push_back(cigar);
        cigar_string << end << 'S';
      }

      al->cigar_string = cigar_string.str();
    }  // end if
  }

  // @Function:
  //     Calculate the length of the previous cigar operator
  //     and store it in new_cigar and new_cigar_string.
  //     Clean up in_M (false), in_X (false), length_M (0), and length_X(0).
  void CleanPreviousMOperator(bool& in_M, bool& in_X, uint32_t& length_M, uint32_t& length_X,
                              std::vector<uint32_t>& new_cigar,
                              std::ostringstream& new_cigar_string) {
    if (in_M) {
      uint32_t match = to_cigar_int(length_M, '=');
      new_cigar.push_back(match);
      new_cigar_string << length_M << '=';
    } else if (in_X) {  // in_X
      uint32_t match = to_cigar_int(length_X, 'X');
      new_cigar.push_back(match);
      new_cigar_string << length_X << 'X';
    }

    // Clean up
    in_M = false;
    in_X = false;
    length_M = 0;
    length_X = 0;
  }

  // @Function:
  //     1. Calculate the number of mismatches.
  //     2. Modify the cigar string:
  //         differentiate matches (M) and mismatches(X).
  // @Return:
  //     The number of mismatches.
  int CalculateNumberMismatch(StripedSmithWaterman::Alignment* al, int8_t const* ref,
                              int8_t const* query, int query_len) {
    ref += al->ref_begin;
    query += al->query_begin;
    int mismatch_length = 0;

    std::vector<uint32_t> new_cigar;
    std::ostringstream new_cigar_string;

    if (al->query_begin > 0) {
      uint32_t cigar = to_cigar_int(al->query_begin, 'S');
      new_cigar.push_back(cigar);
      new_cigar_string << al->query_begin << 'S';
    }

    bool in_M = false;  // the previous is match
    bool in_X = false;  // the previous is mismatch
    uint32_t length_M = 0;
    uint32_t length_X = 0;

    for (unsigned int i = 0; i < al->cigar.size(); ++i) {
      char op = cigar_int_to_op(al->cigar[i]);
      uint32_t length = cigar_int_to_len(al->cigar[i]);
      if (op == 'M') {
        for (uint32_t j = 0; j < length; ++j) {
          if (*ref != *query) {
            ++mismatch_length;
            if (in_M) {  // the previous is match; however the current one is mismatch
              uint32_t match = to_cigar_int(length_M, '=');
              new_cigar.push_back(match);
              new_cigar_string << length_M << '=';
            }
            length_M = 0;
            ++length_X;
            in_M = false;
            in_X = true;
          } else {       // *ref == *query
            if (in_X) {  // the previous is mismatch; however the current one is match
              uint32_t match = to_cigar_int(length_X, 'X');
              new_cigar.push_back(match);
              new_cigar_string << length_X << 'X';
            }
            ++length_M;
            length_X = 0;
            in_M = true;
            in_X = false;
          }  // end of if (*ref != *query)
          ++ref;
          ++query;
        }
      } else if (op == 'I') {
        query += length;
        mismatch_length += static_cast<int>(length);
        CleanPreviousMOperator(in_M, in_X, length_M, length_X, new_cigar, new_cigar_string);
        new_cigar.push_back(al->cigar[i]);
        new_cigar_string << length << 'I';
      } else if (op == 'D') {
        ref += length;
        mismatch_length += static_cast<int>(length);
        CleanPreviousMOperator(in_M, in_X, length_M, length_X, new_cigar, new_cigar_string);
        new_cigar.push_back(al->cigar[i]);
        new_cigar_string << length << 'D';
      }
    }

    CleanPreviousMOperator(in_M, in_X, length_M, length_X, new_cigar, new_cigar_string);

    if (int end = query_len - al->query_end - 1; end > 0) {
      uint32_t cigar = to_cigar_int(end, 'S');
      new_cigar.push_back(cigar);
      new_cigar_string << end << 'S';
    }

    al->cigar_string.clear();
    al->cigar.clear();
    al->cigar_string = new_cigar_string.str();
    al->cigar = new_cigar;

    return mismatch_length;
  }

  void SetFlag(const StripedSmithWaterman::Filter& filter, uint8_t& flag) {
    if (filter.report_begin_position) flag |= 0x08;
    if (filter.report_cigar) flag |= 0x0f;
  }

}  // namespace

namespace StripedSmithWaterman {

  Aligner::Aligner() { BuildDefaultMatrix(); }

  Aligner::Aligner(uint8_t match_score, uint8_t mismatch_penalty, uint8_t gap_opening_penalty,
                   uint8_t gap_extending_penalty)
      : match_score_(match_score),
        mismatch_penalty_(mismatch_penalty),
        gap_opening_penalty_(gap_opening_penalty),
        gap_extending_penalty_(gap_extending_penalty) {
    BuildDefaultMatrix();
  }

  Aligner::Aligner(const int8_t* score_matrix, int score_matrix_size,
                   const int8_t* translation_matrix, int translation_matrix_size)

      : score_matrix_size_(score_matrix_size) {
    score_matrix_.reserve(score_matrix_size_ * score_matrix_size_);
    translation_matrix_.reserve(translation_matrix_size);

    std::copy(score_matrix, score_matrix + score_matrix_size_ * score_matrix_size_,
              std::back_inserter(score_matrix_));
    std::copy(translation_matrix, translation_matrix + translation_matrix_size,
              std::back_inserter(translation_matrix_));
  }

  Aligner::~Aligner() { Clear(); }

  int Aligner::SetReferenceSequence(const char* seq, int length) {
    int len = 0;
    if (!translation_matrix_.empty()) {
      // calculate the valid length
      int valid_length = length;
      // delete the current buffer
      CleanReferenceSequence();
      // allocate a new buffer
      translated_reference_.reserve(valid_length);

      len = TranslateBase(seq, valid_length, translated_reference_.data());
    } else {
      // nothing
    }

    reference_length_ = len;
    return len;
  }

  int Aligner::TranslateBase(const char* bases, const int& length, int8_t* translated) const {
    const char* ptr = bases;
    int len = 0;
    for (int i = 0; i < length; ++i) {
      translated[i] = translation_matrix_[(int)*ptr];
      ++ptr;
      ++len;
    }

    return len;
  }

  bool Aligner::Align(const char* query, const Filter& filter, Alignment* alignment,
                      int32_t maskLen) const {
    if (translation_matrix_.empty()) return false;

    if (reference_length_ == 0) return false;

    auto query_len = static_cast<int>(strlen(query));
    if (query_len == 0) return false;

    std::vector<int8_t> translated_query{};
    translated_query.reserve(query_len);

    TranslateBase(query, query_len, translated_query.data());

    const int8_t score_size = 2;
    s_profile* profile = ssw_init(translated_query.data(), query_len, score_matrix_.data(),
                                  score_matrix_size_, score_size);

    uint8_t flag = 0;
    SetFlag(filter, flag);
    s_align* s_al = ssw_align(profile, translated_reference_.data(), reference_length_,
                              gap_opening_penalty_, gap_extending_penalty_, flag,
                              filter.score_filter, filter.distance_filter, maskLen);

    alignment->Clear();
    ConvertAlignment(*s_al, query_len, alignment);
    alignment->mismatches = CalculateNumberMismatch(alignment, translated_reference_.data(),
                                                    translated_query.data(), query_len);

    // Free memory
    align_destroy(s_al);
    init_destroy(profile);

    return true;
  }

  bool Aligner::Align(const char* query, const char* ref, int ref_len, const Filter& filter,
                      Alignment* alignment, int32_t maskLen) const {
    if (translation_matrix_.empty()) return false;

    auto query_len = static_cast<int>(strlen(query));
    if (query_len == 0) return false;

    std::vector<int8_t> translated_query{};
    translated_query.reserve(query_len);

    TranslateBase(query, query_len, translated_query.data());

    // calculate the valid length
    int valid_ref_len = ref_len;
    std::vector<int8_t> translated_ref{};
    translated_ref.reserve(valid_ref_len);

    TranslateBase(ref, valid_ref_len, translated_ref.data());

    const int8_t score_size = 2;
    s_profile* profile = ssw_init(translated_query.data(), query_len, score_matrix_.data(),
                                  score_matrix_size_, score_size);

    uint8_t flag = 0;
    SetFlag(filter, flag);
    s_align* s_al = ssw_align(profile, translated_ref.data(), valid_ref_len, gap_opening_penalty_,
                              gap_extending_penalty_, flag, filter.score_filter,
                              filter.distance_filter, maskLen);

    alignment->Clear();
    ConvertAlignment(*s_al, query_len, alignment);
    alignment->mismatches = CalculateNumberMismatch(alignment, translated_ref.data(),
                                                    translated_query.data(), query_len);

    align_destroy(s_al);
    init_destroy(profile);

    return true;
  }

  void Aligner::Clear() {
    ClearMatrices();
    CleanReferenceSequence();
  }

  void Aligner::SetAllDefault() {
    score_matrix_size_ = 5;
    match_score_ = 2;
    mismatch_penalty_ = 2;
    gap_opening_penalty_ = 3;
    gap_extending_penalty_ = 1;
    reference_length_ = 0;
  }

  bool Aligner::ReBuild() {
    if (!translation_matrix_.empty()) return false;

    SetAllDefault();
    BuildDefaultMatrix();

    return true;
  }

  bool Aligner::ReBuild(uint8_t match_score, uint8_t mismatch_penalty, uint8_t gap_opening_penalty,
                        uint8_t gap_extending_penalty) {
    if (!translation_matrix_.empty()) return false;

    SetAllDefault();

    match_score_ = match_score;
    mismatch_penalty_ = mismatch_penalty;
    gap_opening_penalty_ = gap_opening_penalty;
    gap_extending_penalty_ = gap_extending_penalty;

    BuildDefaultMatrix();

    return true;
  }

  bool Aligner::ReBuild(const int8_t* score_matrix, int score_matrix_size,
                        const int8_t* translation_matrix, int translation_matrix_size) {
    ClearMatrices();

    score_matrix_.reserve(score_matrix_size * score_matrix_size);
    std::copy(score_matrix, score_matrix + score_matrix_size * score_matrix_size,
              std::back_inserter(score_matrix_));

    translation_matrix_.reserve(translation_matrix_size);

    std::copy(translation_matrix, translation_matrix + translation_matrix_size,
              std::back_inserter(translation_matrix_));

    return true;
  }

  void Aligner::BuildDefaultMatrix() {
    ClearMatrices();
    score_matrix_.reserve(score_matrix_size_ * score_matrix_size_);
    BuildSwScoreMatrix(match_score_, mismatch_penalty_, score_matrix_.data());

    translation_matrix_.reserve(kBaseTranslation.size());
    std::copy(kBaseTranslation.begin(), kBaseTranslation.end(),
              std::back_inserter(translation_matrix_));
  }

  void Aligner::ClearMatrices() {
    std::vector<int8_t>().swap(score_matrix_);
    std::vector<int8_t>().swap(translation_matrix_);
  }
}  // namespace StripedSmithWaterman
