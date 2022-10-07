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


def main():
    reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
    query = "CTGAGCCGGTAAATC"
    masklen = 15
    aligner = mssw.StripedSmithWaterman.Aligner()
    aligner_filter = mssw.StripedSmithWaterman.Filter()
    alignment = mssw.StripedSmithWaterman.Alignment()
    aligner.Align(query, reference, len(reference), aligner_filter, alignment, masklen)
    print_alignment(alignment)


if __name__ == "__main__":
    main()
