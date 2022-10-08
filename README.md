[![pypi](https://img.shields.io/pypi/v/mssw.svg)][pypi status]
[![status](https://img.shields.io/pypi/status/mssw.svg)][pypi status]
[![python version](https://img.shields.io/pypi/pyversions/mssw)][pypi status]
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

```python
import mssw

reference = "CAGCCTTTCTGACCCGGAAATCAAAATAGGCACAACAAA"
query = "CTGAGCCGGTAAATC"
aligner = mssw.Aligner()
aligner_filter = mssw.Filter()
alignment = aligner.align(query, reference, aligner_filter)

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
