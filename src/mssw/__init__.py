# !/usr/bin/env python
"""Init file for scannls package."""
__version__ = "0.0.1"
__PACKAGE_NAME__ = "mssw"

import mssw.cpp  # type: ignore
from mssw.cpp import Alignment, Filter  # type: ignore

import typing as t

__all__ = ["Aligner", "Alignment", "Filter"]


class Aligner:
    def __init__(
        self, match: int = 2, mismatch: int = 2, gap_open: int = 3, gap_extend: int = 1
    ):
        self._aligner = cpp.Aligner(match, mismatch, gap_open, gap_extend)

    def align(
        self,
        query,
        ref,
        align_filter: t.Optional[cpp.Filter] = None,
        alignment: t.Optional[cpp.Alignment] = None,
    ):
        if align_filter is None:
            align_filter = cpp.Filter()

        mask_len = max(len(ref) / 2, 15)

        if alignment is None:
            alignment = cpp.Alignment()
        self._aligner.Align(
            query, ref, len(ref), align_filter, alignment, int(mask_len)
        )
        return alignment
