import mssw

def test_aligner():
    reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
    query = "CTGAGCCGGTAAATC"
    masklen = 15
    aligner = mssw.StripedSmithWaterman.Aligner()
    aligner_filter = mssw.StripedSmithWaterman.Filter()
    alignment = mssw.StripedSmithWaterman.Alignment()
    aligner.Align(query, reference, len(reference), aligner_filter, alignment, masklen)

    assert(alignment.sw_score == 21)
    assert(alignment.sw_score_next_best == 8)
    assert(alignment.ref_begin == 8)
    assert(alignment.ref_end == 21)
    assert(alignment.query_begin == 0)
    assert(alignment.query_end == 14)
    assert(alignment.ref_end_next_best == 4)
    assert(alignment.mismatches == 2)
    assert(alignment.cigar_string== "4=1X4=1I5=")
