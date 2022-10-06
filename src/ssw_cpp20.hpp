// ssw_cpp.h
// Created by Wan-Ping Lee
// Last revision by Mengyao Zhao on 2017-05-30
// Last revision by Yangyang Li on 2020-10-05

#ifndef COMPLETE_STRIPED_SMITH_WATERMAN_CPP_H_
#define COMPLETE_STRIPED_SMITH_WATERMAN_CPP_H_

#include <cstdint>
#include <string>
#include <vector>

namespace StripedSmithWaterman {

  struct Alignment {
    uint16_t sw_score{};            // The best alignment score
    uint16_t sw_score_next_best{};  // The next best alignment score
    int32_t ref_begin{};            // Reference begin position of the best alignment
    int32_t ref_end{};              // Reference end position of the best alignment
    int32_t query_begin{};          // Query begin position of the best alignment
    int32_t query_end{};            // Query end position of the best alignment
    int32_t ref_end_next_best{};    // Reference end position of the next best alignment
    int32_t mismatches{};           // Number of mismatches of the alignment
    std::string cigar_string{};     // Cigar string of the best alignment
    std::vector<uint32_t> cigar{};  // Cigar stored in the BAM format
                                    //   high 28 bits: length
                                    //   low 4 bits: M/I/D/S/X (0/1/2/4/8);
    void Clear() {
      sw_score = 0;
      sw_score_next_best = 0;
      ref_begin = 0;
      ref_end = 0;
      query_begin = 0;
      query_end = 0;
      ref_end_next_best = 0;
      mismatches = 0;
      cigar_string.clear();
      cigar.clear();
    };
  };

  struct Filter {
    // NOTE: No matter the filter, those five fields of Alignment will be given anyway.
    //       sw_score; sw_score_next_best; ref_end; query_end; ref_end_next_best.
    // NOTE: Only need score of alignments, please set 'report_begin_position'
    //       and 'report_cigar' false.

    bool report_begin_position{true};  // Give ref_begin and query_begin.
                                       //   If it is not set, ref_begin and query_begin are -1.
    bool report_cigar{true};           // Give cigar_string and cigar.
                                       //   report_begin_position is automatically TRUE.

    // When *report_cigar* is true and alignment passes these two filters,
    //   cigar_string and cigar will be given.
    uint16_t score_filter{0};         // score >= score_filter
    uint16_t distance_filter{32767};  // ((ref_end - ref_begin) < distance_filter) &&
                                      // ((query_end - read_begin) < distance_filter)

    Filter() = default;

    Filter(bool pos, bool cigar, uint16_t score, uint16_t dis)
        : report_begin_position(pos),
          report_cigar(cigar),
          score_filter(score),
          distance_filter(dis){};
  };

  class Aligner {
  public:
    // =========
    // @function Construct an Aligner on default values.
    //             The function will build the {A.C,G,T,N} aligner.
    //             If you target for other character aligners, then please
    //             use the other constructor and pass the corresponding matrix in.
    // =========
    Aligner();

    // =========
    // @function Construct an Aligner by assigning scores.
    //             The function will build the {A.C,G,T,N} aligner.
    //             If you target for other character aligners, then please
    //             use the other constructor and pass the corresponding matrix in.
    // =========
    Aligner(uint8_t match_score, uint8_t mismatch_penalty, uint8_t gap_opening_penalty,
            uint8_t gap_extending_penalty);

    // =========
    // @function Construct an Aligner by the specific matrixs.
    // =========
    Aligner(const int8_t* score_matrix, int score_matrix_size, const int8_t* translation_matrix,
            int translation_matrix_size);

    ~Aligner();

    // =========
    // @function Build the reference sequence and thus make
    //             Align(const char* query, s_align* alignment) function;
    //             otherwise the reference should be given when aligning.
    //           [NOTICE] If there exists a sequence, that one will be deleted
    //                    and replaced.
    // @param    seq    The reference bases;
    //                  [NOTICE] It is not necessary null terminated.
    // @param    length The length of bases will be be built.
    // @return   The length of the built bases.
    // =========
    int SetReferenceSequence(const char* seq, int length);

