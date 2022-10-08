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
