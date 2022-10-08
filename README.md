[![pypi](https://img.shields.io/pypi/v/mssw.svg)][pypi status]
[![status](https://img.shields.io/pypi/status/mssw.svg)][pypi status]
[![python version](https://img.shields.io/pypi/pyversions/mssw)][pypi status]
[![Tests](https://github.com/cauliyang/Complete-Striped-Smith-Waterman-Library/actions/workflows/tests.yml/badge.svg)](https://github.com/cauliyang/Complete-Striped-Smith-Waterman-Library/actions/workflows/tests.yml)
[![Release](https://github.com/cauliyang/Complete-Striped-Smith-Waterman-Library/actions/workflows/release.yml/badge.svg)](https://github.com/cauliyang/Complete-Striped-Smith-Waterman-Library/actions/workflows/release.yml)

[pypi status]: https://pypi.org/project/mssw/0.1.1/

# Modern C++ Binding for SSW Library

## Changes

- Add Modern C++ Binding
- Use pybind11 Binding
- Provide Python api

## Installation

```bash
$ pip install mssw
```

## Benchmark

The result is tested in Linux x86_64  4 cores and 4GB of memory.

### With BioPython

| Query Length | Reference Length | mssw Time (s) | bio python Time (s) | Speedup  |
| ------------ | ---------------- | ------------- | ------------------- | :------- |
| 15           | 39               | 4.470348e-05  | 1.424551e-04        | 3.186666 |
| 150          | 390              | 2.179623e-04 |  2.270699e-03        | 10.41785 |
| 1500         | 3900             | 1.665862e-02  | 1.534623e-01        | 9.212187 |
| 15000        | 39000            | 1.696888e+00  | 1.574137e+01        | 9.276609 |

## Usage

### Example 1: Alignment with default filter and score matrix

```python
import mssw

reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
query = "CTGAGCCGGTAAATC"
# default match: int = 2, mismatch: int = 2, gap_open: int = 3, gap_extend: int = 1
aligner = mssw.Aligner()
aligner_filter = mssw.Filter()
alignment = aligner.align(query, reference, aligner_filter)
```

### Example 2: Alignment with default filter and score matrix

```python
import mssw

reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
query = "CTGAGCCGGTAAATC"
aligner = mssw.Aligner()
alignment = aligner.align(query, reference)
```

### Example 3: Alignment with filter But custom gap open and gap extension

```python
import mssw

reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
query = "CTGAGCCGGTAAATC"
aligner = mssw.Aligner(match=3, mismatch=1, gap_open=2, gap_extend=2)
alignment = aligner.align(query, reference)
```

### Example 4: Alignment Result

```python
import mssw

reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
query = "CTGAGCCGGTAAATC"
aligner = mssw.Aligner(match=3, mismatch=1, gap_open=2, gap_extend=2)
alignment = aligner.align(query, reference)

assert alignment.sw_score == 21
assert alignment.sw_score_next_best == 2
assert alignment.ref_begin == 8
assert alignment.ref_end == 21
assert alignment.query_begin == 0
assert alignment.query_end == 14
assert alignment.ref_end_next_best == 0
assert alignment.mismatches == 2
assert alignment.cigar_string == "4=1X4=1I5="
```