    void CleanReferenceSequence();

    // =========
    // @function Set penalties for opening and extending gaps
    //           [NOTICE] The defaults are 3 and 1 respectively.
    // =========
    void SetGapPenalty(uint8_t opening, uint8_t extending) {
      gap_opening_penalty_ = opening;
      gap_extending_penalty_ = extending;
    };

    // =========
    // @function Align the query againt the reference that is set by
    //             SetReferenceSequence.
    // @param    query     The query sequence.
    // @param    filter    The filter for the alignment.
    // @param    alignment The container contains the result.
    // @param    maskLen   The distance between the optimal and suboptimal alignment ending position
    // will >= maskLen. We suggest to
    //                     use readLen/2, if you don't have special concerns. Note: maskLen has to
    //                     be >= 15, otherwise this function will NOT return the suboptimal
    //                     alignment information.
    // @return   True: succeed; false: fail.
    // =========
    bool Align(const char* query, const Filter& filter, Alignment* alignment,
               int32_t maskLen) const;

    // =========
    // @function Align the query againt the reference.
    //           [NOTICE] The reference won't replace the reference
    //                      set by SetReferenceSequence.
    // @param    query     The query sequence.
    // @param    ref       The reference sequence.
    //                     [NOTICE] It is not necessary null terminated.
    // @param    ref_len   The length of the reference sequence.
    // @param    filter    The filter for the alignment.
    // @param    alignment The container contains the result.
    // @param    maskLen   The distance between the optimal and suboptimal alignment ending position
    // will >= maskLen. We suggest to
    //                     use readLen/2, if you don't have special concerns. Note: maskLen has to
    //                     be >= 15, otherwise this function will NOT return the suboptimal
    //                     alignment information.
    // @return   True: succeed; false: fail.
    // =========
    bool Align(const char* query, const char* ref, int ref_len, const Filter& filter,
               Alignment* alignment, int32_t maskLen) const;

    // @function Clear up all containers and thus the aligner is disabled.
    //             To rebuild the aligner please use Build functions.
    void Clear();

    // =========
    // @function Rebuild the aligner's ability on default values.
    //           [NOTICE] If the aligner is not cleaned, rebuilding will fail.
    // @return   True: succeed; false: fail.
    // =========
    bool ReBuild();

    // =========
    // @function Rebuild the aligner's ability by the specific matrixs.
    //           [NOTICE] If the aligner is not cleaned, rebuilding will fail.
    // @return   True: succeed; false: fail.
    // =========
    bool ReBuild(uint8_t match_score, uint8_t mismatch_penalty, uint8_t gap_opening_penalty,
                 uint8_t gap_extending_penalty);

    // =========
    // @function Construct an Aligner by the specific matrixs.
    //           [NOTICE] If the aligner is not cleaned, rebuilding will fail.
    // @return   True: succeed; false: fail.
    // =========
    bool ReBuild(const int8_t* score_matrix, int score_matrix_size,
                 const int8_t* translation_matrix, int translation_matrix_size);

  private:
    std::vector<int8_t> score_matrix_{};
    int score_matrix_size_{5};
    std::vector<int8_t> translation_matrix_{};

    uint8_t match_score_{2};            // default: 2
    uint8_t mismatch_penalty_{2};       // default: 2
    uint8_t gap_opening_penalty_{3};    // default: 3
    uint8_t gap_extending_penalty_{1};  // default: 1

    std::vector<int8_t> translated_reference_{};
    int32_t reference_length_{0};

    int TranslateBase(const char* bases, const int& length, int8_t* translated) const;
    void SetAllDefault();
    void BuildDefaultMatrix();
    void ClearMatrices();

    Aligner& operator=(const Aligner&);
    Aligner(const Aligner&);
  };  // class Aligner

  // ================
  // inline functions
  // ================
  inline void Aligner::CleanReferenceSequence() {
    if (reference_length_ == 0) return;

    std::vector<int8_t>().swap(translated_reference_);
    reference_length_ = 0;

    reference_length_ = 0;
  }
}  // namespace StripedSmithWaterman

#endif  // COMPLETE_STRIPED_SMITH_WATERMAN_CPP_H
