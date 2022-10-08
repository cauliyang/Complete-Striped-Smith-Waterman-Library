import mssw


def print_alignment(alignment):
    print("===== SSW result =====\n")
    print(f"Best Smith-Waterman score:\t{alignment.sw_score}\n")
    print(f"Next-best Smith-Waterman score:\t{alignment.sw_score_next_best}\n")
    print(f"Reference start:\t{alignment.ref_begin}\n")
    print(f"Reference end:\t{alignment.ref_end}\n")
    print(f"Query start:\t{alignment.query_begin}\n")
    print(f"Query end:\t{alignment.query_end}\n")
    print(f"Next-best reference end:\t{alignment.ref_end_next_best}\n")
    print(f"Number of mismatches:\t{alignment.mismatches}\n")
    print(f"Cigar: {alignment.cigar_string}\n")
    print(f"======================\n")


def test_aligner():
    reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
    query = "CTGAGCCGGTAAATC"
    aligner = mssw.Aligner()
    aligner_filter = mssw.Filter()
    alignment = aligner.align(query, reference, aligner_filter)
    print_alignment(alignment)
    assert alignment.sw_score == 21
    assert alignment.sw_score_next_best == 2
    assert alignment.ref_begin == 8
    assert alignment.ref_end == 21
    assert alignment.query_begin == 0
    assert alignment.query_end == 14
    assert alignment.ref_end_next_best == 0
    assert alignment.mismatches == 2
    assert alignment.cigar_string == "4=1X4=1I5="


def test_aligner_default():
    reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
    query = "CTGAGCCGGTAAATC"
    aligner = mssw.Aligner()
    alignment = aligner.align(query, reference)
    print_alignment(alignment)
    assert alignment.sw_score == 21
    assert alignment.sw_score_next_best == 2
    assert alignment.ref_begin == 8
    assert alignment.ref_end == 21
    assert alignment.query_begin == 0
    assert alignment.query_end == 14
    assert alignment.ref_end_next_best == 0
    assert alignment.mismatches == 2
    assert alignment.cigar_string == "4=1X4=1I5="


def test_aligner_with_custom_score():
    reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
    query = "CTGAGCCGGTAAATC"
    aligner = mssw.Aligner(match=3, mismatch=10, gap_open=2, gap_extend=2)
    alignment = aligner.align(query, reference)
    assert alignment.cigar_string == "4=1I1D4=1I5="
