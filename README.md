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
masklen = 15
aligner = mssw.StripedSmithWaterman.Aligner()
aligner_filter = mssw.StripedSmithWaterman.Filter()
alignment = mssw.StripedSmithWaterman.Alignment()
aligner.Align(query, reference, len(reference), aligner_filter, alignment, masklen)
```
