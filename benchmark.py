# Import libraries
from Bio import Align
from Bio.Seq import Seq
from functools import wraps
import time
import mssw
from collections import defaultdict

DICT = defaultdict(list)


def read_seq_from_fa(fa_file):
    with open(fa_file) as f:
        seq = f.readlines()[1].strip()
    return seq


def timeit(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.time()
        func(*args, **kwargs)
        end = time.time()
        DICT[func.__name__].append(end - start)

    return wrapper


@timeit
def bio_python(query, reference):
    # Creating sample sequences
    seq1 = Seq(query)
    seq2 = Seq(reference)

    # Calling method
    aligner = Align.PairwiseAligner(
        mode="local",
        match_score=2,
        mismatch_score=-2,
        open_gap_score=-3,
        extend_gap_score=-1,
    )
    # Finding similarities
    alignments = aligner.align(seq2, seq1)

    # Printing best results
    # print(alignments[0])


@timeit
def mssw_align(query, reference):
    aligner = mssw.Aligner(match=2, mismatch=2, gap_open=3, gap_extend=1)
    alignment = aligner.align(query, reference)
    # print(alignment.cigar_string)


def cal_speedup(best, other, result):
    return result[other] / result[best]


def format_result(query, reference):
    print(f"query len: {len(query)}")
    print(f"reference len: {len(reference)}")

    result = {}
    best = [float("inf"), ""]
    for key, value in DICT.items():
        ave_time = sum(value) / len(value)
        result[key] = ave_time

        if ave_time < best[0]:
            best = [ave_time, key]

    for k, v in result.items():
        print(f"{k}: {v:e}")

    print(
        f"Best: {best[0]} {cal_speedup(best[1], 'bio_python', result)}x speedup than bio_python"
    )


def benchmark(query, reference, iterations=10):
    DICT.clear()
    for i in range(iterations):
        bio_python(query, reference)
        mssw_align(query, reference)
    format_result(query, reference)


def main():
    scale = 1000
    reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA" * scale
    query = "CTGAGCCGGTAAATC" * scale

    benchmark(query, reference, 1)


if __name__ == "__main__":
    main()
